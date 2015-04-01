'use strict';

define(['js/app'], function (app) {
  app.controller('sensorCtrl', function ($scope, $http) {

    angular.extend($scope, {
      sensor: []
    });

    $http.get('/sensor').success(function(data){
      angular.extend($scope, {
        sensor: data.data
      });
    });

    $scope.delete = function(id, index){
      $http.delete('/sensor/' + id).success(function(data){
        if(data.success == 1){
          alert(data.message);
          $scope.sensor.splice(index, 1);
        }
      });
    };

  });
});
