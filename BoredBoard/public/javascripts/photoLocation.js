$(document).ready(function(){
	$('#test12').click(function(){
	  alert('its working');
	});

	$('#fileInput').change(function(){
	  //alert('its working');
	  var f = this.value;
	  f = f.replace(/.*[\/\\]/, '');
	  f = "/images/" + f;
	  $('#photo').text(f);
	});
});
