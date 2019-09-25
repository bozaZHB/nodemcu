#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <OneWire.h>
#include <time.h>

IRsend irsend(14); //za IR D5

const char* ssid = "DB Automatika";//e665e4 ZHB_WiFi
const char* password = "262751838";//262751838 12345678
const char* mqtt_server = "192.168.0.100"; //iot.eclipse.org 192.168.0.200

WiFiClient espClient;
PubSubClient client(espClient);

//za serijsko citanje
int incomingByte;  
char Stanje[8];
//NTP
int timezone = 2 * 3600;
int dst = 0;
//timer
long lastMsg = 0;//podeseno na minut 60000
byte timer = 0;
String vremeTajmera1 = "06:00";
String vremeTajmera2 = "19:00";
bool poslato = false;
String trajanje = "10";//difoltno vreme

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
  if (str =="bozaSub/kuca/node1/prskalica1"){
    if(p=='1')Serial.print('q');
    else if(p=='0')Serial.print('w');
  }
  else if (str =="bozaSub/kuca/node1/ping"){
    client.publish("bozaSub/kuca/node1/ping/stanje", String("1").c_str(),false);
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
    if(p=='s'){
      client.publish("bozaSub/kuca/node1/stanje/stanje", Stanje,false);
      delay(20);
      client.publish("bozaSub/kuca/node1/vreme1/stanje", String(vremeTajmera1).c_str(),false);
      delay(20);
      client.publish("bozaSub/kuca/node1/vreme2/stanje", String(vremeTajmera2).c_str(),false);
      delay(20);
      client.publish("bozaSub/kuca/node1/trajanje/stanje", String(trajanje).c_str(),false);
    }
  }
  else if (str =="bozaSub/kuca/node1/prskalicaAll"){
    if(p=='1')Serial.write('h');
    else if(p=='0')Serial.write('j');
  }
  else if (str =="bozaSub/kuca/node1/timer"){
    if (p=='0') {
      timer = 0;
      Stanje[7]='0';
    }
    else if(p=='1') {
      timer = 1;
      Stanje[7]='1';
    }
  }
  else if (str=="bozaSub/kuca/node1/vreme1"){
    vremeTajmera1 = String((char)payload[0])+String((char)payload[1])+String((char)payload[2])+String((char)payload[3])+String((char)payload[4]);
  }
  else if (str=="bozaSub/kuca/node1/vreme2"){
    vremeTajmera2 = String((char)payload[0])+String((char)payload[1])+String((char)payload[2])+String((char)payload[3])+String((char)payload[4]);
  }
  else if (str=="bozaSub/kuca/node1/trajanje"){
    if(p=='1') Serial.write('1');//10
    else if(p=='2') Serial.write('2');//15
    else if(p=='3')Serial.write('3');//20 minuta
  }
}

