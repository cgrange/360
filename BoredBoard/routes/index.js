var express = require('express');
var router = express.Router();
var fs = require('fs-extra');       //File System - for file manipulation

/* Set up mongoose in order to connect to mongo database */
var mongoose = require('mongoose'); //Adds mongoose as a usable dependency

mongoose.connect('mongodb://localhost/activityDB'); //Connects to a mongo database called "activityDB"

var activitySchema = mongoose.Schema({ //Defines the Schema for this database
  Title: String,
  Description: String,
  Winter: String,
  Spring: String,
  Summer: String,
  Fall: String,
  Outdoor: String,
  Indoor: String,
  Cost: Number,
  Lat: Number,
  Lng: Number,
  Address: String,
  Photo: String
});

var Activity = mongoose.model('Activity', activitySchema); //Makes an object from that schema as a model

var db = mongoose.connection; //Saves the connection as a variable to use
db.on('error', console.error.bind(console, 'connection error:')); //Checks for connection errors

var fs = require('fs');

db.once('open', function() { //Lets us know when we're connected
  console.log('Connected');
});


router.post('/upload', function(req, res, next) {
  	var fstream;
        req.pipe(req.busboy);
        req.busboy.on('file', function (fieldname, file, filename) {
            console.log("Uploading: " + filename);
	    
            //Path where image will be uploaded
	    var dir = __dirname;
	    //console.log('old dir = ' + dir);
	    var idx = dir.indexOf('routes');
	    dir = dir.substr(0, idx);
	    //console.log('new dir = ' + dir);
            fstream = fs.createWriteStream(dir + 'public/images/' + filename);
            file.pipe(fstream);
            fstream.on('close', function () {    
                console.log("Upload Finished of " + filename);              
                res.redirect('back');           //where to go next
            });
        });
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
  //console.log("the next line will be the req.body in the routes/index post");
  //console.log(req.body);
  var newactivity = new Activity(req.body); //[3]
//  newactivity.Description = req.body.Description;

  //console.log("the next line will be the newActivity");
  //console.log(newactivity); //[3]
  newactivity.save(function(err, post) { //[4]
    if (err) return console.error(err);
    //console.log(post);
    res.sendStatus(200);
  }); 
});

/* GET filtered activities from database */
router.post('/filtered-activities', function(req, res, next) {
	console.log('req.body: ');
	console.log(req.body);
	//var str = JSON.parse(req.body);
	//console.log('didnt fail yet');
	//var newBod = req.body.replace(/['"]+/g, '');
	//console.log(newBod);
	//jobj = JSON.parse(str);
	//console.log('parsed obj: ');
	//console.log(jobj);
	// use the req body to alter the find function and only get certain
	// activities.
	Activity.find(req.body, function(err,activityList) { //Calls the find() method on your database
	// this ^ works now I need to figure out how to onlly add filters to the find method if they've been set.
		if (err) return console.error(err); //If there's an error, print it out
		else {
			//console.log(activityList); //Otherwise console log the comments you found
			res.json(activityList); //Then send them
		  
		}
	})
});

/* GET activities from database */
router.get('/submit-activity', function(req, res, next) {
	//console.log("In the GET route?");
	Activity.find(function(err,activityList) { //Calls the find() method on your database
		if (err) return console.error(err); //If there's an error, print it out
		else {
			//console.log(activityList); //Otherwise console log the comments you found
			res.json(activityList); //Then send them
		  
		}
	})
});

module.exports = router;
