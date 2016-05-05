$(document).ready(function(){
  $('#filter').click(function(){
	var springTime = "false";
	var winterTime = "false";
	var fallTime = "false";
	var summerTime = "false";
	var moveOn = false;
	var inside = "false";
	var outside = "false";
	if($('#winter').is(":checked")){
		winterTime="true";
		moveOn = true;
	}
	if($('#spring').is(":checked")){
		springTime = "true";
		moveOn = true;
	}
	if($('#summer').is(":checked")){
		summerTime = "true";
		moveOn = true;
	}
	if($('#fall').is(":checked")){
		fallTime = "true";
		moveOn = true;
	}
	if(!$('#indoor').is(":checked") && !$('#outdoor').is(":checked")){
		moveOn = false;
	}
	else{
		if($('#indoor').is(":checked")){
			inside = "true";
		}
		else{
			outside = "true";
		}
	}
	var myobj = {Title:$("#title").val(),Description:$("#description").val(),Winter:winterTime,Spring:springTime,Summer:summerTime,Fall:fallTime,Indoor:inside,Outdoor:outside,Cost:$('#cost').val()};
        jobj = JSON.stringify(myobj);
	
	var url = "/filtered-activities";
	$.ajax({
	  url:url,
	  type: "POST",
	  data: jobj,
	  contentType: "application/json; charset=utf-8",
	  success: function(data,textStatus) {
	    // go back to the activities page 
	    
	  }
	});
/*
	var url = "/filtered-activities?q="+$("#cityfield").val();
  	$.getJSON(url,function(data) { 
    	var everything;
    	everything = "<ul>";
    	$.each(data, function(i,item) {
      		everything += "<li> "+data[i].city;
    	});
    
    	everything += "</ul>";*/ 
  });

});
