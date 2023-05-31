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
let category = ''


// Parse incoming request bodies
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname,'../public')))

// Endpoint to receive BPM data
app.post('/data', (req, res) => {

  console.log(req.body)
    bpm = req.body.bpm;  // Extract the BPM value from the request body
    console.log(bpm);
    category = req.body.category;

  // Perform any processing or storage of the BPM value here
  res.status(200);
  // Send a response back to the ESP8266
  res.send('BPM received');
});



io.on('connection', (socket) => {
  console.log('Client connected');

  // Send the initial BPM value to the client
  socket.emit('bpmUpdate', { bpm: bpm, category: category });
  console.log('emitted bpmUpdate', bpm);

  // Send the updated BPM value to the client every second
  const sendBPMUpdate = setInterval(() => {
    socket.emit('bpmUpdate', { bpm: bpm, category: category });

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
