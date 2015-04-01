'use strict';

define(['js/app'], function (app) {
  app.controller('casePersistCtrl', function ($scope, $http, $routeParams, $window) {

    function clear(){
      angular.extend($scope, {
        data: {
          macAddress: ''
        }
      });
    };

    clear();

    if($routeParams.id){
      $http.get('/case/' + $routeParams.id).success(function(data){
        angular.extend($scope, {
          data: data.data
        });
      });
    };

    $scope.persist = function(){
      $http.post('/case', $scope.data).success(function(data){
        if(data.success == 1){
          alert(data.message);
          if($routeParams.id){
            $window.location = '#/case';
          }else{
            clear();
          }
        }
      });
    };

  });
});
