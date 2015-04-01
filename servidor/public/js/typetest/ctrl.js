'use strict';

define(['js/app'], function (app) {
  app.controller('typetestCtrl', function ($scope, $http) {

    angular.extend($scope, {
      testtype: []
    });

    $http.get('/testtype').success(function(data){
      angular.extend($scope, {
        testtype: data.data
      });
    });

    $scope.delete = function(id, index){
      $http.delete('/testtype/' + id).success(function(data){
        if(data.success == 1){
          alert(data.message);
          $scope.testtype.splice(index, 1);
        }
      });
    };

  });
});
