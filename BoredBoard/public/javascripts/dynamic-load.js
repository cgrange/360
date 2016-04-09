$(document).ready(function(){
	$.getJSON('submit-activity', function(data) {
	        console.log(data);
	        var everything = "<ul>";
	        for(var title in data) {
		        act = data[title];
		        everything += "<li>Title: " + act.Title + " -- Description: " + act.Description + "</li>";
	        }
	        everything += "</ul>";
	        $("body").html(everything);
      	})
});
