$(document).ready(function(){

  //================== submit ============================ 
   $("#serialize").click(function(){
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
	if($('#address').val() == ''){
		moveOn = false;  
	}
	// make sure title, cost, and description are filled out
	if(!moveOn){
		alert("please fill out the form entirely before trying to submit");
		return;
	}
	//google geocoding api to get Lat and Lon and submit those as well
	// geocoding key: AIzaSyDh_JWlyBfzOXP5F9fXIgsaEc2PsFpyf44
	var addressStr = $('#address').val();
	if(addressStr.search('lng') == -1){
		addressStr = addressStr.replace(/\s+/g, '+');
		var geoCodeUrl = "https://maps.googleapis.com/maps/api/geocode/json?address=" + addressStr + "&key=AIzaSyDh_JWlyBfzOXP5F9fXIgsaEc2PsFpyf44";	
		var lat, lng;
		$.ajax({
		  url:geoCodeUrl,
		  type:"GET",
		  async:false,
		  success: function(data, textStatus){
			console.log(data);
			lat = data.results[0].geometry.location.lat;
			lng = data.results[0].geometry.location.lng;
			var myobj = {Title:$("#title").val(),Description:$("#description").val(),Winter:winterTime,Spring:springTime,Summer:summerTime,Fall:fallTime,Indoor:inside,Outdoor:outside,Cost:$('#cost').val(),Lat:lat,Lng:lng,Address:$('#address').val(),Photo:$('#photo').text()};
			jobj = JSON.stringify(myobj);
			$("#json").text(jobj);
			var url = "submit-activity";
			$.ajax({
			  url:url,
			  type: "POST",
			  data: jobj,
			  contentType: "application/json; charset=utf-8",
			  success: function(data,textStatus) {
			      	$("#done").html("You're activity has been submitted!");
			      	console.log(data);
			  	$('#submissionForm').submit();
			  }
			})
		  }
		});
	}else{
	  var addressObj = JSON.parse(addressStr);
	  var geoCodeUrl = "https://maps.googleapis.com/maps/api/geocode/json?latlng=" + addressObj.lat + "," + addressObj.lng + "&key=AIzaSyDh_JWlyBfzOXP5F9fXIgsaEc2PsFpyf44";	 
	  $.ajax({
		url:geoCodeUrl,
		type:"GET",
		async:false,
		success: function(data, textStatus){
		  console.log(data);
		  console.log(data.results[0].formatted_address);
		  var myobj = {Title:$("#title").val(),Description:$("#description").val(),Winter:winterTime,Spring:springTime,Summer:summerTime,Fall:fallTime,Indoor:inside,Outdoor:outside,Cost:$('#cost').val(),Lat:addressObj.lat,Lng:addressObj.lng,Address:data.results[0].formatted_address,Photo:$('#photo').text()};
		  jobj = JSON.stringify(myobj);
		  $("#json").text(jobj);
		  var url = "submit-activity";
		  $.ajax({
			url:url,
			type: "POST",
			data: jobj,
			contentType: "application/json; charset=utf-8",
			success: function(data,textStatus) {
			  $("#done").html("You're activity has been submitted!");
			  console.log(data);
			  $('#submissionForm').submit();
			}
		  })
		}
	  });
	}
    });

//================= CHECK/UNCHECK =======================
	var winterRadio;
    $('#winter').click(function(){	
	if(winterRadio == this){
		this.checked = false;
		winterRadio = null;
	}
	else{
		winterRadio = this;
	}
    });
	var springRadio = null;
    $('#spring').click(function(){	
	if(springRadio == this){
		this.checked = false;
		springRadio = null;
	}
	else{
		springRadio = this;
	}
    });
	var summerRadio = null;
    $('#summer').click(function(){	
	if(summerRadio == this){
		this.checked = false;
		summerRadio = null;
	}
	else{
		summerRadio = this;
	}
    });
	var fallRadio = null;
    $('#fall').click(function(){	
	if(fallRadio == this){
		this.checked = false;
		fallRadio = null;
	}
	else{
		fallRadio = this;
	}
    });
});
