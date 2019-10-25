const express = require('express');
const SocketServer = require('ws').Server;
const path = require('path');

const PORT = process.env.PORT || 9000;
const INDEX = path.join(__dirname, '/pages/index.html');

const server = express()
	.use(express.static(path.join(__dirname, "pages")))
	.use('/',(req, res) =>{	res.sendFile(INDEX) })
	.listen(PORT, () => console.log(`Listening on ${ PORT }`))

const socketServer = new SocketServer({ server });
var count = 0;

socketServer.on('connection', (clientSocket) => {
	console.log('Client connected');
	count ++;
	clientSocket.send("hello client" + count);

	clientSocket.on('message', function(msg) {
		console.log(msg);
	});
	
	clientSocket.on('close', () => console.log('Client disconnected'));
});