'use strict';

define(['js/app'], function (app) {
  app.controller('caseCtrl', function ($scope, $http) {

    angular.extend($scope, {
      case: []
    });

    $http.get('/case').success(function(data){
      angular.extend($scope, {
        case: data.data
      });
    });

    $scope.delete = function(id, index){
      $http.delete('/case/' + id).success(function(data){
        if(data.success == 1){
          alert(data.message);
          $scope.case.splice(index, 1);
        }
      });
    };

  });
});
