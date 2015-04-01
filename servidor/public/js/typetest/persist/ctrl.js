'use strict';

define(['js/app'], function (app) {
  app.controller('typetestPersistCtrl', function ($scope, $http, $routeParams, $window) {

    function clear(){
      angular.extend($scope, {
        data: {
          type: '',
          description: '',
          terrain: '',
          weather: '',
          duration: ''
        }
      });
    };

    clear();

    if($routeParams.id){
      $http.get('/testtype/' + $routeParams.id).success(function(data){
        angular.extend($scope, {
          data: data.data
        });
      });
    };

    $scope.persist = function(){
      $http.post('/testtype', $scope.data).success(function(data){
        if(data.success == 1){
          alert(data.message);
          if($routeParams.id){
            $window.location = '#/typetest';
          }else{
            clear();
          }
        }
      });
    };

  });
});
