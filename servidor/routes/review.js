var db = require('../models');

function isValid(review){
  if(review.description == ''){
    return false;
  }
  if(review.TestTypeId == ''){
    return false;
  }
  if(review.EquipmentId == ''){
    return false;
  }
  return true;
};

function getDate(date){
  var _return = new Date(date);
  if(_return){
    return _return = _return.getDate() + "/" + (_return.getMonth() + 1) + "/" + _return.getFullYear();
  }
  return '';
};

function getTime(date){
  var _return = new Date(date);
  if(_return){
    return _return = _return.getHours() + ":" + _return.getMinutes();
  }
  return '';
};

function copyFormula(idReview){
  db.Formula.findAll({
    where: {
      ReviewId: (idReview - 1)
    },
    attributes: ['id', 'formula', 'SensorId']
  }).success(function(entities) {
    for(var i = 0; i < entities.length; i++){
      db.Formula.create({
        formula: entities[i].formula,
        ReviewId: idReview,
        SensorId: entities[i].SensorId
      });
    };
  })
};

exports.dataView = function(req, res, next) {
  var _return = {};
  db.TestType.findAll({
    attributes: ['id', 'type', 'description', 'terrain', 'weather', 'duration'],
    order: 'description ASC'
  }).success(function(entitiesTestType) {
    _return.testtype = entitiesTestType;
    db.Equipment.findAll({
      attributes: ['id', 'description', 'model', 'year'],
      order: 'description ASC'
    }).success(function(entitiesEquipment) {
      _return.equipment = entitiesEquipment;
      res.json({ success: 1, data: _return });
    }).error(function(error){
      res.json({ success: 2, message: error });
    });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};


exports.find = function(req, res, next) {
  db.Review.find({
    where: {
      id: req.param('id')
    },
    include: [ {
      model: db.TestType,
      attributes: ['id', 'type', 'description', 'terrain', 'weather', 'duration']
    }, {
      model: db.Equipment,
      attributes: ['id', 'description', 'model', 'year']
    }],
    attributes: ['id', 'description']
  }).success(function(entity) {
    res.json({ success: 1, data: entity });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.findAll = function(req, res, next) {
  db.Review.findAll({
    include: [ {
      model: db.TestType,
      attributes: ['id', 'type', 'description', 'terrain', 'weather', 'duration']
    }, {
      model: db.Equipment,
      attributes: ['id', 'description']
    }],
    attributes: ['id', 'description']
  }).success(function(entities) {
    res.json({ success: 1, data: entities });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.persist = function(req, res, next) {
  db.Review.find({ where: { id: req.body.id } }).success(function(entity) {
    if (entity) {
      entity.updateAttributes(req.body).success(function(entity) {
        res.json({ success: 1, message: "Análise atualizada com sucesso!" });
      }).error(function(error){
        res.json({ success: 0, message: "Já existe uma análise com esses dados!"});
      });
    } else {
    db.Review.count({ where: { status: [0, 1] } }).success(function(count) {
      if(count == 0){
        if(isValid(req.body)){
          req.body.UserId = req.user.id;
          req.body.status = 0;
          db.Review.create(req.body).success(function(entity) {
            if(req.body.formulas){
              copyFormula(entity.id);
            }
            res.json({ success: 1, message: "Análise criada com sucesso!" });
          }).error(function(error){
            res.json({ success: 0, message: "Tipo de teste ou equipamento inválidos!"});
          });
        }else{
          res.json({ success: 0, message: "Favor preencher todos os campos!"});
        }
      }else{
        res.json({ success: 0, message: "Já existe uma análise em andamento!"});
      }
    }).error(function(error){
      res.json({ success: 0, message: "Tipo de teste ou equipamento inválidos!"});
    });
    }
  })
};

exports.findRealTime = function(req, res, next) {
  var _return = {};
  db.Review.find({
    where: {
      status: [0, 1]
    },
    include: [ {
      model: db.TestType,
      attributes: ['id', 'description']
    }, {
      model: db.Equipment,
      attributes: ['id', 'description']
    }],
    attributes: ['id', 'status']
  }).success(function(entityReview) {
    if (entityReview) {
      _return.review = entityReview;
      db.Sensor.findAll({
        attributes: ['id', 'description'],
        order: 'description ASC'
      }).success(function(entitiesSensor) {
        _return.sensor = entitiesSensor;
        res.json({ success: 1, data: _return });
      }).error(function(error){
        res.json({ success: 2, message: error });
      });
    } else {
      res.json({ success: 1, message: "Não existe nenhuma análise em andamento!", data: { entity: null }});
    }
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.cancel = function(req, res, next) {
  db.Review.find({ where: {
    id: req.param('id'),
    status: 0
  } }).success(function(entity) {
    if (entity) {
      entity.updateAttributes({ status: 3 }).success(function(entity) {
        res.json({ success: 1, message: "Análise cancelada com sucesso!" });
      }).error(function(error){
        res.json({ success: 0, message: "Impossível cancelar análise!"});
      });
    } else {
      res.json({ success: 0, message: "Análise não encontrada!"});
    }
  })
};

exports.init = function(req, res, next) {
  db.Review.find({ where: {
    id: req.param('id'),
    status: 0
  } }).success(function(entity) {
    if (entity) {
      entity.updateAttributes({ status: 1 }).success(function(entity) {
        res.json({ success: 1, message: "Análise iniciada com sucesso!" });
      }).error(function(error){
        res.json({ success: 0, message: "Impossível iniciar análise!"});
      });
    } else {
      res.json({ success: 0, message: "Análise não encontrada!"});
    }
  })
};

exports.close = function(req, res, next) {
  db.Review.find({ where: {
    id: req.param('id'),
    status: 1
  } }).success(function(entity) {
    if (entity) {
      entity.updateAttributes({ status: 2 }).success(function(entity) {
        res.json({ success: 1, message: "Análise terminada com sucesso!" });
      }).error(function(error){
        res.json({ success: 0, message: "Impossível terminar análise!"});
      });
    } else {
      res.json({ success: 0, message: "Análise não encontrada!"});
    }
  })
};

exports.createExcel = function(req, res, next) {
  var xl = require('excel4node'),
      wb = new xl.WorkBook(),
      data = {},
      conf = [],
      style = wb.Style();

  // Definição dos sensores
  conf[0] = { sheets: null, row: null, line: 0 };
  conf[1] = { sheets: null, row: null, line: 0 };
  conf[2] = { sheets: 'Trator', row: 4, line: 0 };
  conf[3] = { sheets: 'Trator', row: 3, line: 0 };
  conf[4] = { sheets: 'Trator', row: 5, line: 0 };
  conf[5] = { sheets: 'Trator', row: 6, line: 0 };
  conf[6] = { sheets: 'Trator', row: 7, line: 0 };
  conf[7] = { sheets: 'Caixa Interna', row: 4, line: 0 };
  conf[8] = { sheets: 'Caixa Interna', row: 3, line: 0 };
  conf[9] = { sheets: 'Caixa Interna', row: 6, line: 0 };
  conf[10] = { sheets: 'Caixa Interna', row: 7, line: 0 };
  conf[11] = { sheets: 'Caixa Interna', row: 5, line: 0 };
  conf[12] = { sheets: null, row: null, line: 0 };
  conf[13] = { sheets: null, row: null, line: 0 };
  conf[14] = { sheets: null, row: null, line: 0 };
  conf[15] = { sheets: null, row: null, line: 0 };
  conf[16] = { sheets: null, row: null, line: 0 };
  conf[17] = { sheets: null, row: null, line: 0 };
  conf[18] = { sheets: null, row: null, line: 0 };
  conf[19] = { sheets: 'Trator', row: 8, line: 0 };
  conf[20] = { sheets: 'Motor', row: 4, line: 0 };
  conf[21] = { sheets: 'Motor', row: 3, line: 0 };
  conf[22] = { sheets: 'Motor', row: 5, line: 0 };
  conf[23] = { sheets: 'Motor', row: 6, line: 0 };
  conf[24] = { sheets: 'Motor', row: 7, line: 0 };
  conf[25] = { sheets: 'Motor', row: 9, line: 0 };
  conf[26] = { sheets: 'Motor', row: 8, line: 0 };
  conf[27] = { sheets: 'Motor', row: 11, line: 0 };
  conf[28] = { sheets: 'Trator', row: 12, line: 0 };
  conf[29] = { sheets: 'Caixa Externa', row: 4, line: 0 };
  conf[30] = { sheets: 'Caixa Externa', row: 3, line: 0 };
  conf[31] = { sheets: 'Caixa Externa', row: 6, line: 0 };
  conf[32] = { sheets: 'Caixa Externa', row: 7, line: 0 };
  conf[33] = { sheets: 'Caixa Externa', row: 9, line: 0 };
  conf[34] = { sheets: 'Caixa Externa', row: 8, line: 0 };
  conf[35] = { sheets: 'Caixa Externa', row: 5, line: 0 };
  conf[36] = { sheets: null, row: null, line: 0 };
  conf[37] = { sheets: null, row: null, line: 0 };
  conf[38] = { sheets: null, row: null, line: 0 };
  conf[39] = { sheets: null, row: null, line: 0 };
  conf[40] = { sheets: null, row: null, line: 0 };
  conf[41] = { sheets: null, row: null, line: 0 };
  conf[42] = { sheets: null, row: null, line: 0 };
  conf[43] = { sheets: null, row: null, line: 0 };
  conf[44] = { sheets: null, row: null, line: 0 };
  conf[45] = { sheets: null, row: null, line: 0 };
  conf[46] = { sheets: null, row: null, line: 0 };
  conf[47] = { sheets: null, row: null, line: 0 };
  conf[48] = { sheets: null, row: null, line: 0 };

  style.Font.Bold();
  style.Font.Family('Calibri');
  style.Font.Size(11);
  style.Font.Alignment.Vertical('center');
  style.Font.Alignment.Horizontal('center');
  style.Border({
    top:{
      color:'black'
    },
    bottom:{
      color:'black'
    },
    left:{
      color:'black'
    },
    right:{
      color:'black'
    }
  });

  data['Motor'] = wb.WorkSheet('Motor', {});
  data['Trator'] = wb.WorkSheet('Trator', {});
  data['Caixa Interna'] = wb.WorkSheet('Caixa Interna', {});
  data['Cooking Hacks'] = wb.WorkSheet('Cooking Hacks', {});
  data['Caixa Externa'] = wb.WorkSheet('Caixa Externa', {});

  data['Motor'].Column(1).Width(12);
  data['Motor'].Column(2).Width(8);
  data['Motor'].Column(3).Width(22);
  data['Motor'].Column(4).Width(20);
  data['Motor'].Column(5).Width(22);
  data['Motor'].Column(6).Width(20);
  data['Motor'].Column(7).Width(23);
  data['Motor'].Column(8).Width(24);
  data['Motor'].Column(9).Width(18);
  data['Motor'].Column(10).Width(16);
  data['Motor'].Column(11).Width(17);
  data['Motor'].Column(12).Width(12);
  data['Motor'].Column(13).Width(11);
  data['Motor'].Column(14).Width(11);

  data['Motor'].Cell(1,1,2,1,true).String('Data').Style(style);
  data['Motor'].Cell(1,2,2,2,true).String('Hora').Style(style);
  data['Motor'].Cell(1,3).String('Fluxômetro de Entrada').Style(style);
  data['Motor'].Cell(2,3).String('(Pulsos)').Style(style);
  data['Motor'].Cell(1,4).String('Fluxômetro de Saída').Style(style);
  data['Motor'].Cell(2,4).String('(Pulsos)').Style(style);
  data['Motor'].Cell(1,5,1,10,true).String('Temperatura (°C)').Style(style);
  data['Motor'].Cell(2,5).String('Combustível de Entrada').Style(style);
  data['Motor'].Cell(2,6).String('Combustível de Saída').Style(style);
  data['Motor'].Cell(2,7).String('Ar (Frente do Radiador)').Style(style);
  data['Motor'].Cell(2,8).String('Líquido de Arrefecimento').Style(style);
  data['Motor'].Cell(2,9).String('Gases de Escape').Style(style);
  data['Motor'].Cell(2,10).String('Óleo do Motor').Style(style);
  data['Motor'].Cell(1,11,1,12,true).String('Ar Admitida pelo Motor').Style(style);
  data['Motor'].Cell(2,11).String('Volume (cm³/seg)').Style(style);
  data['Motor'].Cell(2,12).String('O2 (ppm)').Style(style);
  data['Motor'].Cell(1,13,1,14,true).String('Gases de Escape (ppm)').Style(style);
  data['Motor'].Cell(2,13).String('CO').Style(style);
  data['Motor'].Cell(2,14).String('CO2').Style(style);

  data['Trator'].Column(1).Width(12);
  data['Trator'].Column(2).Width(8);
  data['Trator'].Column(3).Width(20);
  data['Trator'].Column(4).Width(16);
  data['Trator'].Column(5).Width(17);
  data['Trator'].Column(6).Width(16);
  data['Trator'].Column(7).Width(12);
  data['Trator'].Column(8).Width(7);
  data['Trator'].Column(9).Width(9);
  data['Trator'].Column(10).Width(14);
  data['Trator'].Column(11).Width(19);
  data['Trator'].Column(12).Width(22);
  data['Trator'].Column(13).Width(21);

  data['Trator'].Cell(1,1,2,1,true).String('Data').Style(style);
  data['Trator'].Cell(1,2,2,2,true).String('Hora').Style(style);
  data['Trator'].Cell(1,3,1,8,true).String('Encoders (Pulsos)').Style(style);
  data['Trator'].Cell(2,3).String('Dianteiro Esquerdo').Style(style);
  data['Trator'].Cell(2,4).String('Dianteiro Direito').Style(style);
  data['Trator'].Cell(2,5).String('Traseiro Esquerdo').Style(style);
  data['Trator'].Cell(2,6).String('Traseiro Direito').Style(style);
  data['Trator'].Cell(2,7).String('Roda Guia').Style(style);
  data['Trator'].Cell(2,8).String('TDP').Style(style);
  data['Trator'].Cell(1,9,2,9,true).String('Radar').Style(style);
  data['Trator'].Cell(1,10,2,10,true).String('Distância (m)').Style(style);
  data['Trator'].Cell(1,11,2,11,true).String('Velocidade (Km/h)').Style(style);
  data['Trator'].Cell(1,12,2,12,true).String('Força de Tração (kgf)').Style(style);
  data['Trator'].Cell(1,13,2,13,true).String('Torque na TDP (kgf.m)').Style(style);

  data['Caixa Interna'].Column(1).Width(12);
  data['Caixa Interna'].Column(2).Width(8);
  data['Caixa Interna'].Column(3).Width(17);
  data['Caixa Interna'].Column(4).Width(21);
  data['Caixa Interna'].Column(5).Width(13);
  data['Caixa Interna'].Column(6).Width(13);
  data['Caixa Interna'].Column(7).Width(15);

  data['Caixa Interna'].Cell(1,1,2,1,true).String('Data').Style(style);
  data['Caixa Interna'].Cell(1,2,2,2,true).String('Hora').Style(style);
  data['Caixa Interna'].Cell(1,3,2,3,true).String('Temperatura (°C)').Style(style);
  data['Caixa Interna'].Cell(1,4,2,4,true).String('Umidade Relativa (%)').Style(style);
  data['Caixa Interna'].Cell(1,5,2,5,true).String('Ruído (dBA)').Style(style);
  data['Caixa Interna'].Cell(1,6,2,6,true).String('Altitude (m)').Style(style);
  data['Caixa Interna'].Cell(1,7,2,7,true).String('Pressão (atm)').Style(style);

  data['Cooking Hacks'].Column(1).Width(12);
  data['Cooking Hacks'].Column(2).Width(8);
  data['Cooking Hacks'].Column(3).Width(9);
  data['Cooking Hacks'].Column(4).Width(9);
  data['Cooking Hacks'].Column(5).Width(9);
  data['Cooking Hacks'].Column(6).Width(9);
  data['Cooking Hacks'].Column(7).Width(9);
  data['Cooking Hacks'].Column(8).Width(9);

  data['Cooking Hacks'].Column(9).Width(9);
  data['Cooking Hacks'].Column(10).Width(9);
  data['Cooking Hacks'].Column(11).Width(9);
  data['Cooking Hacks'].Column(12).Width(9);
  data['Cooking Hacks'].Column(13).Width(9);
  data['Cooking Hacks'].Column(14).Width(9);

  data['Cooking Hacks'].Column(15).Width(27);
  data['Cooking Hacks'].Column(16).Width(31);
  data['Cooking Hacks'].Column(17).Width(25);
  data['Cooking Hacks'].Column(18).Width(16);
  data['Cooking Hacks'].Column(19).Width(16);
  data['Cooking Hacks'].Column(20).Width(20);

  data['Cooking Hacks'].Cell(1,1,2,1,true).String('Data').Style(style);
  data['Cooking Hacks'].Cell(1,2,2,2,true).String('Hora').Style(style);
  data['Cooking Hacks'].Cell(1,3,1,8,true).String('Posição do Operador').Style(style);
  data['Cooking Hacks'].Cell(2,3).String('X+').Style(style);
  data['Cooking Hacks'].Cell(2,4).String('X-').Style(style);
  data['Cooking Hacks'].Cell(2,5).String('Y+').Style(style);
  data['Cooking Hacks'].Cell(2,6).String('Y-').Style(style);
  data['Cooking Hacks'].Cell(2,7).String('Z+').Style(style);
  data['Cooking Hacks'].Cell(2,8).String('Z-').Style(style);
  data['Cooking Hacks'].Cell(1,9,1,14,true).String('Posição do Trator').Style(style);
  data['Cooking Hacks'].Cell(2,9).String('X+').Style(style);
  data['Cooking Hacks'].Cell(2,10).String('X-').Style(style);
  data['Cooking Hacks'].Cell(2,11).String('Y+').Style(style);
  data['Cooking Hacks'].Cell(2,12).String('Y-').Style(style);
  data['Cooking Hacks'].Cell(2,13).String('Z+').Style(style);
  data['Cooking Hacks'].Cell(2,14).String('Z-').Style(style);
  data['Cooking Hacks'].Cell(1,15,2,15,true).String('Batimentos Cardíacos (Bpm)').Style(style);
  data['Cooking Hacks'].Cell(1,16,2,16,true).String('Oxigenação Sanguínea (% SPO2)').Style(style);
  data['Cooking Hacks'].Cell(1,17,2,17,true).String('Temperatura Corporal (°C)').Style(style);
  data['Cooking Hacks'].Cell(1,18,1,19,true).String('Suor').Style(style);
  data['Cooking Hacks'].Cell(2,18).String('Condutividade').Style(style);
  data['Cooking Hacks'].Cell(2,19).String('Resistência').Style(style);
  data['Cooking Hacks'].Cell(1,20,2,20,true).String('Fluxo de Respiração').Style(style);

  data['Caixa Externa'].Column(1).Width(12);
  data['Caixa Externa'].Column(2).Width(8);
  data['Caixa Externa'].Column(3).Width(17);
  data['Caixa Externa'].Column(4).Width(21);
  data['Caixa Externa'].Column(5).Width(13);
  data['Caixa Externa'].Column(6).Width(13);
  data['Caixa Externa'].Column(7).Width(15);
  data['Caixa Externa'].Column(8).Width(26);
  data['Caixa Externa'].Column(9).Width(17);

  data['Caixa Externa'].Cell(1,1,2,1,true).String('Data').Style(style);
  data['Caixa Externa'].Cell(1,2,2,2,true).String('Hora').Style(style);
  data['Caixa Externa'].Cell(1,3,2,3,true).String('Temperatura (°C)').Style(style);
  data['Caixa Externa'].Cell(1,4,2,4,true).String('Umidade Relativa (%)').Style(style);
  data['Caixa Externa'].Cell(1,5,2,5,true).String('Ruído (dBA)').Style(style);
  data['Caixa Externa'].Cell(1,6,2,6,true).String('Altitude (m)').Style(style);
  data['Caixa Externa'].Cell(1,7,2,7,true).String('Pressão (atm)').Style(style);
  data['Caixa Externa'].Cell(1,8,2,8,true).String('Velocidade do Vento (m/s)').Style(style);
  data['Caixa Externa'].Cell(1,9,2,9,true).String('Direção do Vento').Style(style);

  db.SensorData.findAll({
    attributes: ['macAddress', 'date', 'counter', 'sensor1', 'sensor2', 'sensor3', 'sensor4', 'sensor5', 'sensor6', 'sensor7', 'sensor8', 'sensor9', 'sensor10', 'sensor11', 'sensor12', 'sensor13'],
    where: {
      ReviewId: req.param('id')
    }
  }).success(function(entitiesData) {
    db.Case.findAll({
      attributes: [ 'id', 'macAddress' ]
    }).success(function(entitiesCase) {
      db.Sensor.findAll({
        attributes: [ 'id', 'field', 'description', 'CaseId' ]
      }).success(function(entitiesSensor) {
         db.Formula.findAll({
          where: {
            ReviewId: req.param('id')
          },
          attributes: [ 'id', 'formula', 'ReviewId', 'SensorId' ]
        }).success(function(entitiesFormula) {

          var x = null;

          for(var i = 0; i < entitiesCase.length; i++){
            for(var j = 0; j < entitiesSensor.length; j++){
              if(entitiesCase[i].id == entitiesSensor[j].CaseId){
                entitiesSensor[j].macAddress = entitiesCase[i].macAddress;
              }
            };
          };

          for(var i = 0; i < entitiesSensor.length; i++){
            for(var j = 0; j < entitiesFormula.length; j++){
              if(entitiesSensor[i].id == entitiesFormula[j].SensorId){
                entitiesSensor[i].formula = entitiesFormula[j].formula;
              }else{
                entitiesSensor[i].formula = null;
              }
            };
          };

          for(var i = 0; i < entitiesData.length; i++){
            for(var j = 0; j < entitiesSensor.length; j++){
              if(entitiesData[i].macAddress == entitiesSensor[j].macAddress){

                if(conf[entitiesSensor[j]['id']]['sheets']){

                  //Data e Hora
                  data[conf[entitiesSensor[j]['id']]['sheets']].Cell(parseInt(conf[entitiesSensor[j]['id']]['line']) + 3, 1).String(getDate(entitiesData[i].date)).Style(style);
                  data[conf[entitiesSensor[j]['id']]['sheets']].Cell(parseInt(conf[entitiesSensor[j]['id']]['line']) + 3, 2).String(getTime(entitiesData[i].date)).Style(style);

                  // Dados

                  x = parseInt(entitiesData[i]['sensor' + entitiesSensor[j]['field']], 16).toString();

                  if(entitiesSensor[j]['formula']){
                    x = eval(formula);
                  }

                  data[conf[entitiesSensor[j]['id']]['sheets']].Cell(parseInt(conf[entitiesSensor[j]['id']]['line']) + 3, conf[entitiesSensor[j]['id']]['row']).String(x).Style(style);

                  conf[entitiesSensor[j]['id']]['line'] = parseInt(conf[entitiesSensor[j]['id']]['line']) + 1;

                }
              }
            };
          };

          wb.write("Lata.xlsx",function(err){
            res.download("Lata.xlsx")
          });
        });
      });
    });
  });
};