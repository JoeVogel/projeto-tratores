var db = require('../models');

function isValid(caixa){
  if(caixa.macAddress == ''){
    return false;
  }
  return true;
};

exports.find = function(req, res, next) {
  db.Case.find({
    where: {
      id: req.param('id')
    },
    attributes: ['id', 'macAddress']
  }).success(function(entity) {
    res.json({ success: 1, data: entity });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.findAll = function(req, res, next) {
  db.Case.findAll({
    attributes: ['id', 'macAddress']
  }).success(function(entities) {
    res.json({ success: 1, data: entities });
  }).error(function(error){
    res.json({ success: 2, message: error });
  });
};

exports.persist = function(req, res, next) {
  db.Case.find({ where: { id: req.body.id } }).success(function(entity) {
    if (entity) {
      entity.updateAttributes(req.body).success(function(entity) {
        res.json({ success: 1, message: "Caixa atualizada com sucesso!"});
      }).error(function(error){
        res.json({ success: 0, message: "Já existe uma caixa com esses dados!"});
      });
    } else {
      if(isValid(req.body)){
        db.Case.create(req.body).success(function(entity) {
          res.json({ success: 1, message: "Caixa criada com sucesso!"});
        }).error(function(error){
          res.json({ success: 0, message: "Já existe uma caixa com esses dados!"});
        });
      }else{
        res.json({ success: 0, message: "Favor preencher todos os campos!"});
      }
    }
  })
};

exports.delete = function(req, res, next) {
  db.Case.find({ where: { id: req.param('id') } }).success(function(entity) {
    if (entity) {
      entity.destroy().success(function() {
        res.json({ success: 1, message: "Caixa excluída!"});
      });
    } else {
      res.json({ success: 0, message: "Caixa não encontrada!"});
    }
  });
};