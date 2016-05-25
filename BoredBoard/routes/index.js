var express = require('express');
var router = express.Router();
//var multer = require('multer');
var multipart = require('connect-multiparty');
var multipartMiddleware = multipart();

router.post('/upload', multipartMiddleware, function(req, resp) {
  console.log(req.body, req.files);
  // don't forget to delete all req.files when done
});

/*
var uploading = multer({
  dest: __dirname + '../public/uploads/',
});

router.post('/upload', uploading.single('photo'), function(req, res){
  console.log("in post request");
  console.log(req);
  console.log(req.file);
  console.log(req.body);
  
});*/

//var fileUpload = require('express-fileupload');

//var myApp = express();
//myApp.use(fileUpload());
//var multer = require('multer'),
//	bodyParser = require('body-parser'),
//	path = require('path');

//var router = express.Router();
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
  console.log("the next line will be the req.body in the routes/index post");
  console.log(req.body);
  var newactivity = new Activity(req.body); //[3]
//  newactivity.Description = req.body.Description;

  console.log("the next line will be the newActivity");
  console.log(newactivity); //[3]
  newactivity.save(function(err, post) { //[4]
    if (err) return console.error(err);
    console.log(post);
    res.sendStatus(200);
  }); 
});

/* GET filtered activities from database */
router.post('/filtered-activities', function(req, res, next) {
	console.log(req.body);

	// use the req body to alter the find function and only get certain
	// activities.
	Activity.find(req.body, function(err,activityList) { //Calls the find() method on your database
	// this ^ works now I need to figure out how to onlly add filters to the find method if they've been set.
		if (err) return console.error(err); //If there's an error, print it out
		else {
			console.log(activityList); //Otherwise console log the comments you found
			res.json(activityList); //Then send them
		  
		}
	})
});

/* GET activities from database */
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

/*router.get('/submit-pic', function(req, res, next){
  console.log("in get pic route");
  
}*/

/// Show files
router.get('/uploads/:file', function (req, res){
  file = req.params.file;
  var img = fs.readFileSync(__dirname + "/uploads/" + file);
  res.writeHead(200, {'Content-Type': 'image/jpg' });
  res.end(img, 'binary');
});
/*
router.post('/upload', upload.single('photo'), function(req, res, next) {
  //do something and return data here?
  console.log("in router.post function");
  console.log(req.file);
  console.log(req.body);
  var sampleFile;
  if(!req.file){
    console.log('no file was uploaded');
    res.send('no file was uploaded');
    return;
  }
  sampleFile = req.file;
  sampleFile.mv('/uploads/gandalf.jpg', function(err){
    if(err){
  	console.log('got some error');
  	res.status(500).send(err);
    }
    else{
  	console.log("file uploaded");
  	res.send("file uploaded");
    }
  });  
});*/
 
module.exports = router;
