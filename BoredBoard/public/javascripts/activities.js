$( document ).ready(function() {
  $('#flip5').flip();
  $('#filters').click(function(){
	console.log("filters clicked");
	window.open('/filters.html','formPrompt','width=600, height=400');
  }); 
});
