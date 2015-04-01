var db = require('../models');

function isValid(formula){
  if(formula.formula == ''){
    return false;
  }
  if(formula.SensorId == ''){
    return false;
  }
  if(formula.ReviewId == ''){
    return false;
  }
  return true;
};

exports.dataView = function(req, res, next) {
  var _return = {};
  db.Sensor.findAll({
    attributes: ['id', 'description'],
    order: 'description ASC'
  }).success(function(entitiesSensor) {
    _return.sensor = entitiesSensor;
    db.Review.findAll({
      include: [ {
        model: db.TestType,
        attributes: ['id', 'description']
      }, {
        model: db.Equipment,
        attributes: ['id', 'description']
      }],
      attributes: ['id', 'description', 'createdAt'],
      limit: 10,
      order: "createdAt DESC"
    }).success(function(entitiesEquipment) {
      _return.review = entitiesEquipment;
      res.json({ success: 1, data: _return });
    }).error(function(error){
      res.json({ success: 2, message: error });
    });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};


exports.findFormulas = function(req, res, next) {
  db.Formula.findAll({
    where: {
      ReviewId: req.param('id')
    },
    attributes: ['id', 'formula', 'SensorId']
  }).success(function(entity) {
    res.json({ success: 1, data: entity || [] });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.find = function(req, res, next) {
  db.Formula.find({
    where: {
      id: req.param('id')
    },
    include: [ {
      model: db.Sensor,
      attributes: ['id', 'field', 'type', 'description'],
      include: [ {
        model: db.Case,
        attributes: ['id', 'macAddress']
      }]
    }, {
      model: db.Equipment,
      attributes: ['id', 'description', 'model', 'year']
    }],
    attributes: ['id', 'formula']
  }).success(function(entity) {
    res.json({ success: 1, data: entity });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.findAll = function(req, res, next) {
  db.Formula.findAll({
    include: [ {
      model: db.Sensor,
      attributes: ['id', 'description']
    }, {
      model: db.Equipment,
      attributes: ['id', 'description']
    }],
    attributes: ['id', 'formula']
  }).success(function(entities) {
    res.json({ success: 1, data: entities });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.persist = function(req, res, next) {
  db.Formula.find({ where: { id: req.body.id } }).success(function(entity) {
    if (entity) {
      entity.updateAttributes({ formula : req.body.formula }).success(function(entity) {
        res.json({ success: 1, message: "Fórmula atualizada com sucesso!"});
      }).error(function(error){
        res.json({ success: 0, message: "Já existe um fórmula com esses dados!"});
      });
    } else {
      if(isValid(req.body)){
        db.Formula.create(req.body).success(function(entity) {
          res.json({ success: 1, message: "Fórmula criada com sucesso!"});
        }).error(function(error){
          res.json({ success: 0, message: "Análise ou sensor inválidos!"});
        });
      }else{
        res.json({ success: 0, message: "Favor preencher todos os campos!"});
      }
    }
  })
};

exports.delete = function(req, res, next) {
  db.Formula.find({ where: { id: req.param('id') } }).success(function(entity) {
    if (entity) {
      entity.destroy().success(function() {
        res.json({ success: 1, message: "Fórmula excluída!"});
      })
    } else {
      res.json({ success: 0, message: "Fórmula não encontrado!"});
    }
  });
};