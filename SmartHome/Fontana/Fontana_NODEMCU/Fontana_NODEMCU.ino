#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <OneWire.h>

IRsend irsend(14); //za IR

const char* ssid = "e665e4";
const char* password = "262751838";
const char* mqtt_server = "iot.eclipse.org";

WiFiClient espClient;
PubSubClient client(espClient);

//za serijsko citanje
int incomingByte;  

void setup_wifi() {
    delay(1000);
    //Serial.print("Connecting to ");
    //Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED){
      delay(500);
      //Serial.print(".");
    }

    
    randomSeed(micros());
    //Serial.println("");
    //Serial.println("WiFi connected");
    //Serial.println("IP address: ");
    //Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  //Serial.print("Command from MQTT broker is : [");
  //Serial.print(topic);
  char p =(char)payload[0];//-'0'

  String str(topic);
  if (str =="bozaSub/kuca/node1/prskalica1"){
    if(p=='1')Serial.print('q');
    else if(p=='0')Serial.print('w');
  }
  else if (str =="bozaSub/kuca/node1/prskalica2"){
    if(p=='1')Serial.print('e');
    else if(p=='0')Serial.print('r');
  }
  else if (str =="bozaSub/kuca/node1/prskalica3"){
    if(p=='1')Serial.print('t');
    else if(p=='0')Serial.print('y');
  }
  else if (str =="bozaSub/kuca/node1/prskalica4"){
    if(p=='1')Serial.print('u');
    else if(p=='0')Serial.print('i');
  }
  else if (str =="bozaSub/kuca/node1/fontanaPumpa"){
    if(p=='1')Serial.print('o');
    else if(p=='0')Serial.print('p');
  }
  else if (str =="bozaSub/kuca/node1/fontanaOsvetljenje"){
    switch(p){
          case '1':
          irsend.sendNEC(0xF700FF, 32);
          break;
          case '2':
          irsend.sendNEC(0xF7807F, 32);
          break;  
          case '3':
          irsend.sendNEC(0xF740BF, 32);
          break; 
          case '4':
          irsend.sendNEC(0xF7C03F, 32);
          break;
          case '5':
          irsend.sendNEC(0xF720DF, 32);
          break; 
          case '6':
          irsend.sendNEC(0xF7A05F, 32);
          break;  
          case '7':
          irsend.sendNEC(0xF7609F, 32);
          break;
          case '8':
          irsend.sendNEC(0xF7E01F, 32);
          break;
          case '9':
          irsend.sendNEC(0xF710EF, 32);
          break;
          case 'q':
          irsend.sendNEC(0xF7906F, 32);
          break;
          case 'w':
          irsend.sendNEC(0xF750AF, 32);
          break;
          case 'e':
          irsend.sendNEC(0xF7D02F, 32);
          break;
          case 'r':
          irsend.sendNEC(0xF730CF, 32);
          break;
          case 't':
          irsend.sendNEC(0xF7B04F, 32);
          break;
          case 'y':
          irsend.sendNEC(0xF7708F, 32);
          break;
          case 'u':
          irsend.sendNEC(0xF7F00F, 32);
          break;
          case 'i':
          irsend.sendNEC(0xF708F7, 32);
          break;
          case 'o':
          irsend.sendNEC(0xF78877, 32);
          break;
          case 'p':
          irsend.sendNEC(0xF748B7, 32);
          break;
          case 'a':
          irsend.sendNEC(0xF7C837, 32);
          break;
          case 's':
          irsend.sendNEC(0xF728D7, 32);
          break;
          case 'd':
          irsend.sendNEC(0xF7A857, 32);
          break;
          case 'f':
          irsend.sendNEC(0xF76897, 32);
          break;
          case 'g':
          irsend.sendNEC(0xF7E817, 32);
          break;
        }
  }
  else if (str =="bozaSub/kuca/node1/radio"){
    if(p=='1')Serial.print('a');
    else if(p=='0')Serial.print('s');
  }
  else if (str =="bozaSub/kuca/node1/neodredjeno"){
    if(p=='1')Serial.write('d');
    else if(p=='0')Serial.write('f');
  }
  else if (str =="bozaSub/kuca/node1/stanje"){
    if(p=='s')Serial.print('g');
  }
}

void reconnect() {
  while (!client.connected())
  {
    //Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      //Serial.println("connected");
      client.subscribe("bozaSub/kuca/node1/#");
    } else {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
} 

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
//IR Led
  irsend.begin();
   
}

void loop() {    
    if (!client.connected()) {
      reconnect();
    }

    client.setCallback(callback);
    client.loop();

  if (Serial.available() > 0) {
    char Str[1];
    incomingByte = Serial.read();
    if (incomingByte == 'q') {
      Str[0]='1';
      client.publish("bozaSub/kuca/node1/prskalica1/stanje", Str,TRUE);
    }
    else if (incomingByte == 'w') {
      Str[0]='0';
      client.publish("bozaSub/kuca/node1/prskalica1/stanje", Str,TRUE);
    }
    else if (incomingByte == 'e') {
      Str[0]='1';
      client.publish("bozaSub/kuca/node1/prskalica2/stanje", Str,TRUE);
    }
    else if (incomingByte == 'r') {
      Str[0]='0';
      client.publish("bozaSub/kuca/node1/prskalica2/stanje", Str,TRUE);
    }
    else if (incomingByte == 't') {
      Str[0]='1';
      client.publish("bozaSub/kuca/node1/prskalica3/stanje", Str,TRUE);
    }
    else if (incomingByte == 'y') {
      Str[0]='0';
      client.publish("bozaSub/kuca/node1/prskalica3/stanje", Str,TRUE);
    }
    else if (incomingByte == 'u') {
      Str[0]='1';
      client.publish("bozaSub/kuca/node1/prskalica4/stanje", Str,TRUE);
    }
    else if (incomingByte == 'i') {
      Str[0]='0';
      client.publish("bozaSub/kuca/node1/prskalica4/stanje", Str,TRUE);
    }
    else if (incomingByte == 'o') {
      Str[0]='1';
      client.publish("bozaSub/kuca/node1/fontanaPumpa/stanje", Str,TRUE);
    }
    else if (incomingByte == 'p') {
      Str[0]='0';
      client.publish("bozaSub/kuca/node1/fontanaPumpa/stanje", Str,TRUE);
    }
    else if (incomingByte == 'a') {
      Str[0]='1';
      client.publish("bozaSub/kuca/node1/radio/stanje", Str,TRUE);
    }
    else if (incomingByte == 's') {
      Str[0]='0';
      client.publish("bozaSub/kuca/node1/radio/stanje", Str,TRUE);
    }
    else if (incomingByte == 'd') {
      Str[0]='1';
      client.publish("bozaSub/kuca/node1/neodredjeno/stanje", Str,TRUE);
    }
    else if (incomingByte == 'f') {
      Str[0]='0';
      //mora da se vidi
      client.publish("bozaSub/kuca/node1/neodredjeno/stanje", Str,TRUE);
    }
  }

}






















