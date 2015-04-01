'use strict';

define(['js/app'], function (app) {
  app.controller('equipmentCtrl', function ($scope, $http) {

    angular.extend($scope, {
      equipment: []
    });

    $http.get('/equipment').success(function(data){
      angular.extend($scope, {
        equipment: data.data
      });
    });

    $scope.delete = function(id, index){
      $http.delete('/equipment/' + id).success(function(data){
        if(data.success == 1){
          alert(data.message);
          $scope.equipment.splice(index, 1);
        }
      });
    };

  });
});
