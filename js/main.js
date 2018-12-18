const express = require('express');
const bodyParser = require('body-parser');
const app = express();

console.log("NodeJS project v.0.3. Image API is ready");
console.log("URL http://localhost:8080/api/images");
console.log("Source code from https://github.com/elabpro/imageupload");

const port = 8000;
require('./app/routes')(app, {});
app.listen(port, () => {
    console.log('Main API on ' + port);
});

const portForTests = 8080;
app.listen(portForTests, () => {
    console.log('Secondary API on ' + portForTests);
});
