var db = require('../models'),
    _sensordata = null;

function isValid(review, sensor){
  if(review == undefined){
    return false;
  }
  if(sensor == undefined){
    return false;
  }
  return true;
};

function formatTime(secs){
  var times = new Array(3600, 60, 1);
  var time = '';
  var tmp;
  for(var i = 0; i < times.length; i++){
    tmp = Math.floor(secs / times[i]);
    if(tmp < 1){
        tmp = '00';
    }else if(tmp < 10){
      tmp = '0' + tmp;
    }
    time += tmp;
    if(i < 2){
        time += ':';
    }
    secs = secs % times[i];
  }
  return time.slice(3, 8);
};

exports.get = function(data, sensor, formula){
  return get(data, sensor, formula);
};

function get(data, sensor, formula){
  var _return = {
    cols: [
      {
        "label": "Data",
        "type": "string",
        "p": {}
      },
      {
        "label": sensor.description,
        "type": "number",
        "p": {}
      }
    ],
    rows: []
  },
  _c = [],
  x = 0,
  _temp;

  for(var i = 0; i < data.length; i++){
    _temp = (new Date(data[i].date) - new Date(data[1].date)) / 1000;
    _c.push({ "v": formatTime(_temp) });
    if(formula != ''){
      x = parseInt(data[i]['sensor' + sensor.field], 16);
      _c.push({ "v": eval(formula) });
    }else{
      _c.push({ "v": parseInt(data[i]['sensor' + sensor.field], 16) });
    }
    _return.rows.push({ c: _c });
    _c = [];
  };
  return { data: _return, sensor: sensor };
};

exports.dataViewExcel = function(req, res, next) {
  var _return = {};
  db.Equipment.findAll({
    attributes: ['id', 'description'],
    order: 'description ASC'
  }).success(function(entitiesEquipment) {
    _return.equipment = entitiesEquipment;
    res.json({ success: 1, data: _return });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.dataView = function(req, res, next) {
  var _return = {};
  db.Equipment.findAll({
    attributes: ['id', 'description'],
    order: 'description ASC'
  }).success(function(entitiesEquipment) {
    _return.equipment = entitiesEquipment;
    db.Sensor.findAll({
      attributes: ['id', 'description'],
      order: 'description ASC'
    }).success(function(entitiesSensor) {
      _return.sensor = entitiesSensor;
      res.json({ success: 1, data: _return });
    }).error(function(error){
      res.json({ success: 2, message: error });
    });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.dataViewReview = function(req, res, next) {
  db.Review.findAll({
    where: {
      EquipmentId: req.param('id'),
      status: 2
    },
    order: 'createdAt DESC',
    include: [ {
      model: db.TestType,
      attributes: ['id', 'description']
    }],
    attributes: ['id', 'description', 'createdAt']
  }).success(function(entities) {
    res.json({ success: 1, data: entities });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.find = function(req, res, next) {
  if(isValid(req.param('review'), req.param('sensor'))){
    var _return = { };
    db.Sensor.find({
      where: {
        id: req.param('sensor')
      },
      attributes: ['id', 'description', 'field'],
      include: [ {
        model: db.Case,
        attributes: ['id', 'macAddress']
      }],
    }).success(function(entitySensor) {
      if(entitySensor){
        db.SensorData.findAll({
          where: {
            macAddress: entitySensor.Case.macAddress,
            ReviewId: req.param('review')
          },
          attributes: ['id', 'date', 'counter', 'sensor' + entitySensor.field],
          order: 'counter ASC'
        }).success(function(entityData) {
          db.Formula.find({
            where: {
              ReviewId: req.param('review'),
              SensorId: req.param('sensor')
            },
            attributes: ['formula']
          }).success(function(entityFormula) {
            if(entityFormula){
              _return = get(entityData || [], entitySensor, entityFormula.formula);
              res.json({ success: 1, data: _return });
            }else{
              _return = get(entityData || [], entitySensor, '');
              res.json({ success: 1, data: _return });
            }
          });
        }).error(function(error){
          res.json({ success: 2, message: error });
        });
      }else{
        res.json({ success: 0, message: "Sensor não encontrado!"});
      }
    }).error(function(error){
      res.json({ success: 2, message: error });
    });
  }else{
    res.json({ success: 0, message: "Favor preencher todos os campos!"});
  }
};

exports.findAll = function(req, res, next) {
  if(isValid(req.param('review'), req.param('sensor'))){
    db.Sensor.find({
      where: {
        id: req.param('sensor')
      },
      attributes: ['id', 'description', 'field'],
      include: [ {
        model: db.Case,
        attributes: ['id', 'macAddress']
      }],
    }).success(function(entitySensor) {
      if(entitySensor){
        db.SensorData.findAll({
          where: {
            macAddress: entitySensor.Case.macAddress,
            ReviewId: req.param('review')
          },
          attributes: ['date', 'counter', ['sensor' + entitySensor.field, 'sensor']],
          order: 'counter ASC'
        }).success(function(entityData) {
          res.json({ success: 1, data: entityData });
        }).error(function(error){
          res.json({ success: 2, message: error });
        });
      }else{
        res.json({ success: 0, message: "Sensor não encontrado!"});
      }
    }).error(function(error){
      res.json({ success: 2, message: error });
    });
  }else{
    res.json({ success: 0, message: "Favor preencher todos os campos!"});
  }
};

exports.persist = function(obj) {
  _sensordata = {
    macAddress: obj.data.split(";")[0] || "",
    date: parseInt(obj.startDate) + (parseInt(obj.data.split(";")[1], 16) * 1000),
    counter: parseInt(obj.data.split(";")[2], 16),
    sensor1: obj.data.split(";")[3] || "",
    sensor2: obj.data.split(";")[4] || "",
    sensor3: obj.data.split(";")[5] || "",
    sensor4: obj.data.split(";")[6] || "",
    sensor5: obj.data.split(";")[7] || "",
    sensor6: obj.data.split(";")[8] || "",
    sensor7: obj.data.split(";")[9] || "",
    sensor8: obj.data.split(";")[10] || "",
    sensor9: obj.data.split(";")[11] || "",
    sensor10: obj.data.split(";")[12] || "",
    sensor11: obj.data.split(";")[13] || "",
    sensor12: obj.data.split(";")[14] || "",
    sensor13: obj.data.split(";")[15] || "",
    ReviewId: obj.reviewId
  };
  db.SensorData.create(_sensordata);
};