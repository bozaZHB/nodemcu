/*
 * NodeMCU - Fontana
 * Smart Home BOZANIC
 * Create by Bozanic Stefan ZHB
 * 22. avgust 2019 last update
 * 
 *  1 RELEY (NEONKA)
 *  1 x temperaturni senzor sa senzorom vlazosti vazduha DH11
 *  
 *  na svakih 15min salje se na server (mySql baza) podatak o trenurnoj temperaturi
 *  
 */
 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<dht.h>
dht DHT;

// senzor se nalazi na D1
#define DHT11_PIN D1 

// podaci o mrezi
const char* ssid = "e665e4";
const char* password = "262751838";
const char* mqtt_server = "192.168.0.100";

WiFiClient espClient;
PubSubClient client(espClient);

// podaci za tajmer
long lastMsg = 0;
char msg[50];
int value = 0;

// izlaz
int NEONKA = D6;

////////////////////////////////////////////////////////////////////
//          SETUP_WIFI
////////////////////////////////////////////////////////////////////
void setup_wifi() {
  delay(100);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

////////////////////////////////////////////////////////////////////
//          CALLBACK
///////////////////////////////////////////////////////////////////
void callback(char* topic, byte* payload, unsigned int length) 
{
  String strTopic(topic);
  Serial.println(strTopic);
  
  char p =(char)payload[0];
  
  // na poruku stanje
  if (strTopic == "bozaSub/kuca/node1/stanje"){
     int chk = DHT.read11(DHT11_PIN);
     
     String temperature = "";
     temperature = temperature + DHT.temperature;
     
     String humidity = "";
     humidity = humidity + DHT.humidity;
     
     char message[58];
     temperature.toCharArray(message,58);
     Serial.println(message);
     client.publish("bozaSub/kuca/node2/dvoriste/temperatura", message, false);

     humidity.toCharArray(message,58);
     Serial.println(message);
     client.publish("bozaSub/kuca/node2/dvoriste/vlaznost", message, false);

     if (digitalRead(NEONKA) == LOW){
        client.publish("bozaSub/kuca/node2/neonka/stanje", String("0").c_str(), false);
     } else {
        client.publish("bozaSub/kuca/node2/neonka/stanje", String("1").c_str(), false);
     }
     
  }
  // neonka
  else if(strTopic =="bozaSub/kuca/node2/neonka"){
    // ON
    if(p == '1'){
      digitalWrite(NEONKA, HIGH);
      client.publish("bozaSub/kuca/node2/neonka/stanje", String("1").c_str(), false);
    } 
    // OFF
    else {
      digitalWrite(NEONKA, LOW);
      client.publish("bozaSub/kuca/node2/neonka/stanje", String("0").c_str(), false);
    }
  } 
  
}

////////////////////////////////////////////////////////////////////
//          RECONNECT
////////////////////////////////////////////////////////////////////
void reconnect() {
  while (!client.connected()) 
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
      delay(5000);
    }
  }
}

////////////////////////////////////////////////////////////////////
//          SETUP
////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_OFF);    
  delay(1000);
  WiFi.mode(WIFI_STA);  
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(NEONKA, OUTPUT); 
  digitalWrite(NEONKA, LOW); 
}


////////////////////////////////////////////////////////////////////
//          LOOP
////////////////////////////////////////////////////////////////////
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();

  // na svakih 15 min
  long now = millis();
  if (now - lastMsg > 900000) {
     lastMsg = now;
     int chk = DHT.read11(DHT11_PIN);
     
     String temperature = "";
     temperature = temperature + DHT.temperature;
     
     String humidity = "";
     humidity = humidity + DHT.humidity;
     
     char message[58];
     temperature.toCharArray(message,58);
     Serial.println(message);
     client.publish("bozaSub/temp", message, false);

     humidity.toCharArray(message,58);
     Serial.println(message);
     client.publish("bozaSub/humidity", message, false);
     
     // insert to db
     HTTPClient http;  
     String postData;

     Serial.println("temperature=" + temperature);
    
     http.begin("http://192.168.0.100/smart_home_temperature/insert.php");       
     http.addHeader("Content-Type", "application/x-www-form-urlencoded");  
 
     int httpCode = http.POST("temperature=" + temperature);   
     String payload = http.getString(); 
 
     Serial.println(httpCode);
     Serial.println(payload);   
 
     http.end();
  }
}
