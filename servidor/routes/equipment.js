var db = require('../models');

function isValid(equipment){
  if(equipment.description == ''){
    return false;
  }
  if(equipment.model == ''){
    return false;
  }
  if(equipment.year == ''){
    return false;
  }
  return true;
};

exports.find = function(req, res, next) {
  db.Equipment.find({
    where: {
      id: req.param('id')
    },
    attributes: ['id', 'description', 'model', 'year']
  }).success(function(entity) {
    res.json({ success: 1, data: entity });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.findAll = function(req, res, next) {
  db.Equipment.findAll({
    attributes: ['id', 'description', 'model', 'year']
  }).success(function(entities) {
    res.json({ success: 1, data: entities });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.persist = function(req, res, next) {
  db.Equipment.find({ where: { id: req.body.id } }).success(function(entity) {
    if (entity) {
      entity.updateAttributes(req.body).success(function(entity) {
        res.json({ success: 1, message: "Equipamento atualizado com sucesso!"});
      }).error(function(error){
        res.json({ success: 0, message: "Já existe um equipamento com esses dados!"});
      });
    } else {
      if(isValid(req.body)){
        db.Equipment.create(req.body).success(function(entity) {
          res.json({ success: 1, message: "Equipamento criado com sucesso!"});
        }).error(function(error){
          res.json({ success: 0, message: "Já existe um equipamento com esses dados!"});
        });
      }else{
        res.json({ success: 0, message: "Favor preencher todos os campos!"});
      }
    }
  })
};

exports.delete = function(req, res, next) {
  db.Equipment.find({ where: { id: req.param('id') } }).success(function(entity) {
    if (entity) {
      entity.destroy().success(function() {
        res.json({ success: 1, message: "Equipamento excluído!"});
      });
    } else {
      res.json({ success: 0, message: "Equipamento não encontrado!"});
    }
  });
};