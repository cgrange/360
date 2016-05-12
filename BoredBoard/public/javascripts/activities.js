$( document ).ready(function() {
  $('.flipper').flip();
  $('#filters').click(function(){
	console.log("filters clicked");
	window.open('/filters.html','formPrompt','width=600, height=400');
  });
  $('.flip').flip(); 
});

MutationObserver = window.MutationObserver || window.WebKitMutationObserver;

var observer = new MutationObserver(function(mutations, observer) {
    // fired when a mutation occurs
    //$('.flipper').flip();
    // ...
});

// define what element should be observed by the observer
// and what types of mutations trigger the callback
observer.observe(document, {
  subtree: true,
  attributes: true
  //...
});
