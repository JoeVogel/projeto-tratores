'use strict';

angular.module('app', []);

angular.module('app').controller('indexCtrl', ['$scope', '$http', '$window', function($scope, $http, $window){

  angular.extend($scope, {
    data: {
      username: 'projeto-trator',
      password: 'dJhk2YO3'
    }
  });

  $scope.login = function(){
    $http.post('/login', $scope.data).success(function(data){
      if(data.success){
        $window.location = '/home';
      }else{
        alert('Email ou senha inválidos!');
      }
    }).error(function(error){
      alert('Erro ao fazer login!');
    });
  };

}]);