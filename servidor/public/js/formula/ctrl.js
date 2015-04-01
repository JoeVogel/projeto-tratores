'use strict';

define(['js/app'], function (app) {
  app.controller('formulaCtrl', function ($scope, $http) {

    angular.extend($scope, {
      sensorequipment: []
    });

    $http.get('/formula').success(function(data){
      angular.extend($scope, {
        sensorequipment: data.data
      });
    });

    $scope.delete = function(id, index){
      $http.delete('/formula/' + id).success(function(data){
        if(data.success == 1){
          alert(data.message);
          $scope.sensorequipment.splice(index, 1);
        }
      });
    };

  });
});
