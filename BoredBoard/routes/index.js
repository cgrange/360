var express = require('express');
var router = express.Router();
/* Set up mongoose in order to connect to mongo database */
var mongoose = require('mongoose'); //Adds mongoose as a usable dependency

mongoose.connect('mongodb://localhost/activityDB'); //Connects to a mongo database called "activityDB"

var activitySchema = mongoose.Schema({ //Defines the Schema for this database
  Title: String,
  Description: String
});

var Activity = mongoose.model('Activity', activitySchema); //Makes an object from that schema as a model

var db = mongoose.connection; //Saves the connection as a variable to use
db.on('error', console.error.bind(console, 'connection error:')); //Checks for connection errors
db.once('open', function() { //Lets us know when we're connected
  console.log('Connected');
});


/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});

router.get('/theBoard.html', function(req, res, next) {
  res.sendFile('theBoard.html');
});

router.get('/activities.html', function(req, res, next) {
  res.sendFile('activities.html');
});

router.get('/events.html', function(req, res, next) {
  res.sendFile('events.html');
});

router.get('/activities', function(req, res, next) {
  res.sendFile('activities.html');
});

router.get('/events', function(req, res, next) {
  res.sendFile('events.html');
});

router.get('/favicon.ico', function(req, res, next) {
  console.log('entered my favicon route');
  res.sendFile('/images/favicon.ico');
});

/*Post to Backend*/
router.post('/submit-activity', function(req, res, next) { 
  console.log(req.body);
  var newactivity = new Activity(req.body); //[3]
//  newactivity.Description = req.body.Description;
  console.log(newactivity); //[3]
  newactivity.save(function(err, post) { //[4]
    if (err) return console.error(err);
    console.log(post);
    res.sendStatus(200);
  }); 
});

/* GET comments from database */
router.get('/submit-activity', function(req, res, next) {
	console.log("In the GET route?");
	Activity.find(function(err,activityList) { //Calls the find() method on your database
		if (err) return console.error(err); //If there's an error, print it out
		else {
			console.log(activityList); //Otherwise console log the comments you found
			res.json(activityList); //Then send them
		  
		}
	})
});
 
module.exports = router;