void reconnect() {
  while (!client.connected())
  {
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      client.subscribe("bozaSub/kuca/node1/#");
    } else {
      delay(5000);
    }
  }
  provera();
} 

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
//IR Led
  irsend.begin();

  Stanje[0]='0';
  Stanje[1]='0';
  Stanje[2]='0';
  Stanje[3]='0';
  Stanje[4]='0';
  Stanje[5]='0';
  Stanje[6]='0';
  Stanje[7]='0';

  //NTP
  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  while(!time(nullptr)){
     delay(1000);
  }

  provera();
}
void provera(){
  Serial.print('k');//provera za prskalicu... Povratna informacija k l
  Serial.print('l');//provera za pumpu fontane... Povratna informacija z x
  Serial.print('z');//provera za radio ... Povratna informacija c v
  Serial.print('x');//provera za neodredjeno... Povratna informacija b n 
  Serial.print('m');//provera za trajanje, difoltno je 10minuta... Povratna inforamcija 1 2 3

}
void loop() {    
    if (!client.connected()) {
      reconnect();
    }

    client.setCallback(callback);
    client.loop();

  if (Serial.available() > 0) {
    char Str[2];
    incomingByte = Serial.read();
    if (incomingByte == 'q') {
      Stanje[0]='1';
      Stanje[1]='0';
      Stanje[2]='0';
      Stanje[3]='0';
      
      client.publish("bozaSub/kuca/node1/prskalica1/stanje", String("1").c_str(),false);
    }
    else if (incomingByte == 'w') {
      Stanje[0]='0';
      Stanje[1]='0';
      Stanje[2]='0';
      Stanje[3]='0';
      client.publish("bozaSub/kuca/node1/prskalica1/stanje", String("0").c_str(),false);
    }
    else if (incomingByte == 'e') {
      Stanje[0]='0';
      Stanje[1]='1';
      Stanje[2]='0';
      Stanje[3]='0';
      client.publish("bozaSub/kuca/node1/prskalica2/stanje", String("1").c_str(),false);
    }
    else if (incomingByte == 'r') {
      Stanje[0]='0';
      Stanje[1]='0';
      Stanje[2]='0';
      Stanje[3]='0';
      client.publish("bozaSub/kuca/node1/prskalica2/stanje", String("0").c_str(),false);
    }
    else if (incomingByte == 't') {
      Stanje[0]='0';
      Stanje[1]='0';
      Stanje[2]='1';
      Stanje[3]='0';
      client.publish("bozaSub/kuca/node1/prskalica3/stanje", String("1").c_str(),false);
    }
    else if (incomingByte == 'y') {
      Stanje[0]='0';
      Stanje[1]='0';
      Stanje[2]='0';
      Stanje[3]='0';
      client.publish("bozaSub/kuca/node1/prskalica3/stanje", String("0").c_str(),false);
    }
    else if (incomingByte == 'u') {
      Stanje[0]='0';
      Stanje[1]='0';
      Stanje[2]='0';
      Stanje[3]='1';
      client.publish("bozaSub/kuca/node1/prskalica4/stanje", String("1").c_str(),false);
    }
    else if (incomingByte == 'i') {
      Stanje[0]='0';
      Stanje[1]='0';
      Stanje[2]='0';
      Stanje[3]='0';
      client.publish("bozaSub/kuca/node1/prskalica4/stanje", String("0").c_str(),false);
    }
    else if (incomingByte == 'o') {
      Stanje[4]='1';
      client.publish("bozaSub/kuca/node1/fontanaPumpa/stanje", String("1").c_str(),false);
    }
    else if (incomingByte == 'p') {
      Stanje[4]='0';
      client.publish("bozaSub/kuca/node1/fontanaPumpa/stanje", String("0").c_str(),false);
    }
    else if (incomingByte == 'a') {
      Stanje[5]='1';
      client.publish("bozaSub/kuca/node1/radio/stanje", String("1").c_str(),false);
    }
    else if (incomingByte == 's') {
      Stanje[5]='0';
      client.publish("bozaSub/kuca/node1/radio/stanje", String("0").c_str(),false);
    }
    else if (incomingByte == 'd') {
      Stanje[6]='1';
      client.publish("bozaSub/kuca/node1/neodredjeno/stanje", String("1").c_str(),false);
    }
    else if (incomingByte == 'f') {
      Stanje[6]='0';
      client.publish("bozaSub/kuca/node1/neodredjeno/stanje", String("0").c_str(),false);
    }
    else if (incomingByte == 'h') {
      client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("1").c_str(),false);
    }
    else if (incomingByte == 'j') {
      Stanje[0]='0';
      Stanje[1]='0';
      Stanje[2]='0';
      Stanje[3]='0';
      client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false);
    }
    else if (incomingByte == 'k') {
      Stanje[0]='1';
      Stanje[1]='1';
      Stanje[2]='1';
      Stanje[3]='1';
    }
    else if (incomingByte == 'l') {
      Stanje[0]='0';
      Stanje[1]='0';
      Stanje[2]='0';
      Stanje[3]='0';
    }
    else if (incomingByte == 'z') {
      Stanje[4]='1';
    }
    else if (incomingByte == 'x') {
      Stanje[4]='0';
    }
    else if (incomingByte == 'c') {
      Stanje[5]='1';
    }
    else if (incomingByte == 'v') {
      Stanje[5]='0';
    }
    else if (incomingByte == 'b') {
      Stanje[6]='1';
    }
    else if (incomingByte == 'n') {
      Stanje[6]='0';
    }
    else if (incomingByte == '1') {
      trajanje = "10";
      client.publish("bozaSub/kuca/node1/trajanje/stanje", String(trajanje).c_str(),false);
    }
    else if (incomingByte == '2') {
      trajanje = "15";
      client.publish("bozaSub/kuca/node1/trajanje/stanje", String(trajanje).c_str(),false);
    }
    else if (incomingByte == '3') {
      trajanje = "20";
      client.publish("bozaSub/kuca/node1/trajanje/stanje", String(trajanje).c_str(),false);
    }
  }


//timer
  long now = millis();
  if (now - lastMsg > 30000) {
    if(timer==1){
      time_t now = time(nullptr);
      struct tm* p_tm = localtime(&now);
      String minuta = String(p_tm->tm_min);
      String sati = String(p_tm->tm_hour);
      if(minuta.length()==1) minuta = "0"+minuta;
      if(sati.length()==1) sati = "0"+sati;
      String vremeTrenutno = sati+":"+minuta;
      if(vremeTajmera1==vremeTrenutno || vremeTajmera2==vremeTrenutno){
        if(!poslato){
          Serial.write('h');
          poslato=true;
        }
      }
      else poslato = false;
    }
    lastMsg = now;
    }
}






















