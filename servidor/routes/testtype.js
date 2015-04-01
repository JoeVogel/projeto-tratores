var db = require('../models');

function isValid(testtype){
  if(testtype.description == ''){
    return false;
  }
  if(testtype.type == ''){
    return false;
  }
  if(testtype.terrain == ''){
    return false;
  }
  if(testtype.weather == ''){
    return false;
  }
  if(testtype.duration == ''){
    return false;
  }
  return true;
};

exports.find = function(req, res, next) {
  db.TestType.find({
    where: {
      id: req.param('id')
    },
    attributes: ['id', 'type', 'description', 'terrain', 'weather', 'duration']
  }).success(function(entity) {
    res.json({ success: 1, data: entity });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.findAll = function(req, res, next) {
  db.TestType.findAll({
    attributes: ['id', 'type', 'description', 'terrain', 'weather', 'duration']
  }).success(function(entities) {
    res.json({ success: 1, data: entities });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.persist = function(req, res, next) {
  db.TestType.find({ where: { id: req.body.id } }).success(function(entity) {
    if (entity) {
      entity.updateAttributes(req.body).success(function(entity) {
        res.json({ success: 1, message: "Tipo de teste atualizado com sucesso!"});
      }).error(function(error){
        res.json({ success: 0, message: "Já existe um tipo de teste com esses dados!"});
      });
    } else {
      if(isValid(req.body)){
        db.TestType.create(req.body).success(function(entity) {
          res.json({ success: 1, message: "Tipo de teste criado com sucesso!"});
        }).error(function(error){
          res.json({ success: 0, message: "Já existe um tipo de teste com esses dados!"});
        });
      }else{
        res.json({ success: 0, message: "Favor preencher todos os campos!"});
      }
    }
  })
};

exports.delete = function(req, res, next) {
  db.TestType.find({ where: { id: req.param('id') } }).success(function(entity) {
    if (entity) {
      entity.destroy().success(function() {
        res.json({ success: 1, message: "Tipo de teste excluído!"});
      })
    } else {
      res.json({ success: 0, message: "Tipo de teste não encontrado!"});
    }
  });
};