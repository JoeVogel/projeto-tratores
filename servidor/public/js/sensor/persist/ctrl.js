'use strict';

define(['js/app'], function (app) {
  app.controller('sensorPersistCtrl', function ($scope, $http, $routeParams, $window) {

    angular.extend($scope, {
      cases: [],
      data: {
        field: 1,
        description: ''
      }
    });

    $http.get('/data-view-sensor').success(function(data){
      if(data.success == 1){
        angular.extend($scope, {
          cases: data.data.cases
        });
        $scope.data.CaseId = data.data.cases[0].id;
      }
    });

    if($routeParams.id){
      $http.get('/sensor/' + $routeParams.id).success(function(data){
        angular.extend($scope, {
          data: data.data
        });
        $scope.data.CaseId = data.data.Case.id;
      });
    };

    $scope.persist = function(){
      $http.post('/sensor', $scope.data).success(function(data){
        if(data.success == 1){
          alert(data.message);
          if($routeParams.id){
            $window.location = '#/sensor';
          }else{
            angular.extend($scope.data, {
              field: 1,
              description: ''
            });
          }
        }
      });
    };

  });
});
