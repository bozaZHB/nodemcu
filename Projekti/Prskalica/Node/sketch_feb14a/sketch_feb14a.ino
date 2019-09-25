
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "ZHB_WiFi";
const char* password = "12345678";
const char* mqtt_server = "iot.eclipse.org";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
    delay(1000);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Command from MQTT broker is : [");
  Serial.print(topic);
  int p =(char)payload[0]-'0';
  
  if(p==0){
    digitalWrite(D0, HIGH);delay(50);
    digitalWrite(D0, LOW);
  }
    
  if(p==1){
    digitalWrite(D1, HIGH);delay(50);
    digitalWrite(D1, LOW);
  }
}

void reconnect() {
  while (!client.connected()) //
  {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      client.subscribe("bozaSub");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 seconds");
      delay(1000);
    }
  }
} 

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  digitalWrite(D0, LOW);     
  digitalWrite(D1, LOW);    
}

void loop() {    
    if (!client.connected()) {
      reconnect();
    }
    client.setCallback(callback);
    client.loop();
}
