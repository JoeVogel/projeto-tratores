var passwordHash = require('password-hash');

module.exports = function(sequelize, DataTypes) {
  var User = sequelize.define('User', {
    name: {
      type: DataTypes.STRING(60)
    },
    email: {
      type: DataTypes.STRING(100)
    },
    password: {
      type: DataTypes.TEXT,
      set: function(password) {
        this.setDataValue('password', passwordHash.generate(password));
      }
    },
    observation: {
      type: DataTypes.TEXT
    }
  }, {
    classMethods: {
      associate: function(models) {
        User.hasMany(models.Review, { onDelete: 'cascade' })
      }
    }
  });
  return User;
};
