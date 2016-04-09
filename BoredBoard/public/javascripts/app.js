angular.module('activity', [])
.controller('MainCtrl', [
  '$scope','$http','$sce',
  function($scope,$http,$sce){
    $scope.activities = [];
    $scope.getAll = function() {
      return $http.get('/submit-activity').success(function(data){
        angular.copy(data, $scope.activities);
      });
    };
    $scope.getAll();
    var generalHeader = '<a href="/"><img src="/images/BB.png" id="bb-symbol" class="pull-left" alt="bored board symbol"></a>  <ul class="pull-left"><li class="btn-link btn-lg">login</li><li class="btn-link btn-lg">sign up</li></ul><ul class="pull-right"><a href="/activities.html"><li class="btn-link btn-lg">activities</li></a><a href="/events.html"><li class="btn-link btn-lg">events</li></a></ul>'
    $scope.header = $sce.trustAsHtml(generalHeader);
  }
]);
