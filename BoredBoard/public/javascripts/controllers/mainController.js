app.controller('MainCtrl', ['$scope','$http','$sce','$route', function($scope,$http,$sce,$route){
    var activityResults;
    var noMore = false;
    var X = 0;
    $scope.mylat;
    $scope.myLng; 
    $scope.open = false;
    $scope.activities = [];
    $scope.getAll = function() {
      return $http.get('/submit-activity').success(function(data){
        activityResults=data;
	for(var i = 0; i < 18; i++){
	  $scope.activities.push(activityResults[i]);
	}
	$route.reload();
	//angular.copy(data, $scope.activities);
      });
    };
    $scope.showPopup = function(){
	$scope.open = !$scope.open;
    };
    $scope.getAll();
    $scope.filteredActivities = []; 
    $scope.filterActivities = function() { 
	//$('#popup').css('display','none');
	var springTime = "false";
	var winterTime = "false";
	var fallTime = "false";
	var summerTime = "false";
	var moveOn = false;
	var inside = "false";
	var outside = "false";
	var jsonStr = '{';
	if($('#winter').is(":checked")){
		winterTime="true";
		moveOn = true;
		jsonStr += '"Winter":"true",';
	}
	if($('#spring').is(":checked")){
		springTime = "true";
		moveOn = true;
		jsonStr += '"Spring":"true",';
	}
	if($('#summer').is(":checked")){
		summerTime = "true";
		moveOn = true;
		jsonStr += '"Summer":"true",';
	}
	if($('#fall').is(":checked")){
		fallTime = "true";
		moveOn = true;
		jsonStr += '"Fall":"true",';
	}
	if(!$('#indoor').is(":checked") && !$('#outdoor').is(":checked")){
		moveOn = false;
	}
	else{
		if($('#indoor').is(":checked")){
			inside = "true";
			jsonStr += '"Indoor":"true",';
		}
		else{
			outside = "true";
			jsonStr += '"Outdoor":"true",';
		}
	}
	if($('#cost').val() != ''){
		jsonStr += '"Cost": { "$lte": ' + $('#cost').val() +' },';
		//jsonStr += '"Cost": ' + $('#cost').val() + ',';
	}
	var radius = -1;
	if($('#radius').val() != ''){
		radius = $('#radius').val();
	}
	jsonStr = jsonStr.substring(0, jsonStr.length - 1);
	jsonStr += '}';
	console.log("jsonStr: " + jsonStr);
	var testObj = JSON.parse(jsonStr);
        jobj = JSON.stringify(testObj);
	console.log(jobj);
	
	var url = "/filtered-activities";

	$.ajax({
	  //async:false,
	  url:url,
	  type: "POST",
	  data: jobj,
	  contentType: "application/json; charset=utf-8",
	  success: function(data,textStatus) {
		$('.ninja').css('display', 'block');
		activityResults = data;
		if(data.length > 18){
			activityResults = data;
			var result = 8*280 + 61;
			var strResult = result.toString();
			strResult += 'px';
			$('.spacer').css('height', strResult);
			for(var i = 0; i < 18; i++){
				//console.log(data[i]);
				checkDist(activityResults[i], radius);
			}
		}
		else{
			noMore = true;
			var mult = data.length/3;
			var realMult = Math.ceil(mult);
			var result = realMult*280 + 61;
			var strResult = result.toString();
			strResult += 'px';
			$('.spacer').css('height', strResult);
			for(var i = 0; i < data.length; i++){
				//console.log(data[i]);
				checkDist(data[i], radius);
			}
		}
	  },
	  error: function(err){
		alert('there was an error');
		console.log(err);
	  }
	});
	$scope.open = !$scope.open;
    };
    var oldHeader = '<a href="/"><img src="/images/BB.png" id="bb-symbol" class="pull-left" alt="bored board symbol"></a>  <ul class="pull-left"><li class="btn-link btn-lg">login</li><li class="btn-link btn-lg">sign up</li></ul><ul class="pull-right"><a href="#myPopup" data-rel="popup"><li id="ilters" class="btn-link btn-lg">filters</li></a><a href="/activities.html"><li class="btn-link btn-lg">activities</li></a><a href="/events.html"><li class="btn-link btn-lg">events</li></a></ul>';
    $scope.getFiltered = function() {
	// get the ajax call hin here from filter.js and have the submit button
	// in the filter page triger something in here that calls
	// get filtered.
      return $http.post('/filtered-activities').success(function(data){
        angular.copy(data, $scope.activities);
      });
    };
	function myLocation(){
	  //if(!$scope.mylat){
	    if(navigator.geolocation){
	  	navigator.geolocation.getCurrentPosition(showPosition);
	    }
	    else{
		alert('Your browser does not support geolocation');
	    }
	  //}
	}

	function showPosition(position){
	  //alert("Latitude: " + position.coords.latitude + " Longitude: " + position.coords.longitude);
	  $scope.mylat = position.coords.latitude;
	  $scope.myLng = position.coords.longitude;
	  $route.reload();
	  //initMap();
	}
    $scope.getLocation = function(){
	myLocation();
    }
    $scope.getLocation();

    function checkDist(data, radius){
	var lat = data.Lat;
	var lng = data.Lng; 
	var distance;
	
	var origin = new google.maps.LatLng($scope.mylat, $scope.myLng);
	var destination = new google.maps.LatLng(lat, lng);

	var service = new google.maps.DistanceMatrixService();
	service.getDistanceMatrix(
	  {
	    origins: [origin],
	    destinations: [destination],
	    travelMode: google.maps.TravelMode.DRIVING,
	    unitSystem: google.maps.UnitSystem.IMPERIAL,
	  }, callback);

	function callback(response, status) {
	  if (status == google.maps.DistanceMatrixStatus.OK) {
	    //var origins = response.originAddresses;
	    //var destinations = response.destinationAddresses;
	    //for (var i = 0; i < origins.length; i++) {
		var results = response.rows[0].elements;
		var element = results[0];
		var meters = element.distance.value;
		distance = meters/1609; //distance measured in miles
		//console.log('the callback is over');
		//var duration = element.duration.text;
		if(radius == -1){
			radius = 30;
		}
		if(distance <= radius){
		  $scope.filteredActivities.push(data);
		}
		$route.reload();
	    //return distance;
	    //}
	  }
	  else{
		console.log("the google distance matrix request failed");
	  }
	}
    };
//================= Infinite Scroll =====================
	
	var win = $(window);
	$scope.loading = false;
	// Each time the user scrolls
	win.scroll(function() {
		if(noMore){return;};
		// End of the document reached?
		if ($(document).height() - win.height() == win.scrollTop()) {
			X += 1;
			$scope.loading = true;
			var i = 18*X;
			if(activityResults.length <= i + 17){
			  noMore = true;
			  for(i; i < activityResults.length; i++){
  				$scope.activities.push(activityResults[i]);
			  }
			  $route.reload();
			  $scope.loading = false;
			}
			else{
			  for(i; i < i+18; i++){
	  			$scope.activities.push(activityResults[i]);
			  }
			  $scope.loading = false;
			} 
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
	var indoorRadio = null;
    $('#indoor').click(function(){	
	if(indoorRadio == this){
		this.checked = false;
		indoorRadio = null;
	}
	else{
		indoorRadio = this;
	}
    });
	var outdoorRadio = null;
    $('#outdoor').click(function(){	
	if(outdoorRadio == this){
		this.checked = false;
		outdoorRadio = null;
	}
	else{
		outdoorRadio = this;
	}
    });
  }
]);
