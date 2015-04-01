'use strict';

require.config({
  baseUrl: "",
  paths: {
    'angular': 'lib/angular/angular.min',
    'angular-route': 'lib/angular-route/angular-route.min',
    'angularAMD': 'lib/angular-amd/angularAMD.min',
    'angular-sanitize': 'lib/angular-sanitize/angular-sanitize.min',
    'angularChart': 'lib/angular-chart/angular-chart.min',
    'jquery': 'lib/jquery/jquery.min',
    'bootstrap': 'lib/bootstrap/js/bootstrap.min',
    'socket': '/socket.io/socket.io'
  },
  shim: {
    'angularAMD': ['angular'],
    'angular-route': ['angular'],
    'angular-sanitize': ['angular'],
    'angularChart': ['angular'],
    'bootstrap': ['jquery']
  },
  deps: ['js/app', 'js/socket', 'angular-sanitize', 'jquery', 'bootstrap']
});