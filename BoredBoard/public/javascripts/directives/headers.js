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
		scope: {open: '='},
		templateUrl: 'javascripts/directives/filterHeader.html',
		link: function(scope){
			scope.showPopup = function(){
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
