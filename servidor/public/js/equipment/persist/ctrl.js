'use strict';

define(['js/app'], function (app) {
  app.controller('equipmentPersistCtrl', function ($scope, $http, $routeParams, $window) {

    function clear(){
      angular.extend($scope, {
        data: {
          description: '',
          model: '',
          year: 2015
        }
      });
    };

    clear();

    if($routeParams.id){
      $http.get('/equipment/' + $routeParams.id).success(function(data){
        angular.extend($scope, {
          data: data.data
        });
      });
    };

    $scope.persist = function(){
      $http.post('/equipment', $scope.data).success(function(data){
        if(data.success == 1){
          alert(data.message);
          if($routeParams.id){
            $window.location = '#/equipment';
          }else{
            clear();
          }
        }
      });
    };

  });
});
