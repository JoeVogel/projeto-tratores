module.exports = function(sequelize, DataTypes) {
  var Formula = sequelize.define('Formula', {
    formula: {
      type: DataTypes.TEXT
    }
  }, {
    classMethods: {
      associate: function(models) {
        Formula.belongsTo(models.Review)
        Formula.belongsTo(models.Sensor)
      }
    }
  });
  return Formula;
};
