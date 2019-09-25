#include <DallasTemperature.h>
#include <OneWire.h>

#include <PID_v1.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define ONE_WIRE_BUS D1
#define RELAY_PIN D2

const char* ssid = "ZHB";
const char* password = "12345678";
const char* mqtt_server = "m23.cloudmqtt.com";
const int mqttPort = 13634; // iot.eclipse.org - 1883  m23.cloudmqtt.com - 13634
const char* user= "ypdfqxdh";
const char* passw= "PuluE8hNViwT";

WiFiClient espClient;
PubSubClient client(espClient);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

double Setpoint, Input, Output;

double Kp=45, Ki=0.05, Kd=0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int WindowSize = 1200000;
unsigned long windowStartTime;

void setup_wifi() {
   delay(100);
   Serial.print("Connecting to ");Serial.println(ssid);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) delay(500);
   randomSeed(micros());
   Serial.println("");Serial.println("WiFi connected");Serial.println("IP address: "); Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  //client.publish("zhb/zhb", String("eto ga").c_str(), false);
  //byteToString(payload, length)
  char p =(char)payload[0];
  String strTopic(topic);
  String setValueAndroid = String((char)payload[0])+String((char)payload[1]);
  Serial.println(strTopic);
  if (strTopic =="zhb/set"){
    Serial.println(setValueAndroid);
    Setpoint = setValueAndroid.toInt();
  }
}

void reconnect() {
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(),user,passw))
    {
      Serial.println("connected");
      client.subscribe("zhb/set");
    } else {
      Serial.print("failed, rc="); Serial.print(client.state());
      delay(2000);
    }
  }
} 

void setup(){

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  Serial.begin(115200);
  Serial.println("Starting");
  windowStartTime = millis();

  Setpoint = 30.0;

  myPID.SetOutputLimits(0, WindowSize);
  myPID.SetMode(AUTOMATIC);

  setup_wifi();
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);
}

void loop(){

  if (!client.connected()) reconnect();
  client.loop();
  
  sensors.requestTemperatures();
  Input = sensors.getTempCByIndex(0);
  client.publish("zhb/temperature", String(Input).c_str(), false);
  Serial.print("Temperature: ");
  Serial.println(Input);
  myPID.Compute();
  
  Serial.print("          Setpoint: ");
  Serial.println(Setpoint);
  if (millis() - windowStartTime > WindowSize){
    windowStartTime += WindowSize;
  }

  Serial.print("OUTPUT: ");
  Serial.println(Output);

  if(((Output * WindowSize) > ((millis() - windowStartTime) * 100)) && Output > 20 ) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("HIGH ");
    client.publish("zhb/status", String("1").c_str(), false);
  }
  else {
    Serial.println("LOW ");
    digitalWrite(RELAY_PIN, LOW);
    client.publish("zhb/status", String("0").c_str(), false);
  }

  Serial.println("....................................................................");
  delay(2000);
  
}
