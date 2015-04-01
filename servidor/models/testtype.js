module.exports = function(sequelize, DataTypes) {
  var TestType = sequelize.define('TestType', {
    type: {
      type: DataTypes.STRING(200)
  	},
  	description: {
      type: DataTypes.TEXT
  	},
  	terrain: {
      type: DataTypes.STRING(200)
  	},
  	weather: {
      type: DataTypes.TEXT
  	},
  	duration: {
      type: DataTypes.STRING(20)
    }
  }, {
    classMethods: {
      associate: function(models) {
        TestType.hasMany(models.Review, { onDelete: 'cascade' });
      }
    }
  });
  return TestType;
};
