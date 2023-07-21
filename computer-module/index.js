// Web app server configuration
const express = require('express');

const port = 4000;

const app = express();


app.use(express.static(__dirname + '/src'));

app.get('/', (req, res) => {
    res.render('index.html')
});

app.listen(port, () => {
  console.log(`Node.js server started on http://localhost:${port}`);
});



