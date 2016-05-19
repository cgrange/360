app.controller('MainCtrl', ['$scope','$http','$sce','$route', function($scope,$http,$sce,$route){
    $scope.open = false;
    $scope.activities = [];
    $scope.getAll = function() {
      return $http.get('/submit-activity').success(function(data){
        angular.copy(data, $scope.activities);
      });
    };
    $scope.showPopup = function(){
	$scope.open = !$scope.open;
    };
    $scope.getAll();
//    $scope.togglePopup = function(){
//	$scope.isPopupOpen = !$scope.isPopupOpen;
//   }
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
		jsonStr += '"Cost":' + $('#cost').val() +',';
	}
	jsonStr = jsonStr.substring(0, jsonStr.length - 1);
	jsonStr += '}';
	//console.log("jsonStr: " + jsonStr);
	var testObj = JSON.parse(jsonStr);
        jobj = JSON.stringify(testObj);
	
	var url = "/filtered-activities";

	$.ajax({
	  async:false,
	  url:url,
	  type: "POST",
	  data: jobj,
	  contentType: "application/json; charset=utf-8",
	  success: function(data,textStatus) {
		$('.ninja').css('display', 'block');
		//console.log(data[0]); 
		var mult = data.length/3;
		var realMult = Math.ceil(mult);
		var result = realMult*280 + 61;
		var strResult = result.toString();
		strResult += 'px';
		$('.spacer').css('height', strResult);
		for(var i = 0; i < data.length; i++){
			console.log(data[i]);
			//var dist = getDist(data[i].locale);
			//if(dist <= radius){
			  $scope.filteredActivities.push(data[i]);
			//}
		}
		console.log('just finished loading the filtered activities');
		$route.reload();
	  }
	});
        $('.flipper').flip();
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
	var myLat, myLon;
	function myLocation(){
	  if(navigator.geolocation){
	  	navigator.geolocation.getCurrentPosition(showPosition);
	  }
	  else{
		alert('Your browser does not support geolocation');
	  }
	}

	function showPosition(position){
	  alert("Latitude: " + position.coords.latitude + " Longitude: " + position.coords.longitude);
	  myLat = position.coords.latitude;
	  myLon = position.coords.longitude;
	}

    function getDist(otherLocation){    
	myLocation();
	
	  var distanceUrl = "https://maps.googleapis.com/maps/api/distancematrix/json?units=imperial&origins=Provo+UT&destinations=" + value + "+UT&key=AIzaSyD9LwLzWPsMzeOUCb86SURo94MdIndpmKE";
	    console.log("this is right before the ajax distance call");
	    var distanceService = new google.maps.DistanceMatrixService();
	    distanceService.getDistanceMatrix({
		origins: ['' + myLat + ', ' + myLon],
		destinations: [''+ otherLocation.lat + ', ' + otherLocation.lon],
		unitSystem: google.maps.UnitSystem.IMPERIAL,
		travelMode: google.maps.TravelMode.DRIVING,   
	    },
	    function (response, status) {
		if (status !== google.maps.DistanceMatrixStatus.OK) {
		    console.log('Error:', status);
		} else {
		    console.log(response);
		    var destinations = response.destinationAddresses;
		    var results = response.rows[0].elements;
		    var distances = "<ul>";
		    for (var j = 0; j < results.length; j++) {
			var element = results[j];
			var distance = element.distance.text;
			var duration = element.duration.text; 
			var to = destinations[j];
			var output = "distance to " + destinations[j] + ": " + distance; 
			console.log(output);
			distances += "<li>" + output + "</li>"; 
		    }
		    distances += "</ul>";
		    $('#distances').html(distances);
		}
	    });
	  console.log("this is right after the ajax distance call");
    };
    /*$scope.getCoords = function() {
	var distanceUrl = "https://maps.googleapis.com/maps/api/distancematrix/json?units=imperial&origins=Provo+UT&destinations=" + value + "+UT&key=AIzaSyD9LwLzWPsMzeOUCb86SURo94MdIndpmKE";
	$.getJson(distanceUrl, function({
	    url:distanceUrl,
	    dataType: "jsonp",
	    success : function(data){
		console.log(data);
	    }
	});
    }*/
  }
]);
