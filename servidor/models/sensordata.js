var db = require('../models');

module.exports = function(sequelize, DataTypes) {
  var SensorData = sequelize.define('SensorData', {
    macAddress: {
      type: DataTypes.STRING(20),
      unique: 'compositeIndex'
    },
    date: {
      type: DataTypes.BIGINT,
      unique: 'compositeIndex'
    },
    counter: {
      type: DataTypes.BIGINT,
      unique: 'compositeIndex'
    },
    sensor1: {
      type: DataTypes.STRING(20)
    },
    sensor2: {
      type: DataTypes.STRING(20)
    },
    sensor3: {
      type: DataTypes.STRING(20)
    },
    sensor4: {
      type: DataTypes.STRING(20)
    },
    sensor5: {
      type: DataTypes.STRING(20)
    },
    sensor6: {
      type: DataTypes.STRING(20)
    },
    sensor7: {
      type: DataTypes.STRING(20)
    },
    sensor8: {
      type: DataTypes.STRING(20)
    },
    sensor9: {
      type: DataTypes.STRING(20)
    },
    sensor10: {
      type: DataTypes.STRING(20)
    },
    sensor11: {
      type: DataTypes.STRING(20)
    },
    sensor12: {
      type: DataTypes.STRING(20)
    },
    sensor13: {
      type: DataTypes.STRING(20)
    }
  }, {
    classMethods: {
      associate: function(models) {
        SensorData.belongsTo(models.Review);
      }
    }
  });
  return SensorData;
};
