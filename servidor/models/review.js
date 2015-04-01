module.exports = function(sequelize, DataTypes) {
  var Review = sequelize.define('Review', {
    status: {
      // 0 = Criada
      // 1 = Andamento
      // 2 = Terminada
      // 3 = Cancelada
      type: DataTypes.INTEGER
    },
    description: {
      type: DataTypes.TEXT
    }
  }, {
    classMethods: {
      associate: function(models) {
        Review.belongsTo(models.User);
        Review.belongsTo(models.Equipment);
        Review.belongsTo(models.TestType);
        Review.hasMany(models.SensorData, { onDelete: 'cascade' });
        Review.hasMany(models.Formula, { onDelete: 'cascade' });
      }
    }
  });
  return Review;
};
