const express = require('express');
const SocketServer = require('ws').Server;
const path = require('path');

const PORT = process.env.PORT || 9000;
const INDEX = path.join(__dirname, '/pages/index.html');

const server = express()
	.use(express.static(path.join(__dirname, "pages")))
	.use('/',(req, res) =>{	res.sendFile(INDEX) })
	.listen(PORT, () => console.log(`Listening on ${ PORT }`))

const wss = new SocketServer({ server });

wss.on('connection', (ws) => {
	ws.on('message', function(msg) {
		var json_data = JSON.parse(msg);
		console.log(json_data);
		switch (json_data.type){
			case "connection_type":
				console.log(json_data.text);
				ws.type = json_data.text;
				break;

			case "username":
				ws.username = json_data.text;
				break;

			case "chat":
				var chat_message = {
					type:"chat",
					text: ws.username + " : " + json_data.text,
				};
				wss.clients.forEach(function(client){
					if(client.type=="client_connection"){
						client.send(JSON.stringify(chat_message));
					}
				});
				break;

			case "command":
				wss.clients.forEach(function(client){
					if(client.type=="hw_connection"){
						client.send(json_data.text);
					}
				});
				break;

			case "sensor":
				var sensor_message = {
					type:"sensor",
					text:json_data.text,
				};
				wss.clients.forEach(function(client){
					if(client.type=="client_connection"){
						client.send(JSON.stringify(sensor_message));
					}
				});
				break;

		}

		// console.log(data);
		// console.log(wss);
		// if (msg.data = "client_connection") {wss.type ="client";}
		// else if (msg.data ="hw_connection") {wss.type ="hw";}
		
		
	});
	
	ws.on('close', () => console.log('Client disconnected'));
});