'use strict';

define(['js/app'], function (app) {
  app.controller('reviewPersistCtrl', function ($scope, $http, $window) {

      var io = null;

     angular.extend($scope, {
      disabled: false,
      sensor: [],
      equipment: [],
    });

    function clear(){
      angular.extend($scope, {
        data: {
          description: '',
          formulas: false,
          testtype: 0,
          equipment: 0,
          TestTypeId: 0,
          EquipmentId: 0
        }
      });
    };

    clear();

    $http.get('/data-view-review').success(function(data){
      angular.extend($scope, {
        testtype: data.data.testtype,
        equipment: data.data.equipment
      });
    });

    $scope.persist = function(){
      angular.extend($scope.data, {
        TestTypeId: $scope.testtype[$scope.data.testtype].id,
        EquipmentId: $scope.equipment[$scope.data.equipment].id
      });
      $http.post('/review', $scope.data).success(function(data){
        if(data.success == 1){
          alert(data.message);
          $window.location = '#/review-real-time';
        }
      });
    };

  });
});
