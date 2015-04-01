'use strict';

define(['js/app', 'angularChart'], function (app, angularChart) {
  app.controller('reviewRealTimeCtrl', function ($scope, $http, $window, $timeout) {

    socket.removeListener("_status");
    socket.removeListener("_processing");
    socket.removeListener("_finish");
    socket.removeListener("_findData");
    socket.removeListener("_isClient");

    socket.on('_status', function (status) {
      if(status == 1){
        $(".modal").modal('hide');
        if (!$scope.$$phase) {
          $scope.$digest();
        }
      }
    });

    socket.on('_processing', function () {
      refresh();
    });

    socket.on('_finish', function () {
      finish();
    });

    socket.on('_findData', function (obj) {
      for(var p in $scope.panels){
        if($scope.panels[p].sensor.id == obj.sensor.id){
          $scope.panels[p].data = obj.data;
          if (!$scope.$$phase) {
            $scope.$digest();
          }
        }
      }
    });

    socket.on('_isClient', function (status) {
      if(status){
        alert("Equipamento está conectado!");
      }else{
        alert("Equipamento não está conectado!");
      }
    });

    function getQuery(sensor, review){
      var response = '?';
      response = response + 'review=' + review;
      response = response + '&sensor=' + sensor;
      return response;
    };

    function refresh(){
      for(var p in $scope.panels){
        socket.emit('findData', { sensor: $scope.panels[p].sensor.id, review: $scope.data.id });
      }
    };

    function finish(){
      $http.post('/review-2/' + $scope.data.id).success(function(data){
        $window.location = "#/sensordata";
      });
    };

    var Chart = function(data){
      return {
        "sensor": data.sensor,
        "type": "LineChart",
        "displayed": true,
        "data": data.data,
        "options": {
          "chartArea":{
            "left": "50",
            "top": "10",
            "width": "95%",
            "height": "82%"
          },
          "legend": {
            "position": "none"
          },
          "displayExactValues": true,
          "vAxis": {
            "gridlines": {
              "count": 10
            }
          }
        }
      }
    };

    angular.extend($scope, {
      messageTitle: 'Iniciando análise',
      message: 'Atualizando análise...',
      panels: [],
      sensors: [],
      sensor: 0,
      data: {}
    });

    $http.get('/review-real-time').success(function(data){
      if(data.data.review){
        angular.extend($scope, {
          data: data.data.review,
          sensors: data.data.sensor
        });
      }else{
        alert(data.message);
        $window.location = '#/review-persist';
      }
    });

    $scope.setChar = function(index, options){
      $scope.panels[index].type = options;
    };

    $scope.cancel = function(){
      $http.post('/review-3/' + $scope.data.id).success(function(data){
        if(data.success == 1){
          alert(data.message);
          $window.location = '#/review-persist';
        }
      });
    };

    $scope.persist = function(){
      $(".modal").modal({ backdrop: 'static' });
      $http.post('/review-1/' + $scope.data.id).success(function(data){
        if(data.success == 1){
          $scope.data.status = 1;
          $scope.message = "Procurando equipamento...";
          socket.emit('status', $scope.data.id);
        }
      });
    };

    $scope.close = function(){
      finish();
    };

    $scope.closeSensorData = function(index){
      $scope.panels.splice(index, 1);
    };

    $scope.findData = function(review){
      $http.get('/sensordata' + getQuery($scope.sensor, review)).success(function(data){
        if(data.success){
          $scope.panels.push(new Chart(data.data));
          if (!$scope.$$phase) {
            $scope.$digest();
          }
        }
      });
    };

    $scope.findEquipment = function(){
      socket.emit('isClient');
    };

  });
});
