module.exports = function(sequelize, DataTypes) {
  var Sensor = sequelize.define('Sensor', {
    field: {
      type: DataTypes.STRING(20)
    },
    type: {
      type: DataTypes.STRING(40)
    },
    description: {
      type: DataTypes.STRING(60)
    }
  }, {
    classMethods: {
      associate: function(models) {
        Sensor.belongsTo(models.Case);
        Sensor.hasMany(models.Formula, { onDelete: 'cascade' });
      }
    }
  });
  return Sensor;
};
