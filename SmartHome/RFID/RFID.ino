#define SS_PIN D0  //D2
#define RST_PIN D1 //D1

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "DB Automatika";
const char* password = "262751838";
const char* mqtt_server = "192.168.0.100";

WiFiClient espClient;
PubSubClient client(espClient);

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;

int VRATA = D2; 

String tag1 = "C5 2A 01 14";
String tag2 = "C5 2A 01 14";
String tag3 = "C5 2A 01 14";
String tag4 = "C5 2A 01 14";
String tag5 = "C5 2A 01 14";
String tag6 = "C5 2A 01 14";
String tag7 = "C5 2A 01 14";
String tag8 = "C5 2A 01 14";
String tag9 = "C5 2A 01 14";
String tag10 = "C5 2A 01 14";

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
  char p =(char)payload[0];//-'0'
  String str(topic);
  
  if (str =="bozaSub/kuca/node3/vrata"){
    if(p=='1'){
      digitalWrite(VRATA,HIGH);
      client.publish("bozaSub/kuca/node3/vrata/stanje", String("1").c_str(), false);
      delay(5000);
      digitalWrite(VRATA,LOW);
      client.publish("bozaSub/kuca/node3/vrata/stanje", String("0").c_str(), false);
    }
    else if(p=='0'){
      if(digitalRead(VRATA)==HIGH){
        digitalWrite(VRATA,LOW);
        client.publish("bozaSub/kuca/node3/vrata/stanje", String("0").c_str(), false);
      }
    }
  }
  else if (str =="bozaSub/kuca/node1/stanje"){
    if(p=='s'){
      if(digitalRead(VRATA)==HIGH)client.publish("bozaSub/kuca/node3/vrata/stanje", String("1").c_str(), false);
      else client.publish("bozaSub/kuca/node3/vrata/stanje", String("0").c_str(), false);
    }
  }
  else if (str =="bozaSub/kuca/node3/tag1/stanje"){
    if(p=='s'){
      client.publish("bozaSub/kuca/node3/vrata/stanje", String(tag1).c_str(), false);
    }
  }
  else if (str =="bozaSub/kuca/node3/tag2"){
    char msg[15];
    for(int i=0;i<length;i++)
    {
      Serial.print((char)payload[i]);  
      msg[i]=(char)payload[i];
    }
  }
} 

void reconnect() {
  while (!client.connected())
  {
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      client.subscribe("bozaSub/kuca/#");
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

//rfid
  Serial.begin(9600);   
  SPI.begin();     
  mfrc522.PCD_Init();   

  //izlaz
  pinMode(VRATA,OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  
  if ( ! mfrc522.PICC_IsNewCardPresent()) return;
  if ( ! mfrc522.PICC_ReadCardSerial()) return;
  
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == tag1 || content.substring(1) == tag2 || content.substring(1) == tag3 ||content.substring(1) == tag4 || content.substring(1) == tag5 || content.substring(1) == tag6 || content.substring(1) == tag7 || content.substring(1) == tag8 || content.substring(1) == tag9 || content.substring(1) == tag10) 
  {
    Serial.println(" Access Granted ");
    Serial.println();
    statuss = 1;
    client.publish("bozaSub/kuca/node3/vrata/stanje", String("1").c_str(), false);
    digitalWrite(VRATA,HIGH);
    delay(5000);
    digitalWrite(VRATA,LOW);
    client.publish("bozaSub/kuca/node3/vrata/stanje", String("0").c_str(), false);
  }
  else
    delay(1000);
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
