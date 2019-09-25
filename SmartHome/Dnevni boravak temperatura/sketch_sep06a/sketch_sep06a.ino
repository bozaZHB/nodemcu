/*
+temperatura
+vlaznost
daljinski
tasteri
reley
static const  D0   = 16;
static const  D1   = 5;
static const  D2   = 4;
static const  D3   = 0;
static const  D4   = 2;
static const  D5   = 14;
static const  D6   = 12;
static const  D7   = 13;
static const  D8   = 15;
static const  D9   = 3;
static const  D10  = 1;

*/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <dht.h>
#include <IRremoteESP8266.h>
dht DHT;

#define DHT11_PIN D1 // Define NodeMCU D3 pin to as temperature data pin of  DHT11
int RECV_PIN = D2; //an IR detector/demodulatord is connected to GPIO pin D2

const char* ssid = "e665e4";
const char* password = "262751838";
const char* mqtt_server = "192.168.0.100";

WiFiClient espClient;
PubSubClient client(espClient);
IRrecv irrecv(RECV_PIN);
decode_results results;

long lastMsgTemp = 0;
long lastMsgIr = 0;
int value = 0;
char msg[50];

bool sijalicaJedan = false;
bool sijalicaDva = false;
bool sijalicaTri = false;
bool sijalicaCetiri = false;

bool ukljuceno = false;

int SIJALICA1 = D0;
int SIJALICA2 = D3;
int SIJALICA3 = D4;
int SIJALICA4 = D5;

int PREKIDAC1 = D6;
int PREKIDAC2 = D7;
int PREKIDAC3 = D8;
int PREKIDAC4 = 3;

long longPressTime = 250;

long buttonTimer1 = 0;
boolean buttonActive1 = false;
boolean longPressActive1 = false;

long buttonTimer2 = 0;
boolean buttonActive2 = false;
boolean longPressActive2 = false;

long buttonTimer3 = 0;
boolean buttonActive3 = false;
boolean longPressActive3 = false;

