var db = require('../models');

function isValid(sensor){
  if(sensor.field == ''){
    return false;
  }
  if(sensor.CaseId == ''){
    return false;
  }
  if(sensor.description == ''){
    return false;
  }
  return true;
};

exports.dataView = function(req, res, next) {
  var _return = {};
  db.Case.findAll({
    attributes: ['id', 'macAddress']
  }).success(function(entities) {
    _return.cases = entities;
    res.json({ success: 1, data: _return });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.find = function(req, res, next) {
  db.Sensor.find({
    where: {
      id: req.param('id')
    },
    attributes: ['id', 'field', 'description'],
    include: [ {
      model: db.Case,
      attributes: ['id', 'macAddress']
    }],
  }).success(function(entity) {
    res.json({ success: 1, data: entity });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.findAll = function(req, res, next) {
  db.Sensor.findAll({
    attributes: ['id', 'field', 'description'],
    include: [ {
      model: db.Case,
      attributes: ['id', 'macAddress']
    }],
  }).success(function(entities) {
    res.json({ success: 1, data: entities });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.persist = function(req, res, next) {
  db.Sensor.find({ where: { id: req.body.id } }).success(function(entity) {
    if (entity) {
      entity.updateAttributes(req.body).success(function(entity) {
        res.json({ success: 1, message: "Sensor atualizado com sucesso!"});
      }).error(function(error){
        res.json({ success: 0, message: "Já existe um sensor com esses dados!"});
      });
    } else {
      if(isValid(req.body)){
        db.Sensor.count({
          where: {
            CaseId: req.body.CaseId,
            field: req.body.field
          },
        }).success(function(count) {
          if(count > 0){
            res.json({ success: 0, message: "Já existe um sensor nesse campo e caixa!"});
          }else{
            req.body.type = "HEX";
            db.Sensor.create(req.body).success(function(entity) {
              res.json({ success: 1, message: "Sensor criado com sucesso!"});
            }).error(function(error){
              res.json({ success: 0, message: "Já existe um sensor com esses dados!"});
            });
          }
        }).error(function(error){
          res.json({ success: 0, message: "Já existe um sensor nesse campo e caixa!"});
        });
      }else{
        res.json({ success: 0, message: "Favor preencher todos os campos!"});
      }
    }
  })
};

exports.delete = function(req, res, next) {
  db.Sensor.find({ where: { id: req.param('id') } }).success(function(entity) {
    if (entity) {
      entity.destroy().success(function() {
        res.json({ success: 1, message: "Sensor excluído!"});
      });
    } else {
      res.json({ success: 0, message: "Sensor não encontrado!"});
    }
  });
};