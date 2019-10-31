#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <String.h>
#include <ArduinoJson.h>

WebSocketsClient wsc;

const char *ssid = "Unknown";
const char *pass = "m.azazi33";
/*
#define SERVER  "192.168.1.6"
#define PORT    9000
#define URL     "/"
*/
#define SERVER  "vast-brushlands-47071.herokuapp.com"
#define PORT    80
#define URL     "/"

bool isConnected = false;
uint64_t now;
uint64_t prev;

#define device1 16
#define device2 19
#define device3 20
#define sensor A0

int sensor_reading = 0;
char sensor_reading_text[100];

void websocketEvent(WStype_t type, uint8_t *data, size_t length){
  switch(type){
    case(WStype_CONNECTED):
      Serial.printf("connected to server\n");
      //{"type":"connection_type","text":"hw_connection"}
      wsc.sendTXT("{\"type\":\"connection_type\",\"text\":\"hw_connection\"}");
      isConnected = true;
      break;

    case(WStype_TEXT):
    {
      Serial.printf("data: %s\n",data);
      char* s = (char*)data;
      if      (strcmp(s,"device1on") == 0)    {digitalWrite(device1,HIGH);}
      else if (strcmp(s,"device1off") == 0)   {digitalWrite(device1,LOW);}
      else if (strcmp(s,"device2on") == 0)    {digitalWrite(device2,HIGH);}
      else if (strcmp(s,"device2off") == 0)   {digitalWrite(device2,LOW);}
      else if (strcmp(s,"device3on") == 0)    {digitalWrite(device3,HIGH);}
      else if (strcmp(s,"device3off") == 0)   {digitalWrite(device3,LOW);}
      /*
      switch (){
        case "device1on":
          digitalWrite(device1,HIGH);
          break;
        case "device1off":
          digitalWrite(device1,LOW);
          break;
        case "device2on":
          digitalWrite(device2,HIGH);
          break;
        case "device2off":
          digitalWrite(device2,LOW);
          break;
        case "device3on":
          digitalWrite(device3,HIGH);
          break;
        case "device3off":
          digitalWrite(device3,LOW);
          break;
      }*/
      break;
    }
    case(WStype_DISCONNECTED):
      Serial.printf("server Disconnected!\n");
      isConnected = false;
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
  
  pinMode(device1,OUTPUT);
  pinMode(device2,OUTPUT);
  pinMode(device3,OUTPUT);
  pinMode(sensor,INPUT);
  now = millis();
  prev = now;
}

void loop(){
  wsc.loop();
  if(isConnected){
    now = millis();
    if((now-prev)>1000){
      sensor_reading=analogRead(sensor);
      StaticJsonDocument<500> doc;
      doc["type"] = "sensor";
      doc["text"] = sensor_reading;
      serializeJson(doc, sensor_reading_text);
      wsc.sendTXT(sensor_reading_text);
      prev = now;
    }
  }
}

