app.directive('generalHeader',
function(){
	return{
		restrict: 'E',
		scope: {},
		templateUrl: 'javascripts/directives/generalHeader.html'
	};
});

app.directive('filterHeader',
function(){
	return{
		restrict: 'E',
		scope: {open: '=', mylat: '='},
		templateUrl: 'javascripts/directives/filterHeader.html',
		link: function(scope){
			/*$scope.$watch('myLat', function(){
				var worthless = $scope.myLat
				alert('myLat changed');
				
			});*/
			scope.showPopup = function(){
				if(!scope.mylat){
					alert("we can't filter activities to your area until you allow us to use your location.");
					return;
				}
				scope.open = !scope.open;
				//alert('in the direcctives showPopup function');	
			}
		}
		//controller: function($scope){
			//alert($scope.open);
		//}
		//controller: 'MainCtrl'
	};
}); 
