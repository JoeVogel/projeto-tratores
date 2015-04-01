module.exports = function(sequelize, DataTypes) {
  var Equipment = sequelize.define('Equipment', {
    description: {
      type: DataTypes.STRING(60)
    },
    model: {
      type: DataTypes.STRING(40)
    },
    year: {
      type: DataTypes.INTEGER
    }
  }, {
    classMethods: {
      associate: function(models) {
        Equipment.hasMany(models.Review, { onDelete: 'cascade' });
      }
    }
  });
  return Equipment;
};