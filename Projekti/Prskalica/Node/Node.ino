#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "ZHB_WiFi";
const char* password = "12345678";
const char* mqtt_server = "iot.eclipse.org";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
    delay(1000);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED){
      delay(500);
    }    
    randomSeed(micros());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  int p =(char)payload[0]-'0';
  //if (topic =="bozaSub1/led1"){
    if(p==1){
      Serial.print('H');
      digitalWrite(D0, HIGH);
    }
    else{
      Serial.print('L');
      digitalWrite(D0, LOW);
    }
      
  
}

void reconnect() {
  while (!client.connected()){
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      client.subscribe("bozaSub1/led1");
    } else {
      delay(5000);
    }
  }
} 

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(D0, OUTPUT);
  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.setCallback(callback);
  client.loop();
}

