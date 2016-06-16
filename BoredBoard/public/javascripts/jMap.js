// In the following example, markers appear when the user clicks on the map.

var marker;

function initialize() {
  var provo = { lat: 40.2338, lng: -111.6586 };
  var map = new google.maps.Map(document.getElementById('map'), {
    zoom: 13,
    center: provo,
    mapTypeId: google.maps.MapTypeId.ROADMAP //might want to try HYBRID instead of ROADMAP
  });

  // This event listener calls addMarker() when the map is clicked.
  google.maps.event.addListener(map, 'click', function(event) {
    removeMarker();
    addMarker(event.latLng, map);
  });
}

// Adds a marker to the map.
function addMarker(location, map) {
  // Add the marker at the clicked location, and add the next-available label
  // from the array of alphabetical characters.
  //console.log(JSON.stringify(location));
  $('#address').val(JSON.stringify(location));
  marker = new google.maps.Marker({
    position: location,
    map: map
  });
}

function initMap(){
	google.maps.event.addDomListener(window, 'load', initialize);
}

// to remove a marker from the map (doesn’t delete it)
function removeMarker(){
  if(marker){
	marker.setMap(null);
  }
}

