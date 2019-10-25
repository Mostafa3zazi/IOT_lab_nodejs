#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient wsc;

const char *ssid = "CSE Dept";
const char *pass = "CSE@zu2020";

#define SERVER  "192.168.22.49"
#define PORT    9000
#define URL     "/"

void websocketEvent(WStype_t type, uint8_t *data, size_t length){
  switch(type){
    case(WStype_CONNECTED):
      Serial.printf("connected to server\n");
      break;

    case(WStype_TEXT):
      Serial.printf("data: %s\n",data);
      break;
  }
}

void setup(){
  Serial.begin(115200);
    
  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(500);
  }

  Serial.println(WiFi.SSID());
  Serial.println(WiFi.localIP());

  wsc.begin(SERVER, PORT, URL);
  wsc.onEvent(websocketEvent);
  wsc.setReconnectInterval(1000);
}

void loop(){
  wsc.loop();
}

