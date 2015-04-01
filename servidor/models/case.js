module.exports = function(sequelize, DataTypes) {
  var Case = sequelize.define('Case', {
    macAddress: {
      type: DataTypes.STRING(20),
      unique: true
    }
  }, {
    classMethods: {
      associate: function(models) {
        Case.hasMany(models.Sensor, { onDelete: 'cascade' });
      }
    }
  });
  return Case;
};
