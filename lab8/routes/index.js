var express = require('express');
var router = express.Router();
/* Set up mongoose in order to connect to mongo database*/ 
var mongoose = require('mongoose'); //Adds mongoose as a usable dependency

mongoose.createConnection('mongodb://localhost/myDB'); //Connects to a mongo database called "commentDB"

var commentSchema = mongoose.Schema({ //Defines the Schema for this database
  Name: String,
  Comment: String
});

var Comment;

if (mongoose.models.Comment) {
  Comment = mongoose.model('Comment');
} else {
  Comment = mongoose.model('Comment', commentSchema); //Makes an object from that schema as a model
}

//module.exports = Comment;

var db = mongoose.createConnection('mongodb://localhost/myDB'); //Saves the connection as a variable to use
db.on('error', console.error.bind(console, 'connection error:')); //Checks for connection errors
db.once('open', function() { //Lets us know when we're connected
  console.log('Connected');
});

/* GET home page. */
router.post('/comment', function(req, res, next) {  
  var reqString = JSON.stringify(req.body);
  console.log(req.body); 
  var newcomment = new Comment(req.body); //[3]
  console.log("made it past my newcomment block");
  console.log(newcomment); //[3]
  newcomment.save(function(err, post) { //[4]
    if (err) console.log("entered the err if statement"); return console.error(err);
    console.log(post);
    res.sendStatus(200);
  });
});

/* GET comments from database */
router.get('/comment', function(req, res, next) {
  console.log("In the GET route?");
  Comment.find(function(err,commentList) { //Calls the find() method on your database
    if (err) return console.error(err); //If there's an error, print it out
    else {
      	console.log(commentList); //Otherwise console log the comments you found
	res.json(commentList);  
    }
  })
});


router.get('/commentFilter', function(req, res, next) {
  console.log(req.query.q);
  Comment.find({"Name":req.query.q}, function(err,commentList) { //Calls the find() method on your database
    if (err) return console.error(err); //If there's an error, print it out
    else {
      	console.log(commentList); //Otherwise console log the comments you found
	res.json(commentList);  
    }
  })
});

module.exports = router;