long buttonTimer4 = 0;
boolean buttonActive4 = false;
boolean longPressActive4 = false;

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
     //temperatura
     int chk = DHT.read11(DHT11_PIN);
     String msg="";
     msg= msg+ DHT.temperature;
     char message[58];
     msg.toCharArray(message,58);
     Serial.println(message);
     client.publish("bozaSub/kuca/node3/dnevniBoravak/temperatura", message);
     //vlaznost
     msg="";
     msg=msg+DHT.humidity ;
     char message2[58];
     msg.toCharArray(message2,58);
     Serial.println(message2);
     client.publish("bozaSub/kuca/node3/dnevniBoravak/vlaznost", message2);

     if(!digitalRead(SIJALICA1)) client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica1/stanje", String("1").c_str(),false);
     else client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica1/stanje", String("0").c_str(),false);
     if(!digitalRead(SIJALICA2)) client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica2/stanje", String("1").c_str(),false);
     else client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica2/stanje", String("0").c_str(),false);
     if(!digitalRead(SIJALICA3)) client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica3/stanje", String("1").c_str(),false);
     else client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica3/stanje", String("0").c_str(),false);
     if(!digitalRead(SIJALICA4)) client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica4/stanje", String("1").c_str(),false);
     else client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica4/stanje", String("0").c_str(),false);
    }
  }
  else if (str =="bozaSub/kuca/node3/dnevniBoravak/sijalica1"){
    if(p=='1'){
      digitalWrite(SIJALICA1,ukljuceno);
      client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica1/stanje", String("1").c_str(),false);
    }
    else if(p=='0'){
      digitalWrite(SIJALICA1,!ukljuceno);
      client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica1/stanje", String("0").c_str(),false);
    }
  }
  else if (str =="bozaSub/kuca/node3/dnevniBoravak/sijalica2"){
    if(p=='1'){
      digitalWrite(SIJALICA2,ukljuceno);
      client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica2/stanje", String("1").c_str(),false);
    }
    else if(p=='0'){
      digitalWrite(SIJALICA2,!ukljuceno);
      client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica2/stanje", String("0").c_str(),false);
    }
  }
  else if (str =="bozaSub/kuca/node3/dnevniBoravak/sijalica3"){
    if(p=='1'){
      digitalWrite(SIJALICA3,ukljuceno);
      client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica3/stanje", String("1").c_str(),false);
    }
    else if(p=='0'){
      digitalWrite(SIJALICA3,!ukljuceno);
      client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica3/stanje", String("0").c_str(),false);
    }
  }
  else if (str =="bozaSub/kuca/node3/dnevniBoravak/sijalica4"){
    if(p=='1'){
      digitalWrite(SIJALICA4,ukljuceno);
      client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica4/stanje", String("1").c_str(),false);
    }
    else if(p=='0'){
      digitalWrite(SIJALICA4,!ukljuceno);
      client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica4/stanje", String("0").c_str(),false);
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
  
  irrecv.enableIRIn(); // Start the receiver

  pinMode(SIJALICA1, OUTPUT);
  pinMode(SIJALICA2, OUTPUT);
  pinMode(SIJALICA3, OUTPUT);
  pinMode(SIJALICA4, OUTPUT);
  
  digitalWrite(SIJALICA1,!ukljuceno);
  digitalWrite(SIJALICA2,!ukljuceno);
  digitalWrite(SIJALICA3,!ukljuceno);
  digitalWrite(SIJALICA4,!ukljuceno);

  pinMode(PREKIDAC1, INPUT);
  pinMode(PREKIDAC2, INPUT);
  pinMode(PREKIDAC3, INPUT);
  pinMode(PREKIDAC4, INPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //slanje na 30s temperature i vlaznsot dnevnog boravka
  long now = millis();
  if (now - lastMsgTemp > 30000) {
    //slanje temperatura
    lastMsgTemp = now;
    int chk = DHT.read11(DHT11_PIN);
    String msg="";
    msg= msg+ DHT.temperature;
    char message[58];
    msg.toCharArray(message,58);
    Serial.println(message);
    client.publish("bozaSub/kuca/node3/dnevniBoravak/temperatura", message,false);
    //vlaznost
    msg="";
    msg=msg+DHT.humidity ;
    char message2[58];
    msg.toCharArray(message2,58);
    Serial.println(message2);
    client.publish("bozaSub/kuca/node3/dnevniBoravak/vlaznost", message2,false);
  }

  //IR 
  if (now - lastMsgIr > 1000) {
    lastMsgIr = now;
    if (irrecv.decode(&results)) 
    {
      Serial.println(results.value, HEX);
      irrecv.resume(); // Receive the next value
      String msg=""; msg= msg+ results.value;
      char message[58];
      msg.toCharArray(message,58);
      Serial.println(message);
      if(msg == "2160038254"){
        digitalWrite(SIJALICA1,!digitalRead(SIJALICA1));
        if(!digitalRead(SIJALICA1)) client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica1/stanje", String("1").c_str(),false);
        else client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica1/stanje", String("0").c_str(),false);
      }
      else if(msg == "2160009694"){
        digitalWrite(SIJALICA2,!digitalRead(SIJALICA2));
        if(!digitalRead(SIJALICA2)) client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica2/stanje", String("1").c_str(),false);
        else client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica2/stanje", String("0").c_str(),false);
      }
      else if(msg == "2160040804"){
        digitalWrite(SIJALICA3,!digitalRead(SIJALICA3));
        if(!digitalRead(SIJALICA3)) client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica3/stanje", String("1").c_str(),false);
        else client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica3/stanje", String("0").c_str(),false);
      }
      else if(msg == "2160028054"){
        digitalWrite(SIJALICA4,!digitalRead(SIJALICA4));
        if(!digitalRead(SIJALICA4)) client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica4/stanje", String("1").c_str(),false);
        else client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica4/stanje", String("0").c_str(),false);
      }  
    }
  }
  
  //longClick taster za 3 sijalice
  if (digitalRead(PREKIDAC1) == HIGH) {
    if (buttonActive1 == false) {
      buttonActive1 = true;
      buttonTimer1 = millis();
    }
    if ((millis() - buttonTimer1 > longPressTime) && (longPressActive1 == false)) {
      longPressActive1 = true;   
      digitalWrite(SIJALICA1,!digitalRead(SIJALICA1));
      if(!digitalRead(SIJALICA1)) client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica1/stanje", String("1").c_str(),false);
      else client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica1/stanje", String("0").c_str(),false);
    }
  }else {
    buttonActive1 = false;
    longPressActive1 = false;
  }

    if (digitalRead(PREKIDAC2) == HIGH) {
    if (buttonActive2 == false) {
      buttonActive2 = true;
      buttonTimer2 = millis();
    }
    if ((millis() - buttonTimer2 > longPressTime) && (longPressActive2 == false)) {
      longPressActive2 = true;   
      digitalWrite(SIJALICA2,!digitalRead(SIJALICA2));
      if(!digitalRead(SIJALICA2)) client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica2/stanje", String("1").c_str(),false);
      else client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica2/stanje", String("0").c_str(),false);
    }
  }else {
    buttonActive2 = false;
    longPressActive2 = false;
  }

    if (digitalRead(PREKIDAC3) == HIGH) {
    if (buttonActive3 == false) {
      buttonActive3 = true;
      buttonTimer3 = millis();
    }
    if ((millis() - buttonTimer3 > longPressTime) && (longPressActive3 == false)) {
      longPressActive3 = true;   
      digitalWrite(SIJALICA3,!digitalRead(SIJALICA3));
      if(!digitalRead(SIJALICA3)) client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica3/stanje", String("1").c_str(),false);
      else client.publish("bozaSub/kuca/node3/dnevniBoravak/sijalica3/stanje", String("0").c_str(),false);
    }
  }else {
    buttonActive3 = false;
    longPressActive3 = false;
  }

}
