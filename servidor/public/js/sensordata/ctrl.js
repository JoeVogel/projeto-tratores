'use strict';

define(['js/app', 'angularChart'], function (app, angularChart) {
  app.controller('sensordataCtrl', function ($scope, $http) {

    function getQuery(obj){
      var response = '?';
      response = response + 'review=' + obj.review;
      response = response + '&sensor=' + obj.sensor;
      return response;
    };

    var Chart = function(data){
      return {
        "sensor": data.sensor,
        "type": "LineChart",
        "displayed": true,
        "data": data.data,
        "options": {
          "chartArea":{
            "left": "50",
            "top": "10",
            "width": "95%",
            "height": "82%"
          },
          "legend": {
            "position": "none"
          },
          "displayExactValues": true,
          "vAxis": {
            "gridlines": {
              "count": 10
            }
          }
        }
      }
    };

    angular.extend($scope, {
      equipment: [],
      review: [],
      panels: [],
      sensor: [],
      data: {
        equipment: 0,
        review: 0,
        sensor: 0
      }
    });

    $http.get('/data-view-sensordata').success(function(data){
      $scope.equipment = data.data.equipment;
      $scope.sensor = data.data.sensor;
    });

    $scope.findReview = function(equipment){
      $scope.data.review = 0;
      if(equipment != 0){
        $http.get('/data-view-sensordata-review/' + equipment).success(function(data){
          $scope.review = data.data;
        });
      };
    };

    $scope.findSensor = function(review){
      $scope.data.sensor = 0;
    };

    $scope.closeSensorData = function(index){
      $scope.panels.splice(index, 1);
    };

    $scope.setChar = function(index, options){
      $scope.panels[index].type = options;
    };

    $scope.findData = function(){
      $http.get('/sensordata' + getQuery($scope.data)).success(function(data){
        $scope.panels.push(new Chart(data.data));
      });
    };

  });
});
