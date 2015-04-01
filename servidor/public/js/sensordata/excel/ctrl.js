'use strict';

define(['js/app'], function (app) {
  app.controller('sensordataExcelCtrl', function ($scope, $http, $timeout) {

    angular.extend($scope, {
      equipment: [],
      review: [],
      data: {
        equipment: 0,
        review: 0
      }
    });

    $http.get('/data-view-sensordata-excel').success(function(data){
      $scope.equipment = data.data.equipment;
    });

    $scope.findReview = function(equipment){
      $scope.data.review = 0;
      if(equipment != 0){
        $http.get('/data-view-sensordata-review/' + equipment).success(function(data){
          $scope.review = data.data;
        });
      };
    };

  });
});
