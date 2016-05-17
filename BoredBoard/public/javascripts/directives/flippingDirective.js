app.directive('flippingDirective', function() {
  return function(scope, element, attrs) { 
    scope.$watch('$last',function(v){
      if (v){
	 $('.flipper').flip();
      }  
    });
    
  };
})
