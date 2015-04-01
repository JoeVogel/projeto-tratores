'use strict';

define(['angularAMD', 'angular-route', 'angular-sanitize'], function(angularAMD) {

  var app = angular.module("app", ['ngRoute', 'ngSanitize']);

  app.config(function ($routeProvider, $locationProvider, $httpProvider) {

    $routeProvider.when("/home", angularAMD.route({
      templateUrl: '/views/home!' + Math.random(),
      controller: 'homeCtrl',
      controllerUrl: 'js/home/ctrl'
    })).when("/profile", angularAMD.route({
      templateUrl: '/views/profile!' + Math.random(),
      controller: 'profileCtrl',
      controllerUrl: 'js/profile/ctrl'
    })).when("/typetest", angularAMD.route({
      templateUrl: '/views/typetest!' + Math.random(),
      controller: 'typetestCtrl',
      controllerUrl: 'js/typetest/ctrl'
    })).when("/typetest-persist", angularAMD.route({
      templateUrl: '/views/persist/typetest!' + Math.random(),
      controller: 'typetestPersistCtrl',
      controllerUrl: 'js/typetest/persist/ctrl'
    })).when("/typetest-persist/:id", angularAMD.route({
      templateUrl: '/views/persist/typetest!' + Math.random(),
      controller: 'typetestPersistCtrl',
      controllerUrl: 'js/typetest/persist/ctrl'
    })).when("/equipment", angularAMD.route({
      templateUrl: '/views/equipment!' + Math.random(),
      controller: 'equipmentCtrl',
      controllerUrl: 'js/equipment/ctrl'
    })).when("/equipment-persist", angularAMD.route({
      templateUrl: '/views/persist/equipment!' + Math.random(),
      controller: 'equipmentPersistCtrl',
      controllerUrl: 'js/equipment/persist/ctrl'
    })).when("/equipment-persist/:id", angularAMD.route({
      templateUrl: '/views/persist/equipment!' + Math.random(),
      controller: 'equipmentPersistCtrl',
      controllerUrl: 'js/equipment/persist/ctrl'
    })).when("/sensor", angularAMD.route({
      templateUrl: '/views/sensor!' + Math.random(),
      controller: 'sensorCtrl',
      controllerUrl: 'js/sensor/ctrl'
    })).when("/sensor-persist", angularAMD.route({
      templateUrl: '/views/persist/sensor!' + Math.random(),
      controller: 'sensorPersistCtrl',
      controllerUrl: 'js/sensor/persist/ctrl'
    })).when("/sensor-persist/:id", angularAMD.route({
      templateUrl: '/views/persist/sensor!' + Math.random(),
      controller: 'sensorPersistCtrl',
      controllerUrl: 'js/sensor/persist/ctrl'
    })).when("/case", angularAMD.route({
      templateUrl: '/views/case!' + Math.random(),
      controller: 'caseCtrl',
      controllerUrl: 'js/case/ctrl'
    })).when("/case-persist", angularAMD.route({
      templateUrl: '/views/persist/case!' + Math.random(),
      controller: 'casePersistCtrl',
      controllerUrl: 'js/case/persist/ctrl'
    })).when("/case-persist/:id", angularAMD.route({
      templateUrl: '/views/persist/case!' + Math.random(),
      controller: 'casePersistCtrl',
      controllerUrl: 'js/case/persist/ctrl'
    })).when("/formula", angularAMD.route({
      templateUrl: '/views/formula!' + Math.random(),
      controller: 'formulaCtrl',
      controllerUrl: 'js/formula/ctrl'
    })).when("/formula-persist", angularAMD.route({
      templateUrl: '/views/persist/formula!' + Math.random(),
      controller: 'formulaPersistCtrl',
      controllerUrl: 'js/formula/persist/ctrl'
    })).when("/formula-persist/:id", angularAMD.route({
      templateUrl: '/views/persist/formula!' + Math.random(),
      controller: 'formulaPersistCtrl',
      controllerUrl: 'js/formula/persist/ctrl'
    })).when("/user-persist", angularAMD.route({
      templateUrl: '/views/persist/user!' + Math.random(),
      controller: 'userPersistCtrl',
      controllerUrl: 'js/user/persist/ctrl'
    })).when("/review-persist", angularAMD.route({
      templateUrl: '/views/persist/review!' + Math.random(),
      controller: 'reviewPersistCtrl',
      controllerUrl: 'js/review/persist/ctrl'
    })).when("/sensordata", angularAMD.route({
      templateUrl: '/views/sensordata!' + Math.random(),
      controller: 'sensordataCtrl',
      controllerUrl: 'js/sensordata/ctrl'
    })).when("/sensordata-excel", angularAMD.route({
      templateUrl: '/views/excel/sensordata!' + Math.random(),
      controller: 'sensordataExcelCtrl',
      controllerUrl: 'js/sensordata/excel/ctrl'
    })).when("/review-real-time", angularAMD.route({
      templateUrl: '/views/realtime/review!' + Math.random(),
      controller: 'reviewRealTimeCtrl',
      controllerUrl: 'js/review/realtime/ctrl'
    })).otherwise({ redirectTo: "/home" });

    var interceptor = function($window){
      function success(response){
        if(response.data.success == 2){
          error(response);
        }
        if(response.data.success == 0){
          info(response);
        }
        return response;
      };
      function info(response) {
        alert(response.data.message);
      };
      function error(response) {
        alert(response.data.message || 'Falha na autenticação!');
        $window.location = '/';
      };
      return function(promise) {
        return promise.then(success, error);
      }
    }
    $httpProvider.responseInterceptors.push(interceptor);
  });

  app.run(function($rootScope, $templateCache) {
    $rootScope.$on('$viewContentLoaded', function() {
      $templateCache.removeAll();
    });
  });

  return angularAMD.bootstrap(app);
});