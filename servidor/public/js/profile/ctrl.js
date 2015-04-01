'use strict';

define(['js/app'], function (app) {
  app.controller('profileCtrl', function ($scope, $http) {

    angular.extend($scope, {
      data: {}
    });

    $http.get('/user').success(function(data) {
      angular.extend($scope, {
        data: data
      });
    });

  });
});
