$(document).ready(function(){
    $("#serialize").click(function(){
        var myobj = {Title:$("#title").val(),Description:$("#description").val()};
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
	  }
	})
    });
    $("#getThem").click(function() {
      	$.getJSON('submit-activity', function(data) {
	        console.log(data);
		window.location.href='dynamic-activities.html';
	        var everything = "<ul>";
	        for(var title in data) {
		        act = data[title];
		        everything += "<li>Title: " + act.Title + " -- Description: " + act.Description + "</li>";
	        }
	        everything += "</ul>";
	        $("body").html(everything);
      	})
    });
});
