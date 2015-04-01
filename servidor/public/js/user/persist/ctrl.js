'use strict';

define(['js/app'], function (app) {
  app.controller('userPersistCtrl', function ($scope, $http, $window) {

  function clear(){
    angular.extend($scope, {
      data: {
        name: '',
        email: '',
        password: ''
      }
    });
  };

  clear();

  $scope.persist = function(){
    $http.post('/user', $scope.data).success(function(data){
      if(data.success){
        alert(data.message);
        clear();
      }
    });
  };

  });
});
