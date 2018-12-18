const express        = require('express');
const bodyParser     = require('body-parser');
const app            = express();

const port = 8000;
require('./app/routes')(app, {});
app.listen(port, () => {
  console.log('Main API on ' + port);
});

const portForTests = 8080;
app.listen(portForTests, () => {
  console.log('Secondary API on ' + portForTests);
});
