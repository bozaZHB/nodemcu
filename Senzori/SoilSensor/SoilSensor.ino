#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "e665e4";
const char* password = "262751838";
const char* mqtt_server = "192.168.0.100";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
int sense_Pin = 0;

void setup_wifi() {
  delay(100);
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
  char p =(char)payload[0];//-'0'
  String str(topic);
  if (str =="bozaSub/kuca/node1/stanje"){
    if(p=='s'){
     client.publish("bozaSub/kuca/node4/vlaznost/stanje", String(analogRead(sense_Pin)).c_str(),false);
    }
  }

}

void reconnect() {
  if (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      client.subscribe("bozaSub/kuca/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(500);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.end();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    client.publish("bozaSub/kuca/node4/vlaznost/stanje", String(analogRead(sense_Pin)).c_str(),false);
  }
}
