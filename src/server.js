const path=require('path')
const http=require('http')
const express=require('express')
const app=express()
const socketIo=require('socket.io')
const port='3000'
const server=http.createServer(app)
const io=socketIo(server)
const bodyParser = require('body-parser');
let bpm = 0;


// Parse incoming request bodies
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname,'../public')))

// Endpoint to receive BPM data
app.post('/data', (req, res) => {
    bpm = req.body.bpm;  // Extract the BPM value from the request body
    console.log(bpm);

  // Perform any processing or storage of the BPM value here
  res.status(200);
  // Send a response back to the ESP8266
  res.send('BPM received');
});

app.get('/', (req, res) => {
  const bpm = req.body.bpm;  // Extract the BPM value from the request body
  console.log(req.body.bpm);



  // Perform any processing or storage of the BPM value here
  res.status(200);
  // Send a response back to the ESP8266
  res.send(`
  <html>
  <head>
  <title>Heart Rate Monitor</title>
  </head>
  <body>
  <h1>Heart Rate Monitor</h1>
  <p>Current BPM: ${bpm}</p>
  </body>
  </html>

  `);
});


io.on('connection', (socket) => {
  console.log('Client connected');

  // Send the initial BPM value to the client
  socket.emit('bpmUpdate', bpm);
  console.log('emitted bpmUpdate', bpm);

  // Send the updated BPM value to the client every second
  const sendBPMUpdate = setInterval(() => {
    socket.emit('bpmUpdate', bpm);
    console.log('emitted bpmUpdate', bpm);
  }, 1000);

  // Handle client disconnection
  socket.on('disconnect', () => {
    console.log('Client disconnected');

    // Stop sending BPM updates when the client disconnects
    clearInterval(sendBPMUpdate);
  });
});


// Start the server
server.listen(port, () => {
  console.log(`Server listening on port ${port}`);
});
