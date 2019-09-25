#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Brezovica";
const char* password = "rokizitiste";
const char* mqttServer = "m23.cloudmqtt.com";
const int mqttPort = 13634; // iot.eclipse.org - 1883  m23.cloudmqtt.com - 13634
const char* user= "ypdfqxdh";
const char* passw= "PuluE8hNViwT";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
   delay(100);
   //Serial.print("Connecting to ");Serial.println(ssid);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) delay(500);
   randomSeed(micros());
   //Serial.println("");Serial.println("WiFi connected");Serial.println("IP address: "); Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  //client.publish("zhb/zhb", String("eto ga").c_str(), false);
  //byteToString(payload, length)
  char p =(char)payload[0];
  String strTopic(topic);
  if (strTopic =="zhb/proba"){
    Serial.println(p);
  }
} 

void reconnect() {
  while (!client.connected()) 
  {
    //Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(),user,passw))
    {
      //Serial.println("connected");
      client.subscribe("zhb/#");
    } else {
      //Serial.print("failed, rc="); Serial.print(client.state());
      delay(5000);
    }
  }
} 

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

String byteToString(byte* byteMessage, int length){
  char charMessage[length+1];
  for(int i=0; i<length; i++)
    charMessage[i] = (char)byteMessage[i];
  return (String(charMessage)).substring(0,length);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  
}


