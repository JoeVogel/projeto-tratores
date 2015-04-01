'use strict';

define(['js/app'], function (app) {
  app.controller('formulaPersistCtrl', function ($scope, $http, $window) {

    angular.extend($scope, {
      search: true,
      disabled: false,
      sensor: [],
      review: [],
      data: {
        formula: '',
        review: 0,
        SensorId: 0,
        ReviewId: 0
      }
    });

    $http.get('/data-view-formula').success(function(data){
      angular.extend($scope, {
        sensor: data.data.sensor,
        review: data.data.review
      });
      $scope.findFormulas(data.data.review[0].id);
    });

    $scope.findFormulas = function(idReview){
      $scope.search = true;
      $http.get('/formula-review/' + idReview).success(function(data){
        for(var i = 0; i < $scope.sensor.length; i++){
          $scope.sensor[i].idFormula = undefined;
          $scope.sensor[i].formula = '';
          $scope.sensor[i].status = false;
          for(var j = 0; j < data.data.length; j++){
            if(data.data[j].SensorId == $scope.sensor[i].id){
              $scope.sensor[i].idFormula = data.data[j].id;
              $scope.sensor[i].formula = data.data[j].formula;
            }
          }
        };
        $scope.search = false;
      });
    };

    $scope.persist = function(idSensor, idFormula, formula, index){
      angular.extend($scope.data, {
        id: idFormula,
        SensorId: idSensor,
        ReviewId: $scope.review[$scope.data.review].id,
        formula: formula
      });
      $http.post('/formula', $scope.data).success(function(data){
        if(data.success == 1){
          $scope.sensor[index].status = false;
        }
      });
    };

  });
});
