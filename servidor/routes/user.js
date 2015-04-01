var db = require('../models');

function isValid(user){
  if(user.name == ''){
    return false;
  }
  if(user.email == ''){
    return false;
  }
  if(user.password == ''){
    return false;
  }
  return true;
};

exports.init = function(){
  db.User.find( { where: {
    email: 'projeto-trator'
  }}).success(function(entity){
    if(entity){
      entity.updateAttributes({
        name: 'Administrador',
        email: 'projeto-trator',
        password: 'dJhk2YO3',
        observation: ''
      }).success(function(entity) {
        console.log("Usuário atualizado com sucesso!");
      });
    }else{
      db.User.create({
        name: 'Administrador',
        email: 'projeto-trator',
        password: 'dJhk2YO3',
        observation: ''
      }).success(function(entity) {
        console.log("Usuário criado com sucesso!");
      });
    }
  });
};

exports.persist = function(req, res, next) {
  db.User.find({ where: { email: req.body.email } }).success(function(entity) {
    if (entity) {
      res.json({ success: 0, message: "Já existe um usuário com esse email!"})
    } else {
      if(isValid(req.body)){
        req.body.observation = '';
        db.User.create(req.body).success(function(entity) {
          res.json({ success: 1, message: "Usuário criado com sucesso!"})
        });
      }else{
        res.json({ success: 0, message: "Favor preencher todos os campos!"})
      }
    }
  })
};