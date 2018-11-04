//fali neonka
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<dht.h>
dht DHT;

// Define NodeMCU D3 pin to as temperature data pin of  DHT11
#define DHT11_PIN D1

const char* ssid = "e665e4";
const char* password = "262751838";
const char* mqtt_server = "iot.eclipse.org";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
int value = 0;
char msg[50];
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
     int chk = DHT.read11(DHT11_PIN);
     String msg="";
     msg= msg+ DHT.temperature;
     char message[58];
     msg.toCharArray(message,58);
     Serial.println(message);
     client.publish("bozaSub/kuca/node2/dvoriste/temperatura", message);

     msg="";
     msg=msg+DHT.humidity ;
     char message2[58];
     msg.toCharArray(message2,58);
     Serial.println(message2);
    client.publish("bozaSub/kuca/node2/dvoriste/vlaznost", message2);
    }
  }
} //end callback

void reconnect() {
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("bozaSub/kuca/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  int chk = DHT.read11(DHT11_PIN);
  Serial.print(" Starting Humidity: " );
  Serial.print(DHT.humidity, 1);
  Serial.println('%');
  Serial.print(" Starting Temparature ");
  Serial.print(DHT.temperature, 1);
  Serial.println('C');
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  // read DHT11 sensor every 6 seconds
  if (now - lastMsg > 6000) {
     lastMsg = now;
     int chk = DHT.read11(DHT11_PIN);
     String msg="";
     msg= msg+ DHT.temperature;
     char message[58];
     msg.toCharArray(message,58);
     Serial.println(message);
     client.publish("bozaSub/kuca/node2/dvoriste/temperatura", message);

     msg="";
     msg=msg+DHT.humidity ;
     char message2[58];
     msg.toCharArray(message2,58);
     Serial.println(message2);
    client.publish("bozaSub/kuca/node2/dvoriste/vlaznost", message2);

  }
}
