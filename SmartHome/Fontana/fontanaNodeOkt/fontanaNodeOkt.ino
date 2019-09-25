#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <OneWire.h>
#include <time.h>
#include <EEPROM.h>

IRsend irsend(14); //za IR D5

const char* ssid = "e665e4";//e665e4 ZHB_WiFi
const char* password = "262751838";//262751838 12345678
const char* mqtt_server = "192.168.0.100"; //iot.eclipse.org 192.168.0.200

WiFiClient espClient;
PubSubClient client(espClient);

char Stanje[8];

//NTP
int dst = 0;

//prskalica
volatile bool prskalica = false;
int prska = 0;
long lastMsg = 0;

//timer
long lastMsgTimer = 0;//podeseno na minut 30000
byte timer = 0;
String vremeTajmera1;
String vremeTajmera2;
bool poslato = false;
String trajanje;//difoltno vreme


//izlazi
// D0 NEODREDJENO_ULAZ
// D1 PRSKALICA1
// D2 PRSKALICA2
// D3 RADIO
// D4 PRSKALICA3
// D5 IR
// D6 PRSKALICA4
// D7 PRSKALICA_ULAZ
// D8 FONTANAPUMPA_ULAZ
// RX NEODREDJENO
// TX FONTANAPUMPA
int PRSKALICA1 = D1; 
int PRSKALICA2 = D2; 
int PRSKALICA3 = D4; 
int PRSKALICA4 = D6;
int FONTANAPUMPA = 1;//TX
int NEODREDJENO = 3;//RX
int RADIO=D3;        
//ulazi
int PRSKALICA_ULAZ=D7;
int FONTANAPUMPA_ULAZ=D8;
int NEODREDJENO_ULAZ=D0;


//tasteri

long buttonTimer1 = 0;
long buttonTimer2 = 0;
long buttonTimer3 = 0;
long buttonTimer4 = 0;
long longPressTime = 250;
boolean buttonActive1 = false;
boolean buttonActive2 = false;
boolean buttonActive3 = false;
boolean buttonActive4 = false;
boolean longPressActive1 = false;
boolean longPressActive2 = false;
boolean longPressActive3 = false;
boolean longPressActive4 = false;

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
    if(p=='1'){
      digitalWrite(PRSKALICA1, HIGH);
      client.publish("bozaSub/kuca/node1/prskalica1/stanje", String("1").c_str(),false);
    }
    else if(p=='0'){
      digitalWrite(PRSKALICA1, LOW);
      client.publish("bozaSub/kuca/node1/prskalica1/stanje", String("0").c_str(),false);
    }
    digitalWrite(PRSKALICA2, LOW);
    digitalWrite(PRSKALICA3, LOW);
    digitalWrite(PRSKALICA4, LOW);
  }
  else if (str =="bozaSub/kuca/node1/ping"){
    client.publish("bozaSub/kuca/node1/ping/stanje", String("1").c_str(),false);
  }
  
  //prskalica 2
  else if (str =="bozaSub/kuca/node1/prskalica2"){
    if(p=='1'){
      digitalWrite(PRSKALICA2, HIGH);
      client.publish("bozaSub/kuca/node1/prskalica2/stanje", String("1").c_str(),false);
    }
    else if(p=='0'){
      digitalWrite(PRSKALICA2, LOW);
      client.publish("bozaSub/kuca/node1/prskalica2/stanje", String("0").c_str(),false);
    }
    digitalWrite(PRSKALICA1, LOW);
    digitalWrite(PRSKALICA3, LOW);
    digitalWrite(PRSKALICA4, LOW);
  }

  //prskalica 3
  else if (str =="bozaSub/kuca/node1/prskalica3"){
    if(p=='1'){
      digitalWrite(PRSKALICA3, HIGH);
      client.publish("bozaSub/kuca/node1/prskalica3/stanje", String("1").c_str(),false);
    }
    else if(p=='0'){
      digitalWrite(PRSKALICA3, LOW);
      client.publish("bozaSub/kuca/node1/prskalica3/stanje", String("0").c_str(),false);
    }
    digitalWrite(PRSKALICA1, LOW);
    digitalWrite(PRSKALICA2, LOW);
    digitalWrite(PRSKALICA4, LOW);
  }

  //prskalica 4
  else if (str =="bozaSub/kuca/node1/prskalica4"){
    if(p=='1'){
      digitalWrite(PRSKALICA4, HIGH);
      client.publish("bozaSub/kuca/node1/prskalica4/stanje", String("1").c_str(),false);
    }
    else if(p=='0'){
      digitalWrite(PRSKALICA4, LOW);
      client.publish("bozaSub/kuca/node1/prskalica4/stanje", String("0").c_str(),false);
    }
    digitalWrite(PRSKALICA1, LOW);
    digitalWrite(PRSKALICA2, LOW);
    digitalWrite(PRSKALICA3, LOW);
  }

  //fontana pumpa
  else if (str =="bozaSub/kuca/node1/fontanaPumpa"){
    if(p=='1'){
      digitalWrite(FONTANAPUMPA, HIGH);
      client.publish("bozaSub/kuca/node1/fontanaPumpa/stanje", String("1").c_str(),false);
    }
    else if(p=='0'){
      digitalWrite(FONTANAPUMPA, LOW);
      client.publish("bozaSub/kuca/node1/fontanaPumpa/stanje", String("0").c_str(),false);
    }
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
    if(p=='1'){
      digitalWrite(RADIO, HIGH); 
      client.publish("bozaSub/kuca/node1/radio/stanje", String("1").c_str(),false);
    }
    else if(p=='0'){
      digitalWrite(RADIO, LOW); 
      client.publish("bozaSub/kuca/node1/radio/stanje", String("0").c_str(),false);
    }
  }
  else if (str =="bozaSub/kuca/node1/neodredjeno"){
    if(p=='1'){
      digitalWrite(NEODREDJENO, HIGH);
      client.publish("bozaSub/kuca/node1/neodredjeno/stanje", String("1").c_str(),false);
    }
    else if(p=='0'){
      digitalWrite(NEODREDJENO, LOW); 
      client.publish("bozaSub/kuca/node1/neodredjeno/stanje", String("0").c_str(),false);
    }
  }
  else if (str =="bozaSub/kuca/node1/stanje"){
    if(p=='s'){
      //kreiraj string stanja ---- prva cetiri cini prskalice, peto je
      if(digitalRead(PRSKALICA1)==HIGH) Stanje[0]='1';
      else Stanje[0]='0';
      if(digitalRead(PRSKALICA2)==HIGH) Stanje[1]='1';
      else Stanje[1]='0';
      if(digitalRead(PRSKALICA3)==HIGH) Stanje[2]='1';
      else Stanje[2]='0';
      if(digitalRead(PRSKALICA4)==HIGH) Stanje[3]='1';
      else Stanje[3]='0';
      if(digitalRead(FONTANAPUMPA)==HIGH) Stanje[4]='1';
      else Stanje[4]='0';
      Stanje[5]='0';
      if(digitalRead(NEODREDJENO)==HIGH) Stanje[6]='1';
      else Stanje[6]='0';
      if(timer==0) Stanje[7]='0';
      else Stanje[7]='1';
      
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
    if(p=='1'){
      //prskalicaAll on
      prskalica = true;
      client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("1").c_str(),false);
      lastMsg = millis();
      prska = 0;
    }
    else if(p=='0'){
      //prskalicaAll off
      prskalica = false;
      client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false);
      digitalWrite(PRSKALICA1,LOW);
      digitalWrite(PRSKALICA2,LOW);
      digitalWrite(PRSKALICA3,LOW);
      digitalWrite(PRSKALICA4,LOW);
    }
  }
  else if (str =="bozaSub/kuca/node1/timer"){
    if (p=='0') timer = 0;
    else if(p=='1') timer = 1;
  }
  else if (str=="bozaSub/kuca/node1/vreme1"){
    vremeTajmera1 = String((char)payload[0])+String((char)payload[1])+String((char)payload[2])+String((char)payload[3])+String((char)payload[4]);
    int i = 0;
    for(i=0;i<5;i++){
      EEPROM.write(i, (char)payload[i]);
      EEPROM.commit(); 
    }
  }
  else if (str=="bozaSub/kuca/node1/vreme2"){
    vremeTajmera2 = String((char)payload[0])+String((char)payload[1])+String((char)payload[2])+String((char)payload[3])+String((char)payload[4]);
    int i = 0;
    for(i=0;i<5;i++){
      EEPROM.write(i+10, (char)payload[i]);
      EEPROM.commit(); 
    }
  }
  else if (str=="bozaSub/kuca/node1/trajanje"){
    if(p=='0') trajanje = 5;
    else if(p=='1') trajanje = 10;
    else if(p=='2') trajanje = 15;
    else if(p=='3') trajanje = 20;
    EEPROM.write(21, p);
    EEPROM.commit();
    client.publish("bozaSub/kuca/node1/trajanje/stanje", String(trajanje).c_str(),false);
  }
  else if (str =="bozaSub/kuca/node1/vremeNaMikrokontroleru"){
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    String minuta = String(p_tm->tm_min);
    String sati = String(p_tm->tm_hour);
    if(minuta.length()==1) minuta = "0"+minuta;
    if(sati.length()==1) sati = "0"+sati;
    String vremeTrenutno = sati+":"+minuta;
    client.publish("bozaSub/kuca/node1/vremeNaMikrokontroleru/stanje", String(vremeTrenutno).c_str(),false);
  }
  else if (str =="bozaSub/kuca/node1/vremenskaZona"){
    EEPROM.write(20, p);
    EEPROM.commit(); 
  }
}

void reconnect() {
  if (!client.connected())
  {
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      client.subscribe("bozaSub/kuca/node1/#");
    } else {
      delay(1000);
    }
  }
} 

void setup() {
  EEPROM.begin(512);
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

  //vremena
  vremeTajmera1 = String((char)EEPROM.read(0))+String((char)EEPROM.read(1))+String((char)EEPROM.read(2))+String((char)EEPROM.read(3))+String((char)EEPROM.read(4));
  vremeTajmera2 = String((char)EEPROM.read(10))+String((char)EEPROM.read(11))+String((char)EEPROM.read(12))+String((char)EEPROM.read(13))+String((char)EEPROM.read(14));

  //trajanje
  if(EEPROM.read(21)=='0') trajanje = 5;
  else if(EEPROM.read(21)=='1') trajanje = 10;
  else if(EEPROM.read(21)=='2') trajanje = 15;
  else if(EEPROM.read(21)=='3') trajanje = 20;
  //NTP
  configTime(EEPROM.read(20)* 3600, dst, "pool.ntp.org","time.nist.gov");
  while(!time(nullptr)){
     delay(1000);
  }
  
  //inicijalizacija izlaza
  pinMode(PRSKALICA1, OUTPUT);
  pinMode(PRSKALICA2, OUTPUT);
  pinMode(PRSKALICA3, OUTPUT);
  pinMode(PRSKALICA4, OUTPUT);
  pinMode(FONTANAPUMPA, OUTPUT);
  pinMode(NEODREDJENO, OUTPUT);
  pinMode(RADIO, OUTPUT);
  
  pinMode(PRSKALICA_ULAZ, INPUT);
  pinMode(FONTANAPUMPA_ULAZ, INPUT);
  pinMode(NEODREDJENO_ULAZ, INPUT);
  
  digitalWrite(PRSKALICA1, LOW); 
  digitalWrite(PRSKALICA2, LOW); 
  digitalWrite(PRSKALICA3, LOW); 
  digitalWrite(PRSKALICA4, LOW); 
  digitalWrite(FONTANAPUMPA, LOW); 
  digitalWrite(NEODREDJENO, LOW);
  digitalWrite(RADIO, LOW); 
}

void loop() {    
    if (!client.connected()) {
      reconnect();
    }

    client.setCallback(callback);
    client.loop();

//timer
  long now = millis();
  if (now - lastMsgTimer > 30000) {
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
          prskalica = true;
          client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("1").c_str(),false);
          lastMsg = millis();
          prska = 0;
          poslato=true;
        }
      }
      else poslato = false;
    }
    lastMsgTimer = now;
    }

//prskalica
if(prskalica){
    if(prska == 0){
        digitalWrite(PRSKALICA1,HIGH);
        digitalWrite(PRSKALICA2,LOW);
        digitalWrite(PRSKALICA3,LOW);
        digitalWrite(PRSKALICA4,LOW);
        client.publish("bozaSub/kuca/node1/prskalica1/stanje", String("1").c_str(),false);
        prska++;
    }

    if(prska == 1){
      if (now- lastMsg > 60000*trajanje.toInt()) {
        digitalWrite(PRSKALICA1,LOW);
        digitalWrite(PRSKALICA2,HIGH);
        digitalWrite(PRSKALICA3,LOW);
        digitalWrite(PRSKALICA4,LOW);
        client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false); delay(20);
        client.publish("bozaSub/kuca/node1/prskalica2/stanje", String("1").c_str(),false);
        prska++;
      }
    }

    if(prska == 2){
      if (now- lastMsg > 120000*trajanje.toInt()) {
        digitalWrite(PRSKALICA1,LOW);
        digitalWrite(PRSKALICA2,LOW);
        digitalWrite(PRSKALICA3,HIGH);
        digitalWrite(PRSKALICA4,LOW);
        client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false); delay(20);
        client.publish("bozaSub/kuca/node1/prskalica3/stanje", String("1").c_str(),false);
        prska++;
      }
    }

    if(prska == 3){
      if (now- lastMsg > 180000*trajanje.toInt()) {
        digitalWrite(PRSKALICA1,LOW);
        digitalWrite(PRSKALICA2,LOW);
        digitalWrite(PRSKALICA3,LOW);
        digitalWrite(PRSKALICA4,HIGH);
        client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false); delay(20);
        client.publish("bozaSub/kuca/node1/prskalica4/stanje", String("1").c_str(),false);
        prska++;
      }
    }
    
    if(prska == 4){
      if (now- lastMsg > 240000*trajanje.toInt()) {
        lastMsg = now;
        client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false); delay(20);
        prska=0;
        prskalica = false;
        digitalWrite(PRSKALICA1,LOW);
        digitalWrite(PRSKALICA2,LOW);
        digitalWrite(PRSKALICA3,LOW);
        digitalWrite(PRSKALICA4,LOW);
      }
    }
  }

//prskalica
  if (digitalRead(PRSKALICA_ULAZ) == HIGH) {
    if (buttonActive1 == false) {
      buttonActive1 = true;
      buttonTimer1 = millis();
    }
    if ((millis() - buttonTimer1 > longPressTime) && (longPressActive1 == false)) {
      longPressActive1 = true;   
        if(digitalRead(PRSKALICA1) == LOW && digitalRead(PRSKALICA2) == LOW && digitalRead(PRSKALICA3) == LOW && digitalRead(PRSKALICA4) == LOW){
          prskalica = true;
          client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("1").c_str(),false); delay(50);
          lastMsg = millis();
          prska = 0;
        }
        else{
          prskalica = false;
          client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false); delay(50);
          digitalWrite(PRSKALICA1,LOW);
          digitalWrite(PRSKALICA2,LOW);
          digitalWrite(PRSKALICA3,LOW);
          digitalWrite(PRSKALICA4,LOW);
        }
      }
  }else {
    buttonActive1 = false;
    longPressActive1 = false;
  }

  
//taster za neodredjeno
  if (digitalRead(NEODREDJENO_ULAZ) == HIGH) {
    if (buttonActive4 == false) {
      buttonActive4 = true;
      buttonTimer4 = millis();
    }
    if ((millis() - buttonTimer4 > longPressTime) && (longPressActive4 == false)) {
      longPressActive4 = true;   
        if(digitalRead(NEODREDJENO)==LOW){
          client.publish("bozaSub/kuca/node1/neodredjeno/stanje", String("1").c_str(),false);
          digitalWrite(NEODREDJENO, HIGH);
        }
        else{
          client.publish("bozaSub/kuca/node1/neodredjeno/stanje", String("0").c_str(),false);
          digitalWrite(NEODREDJENO, LOW);
        }
      }
  }else {
    buttonActive4 = false;
    longPressActive4 = false;
  }

//taster za fontanu pumpe
  if (digitalRead(FONTANAPUMPA_ULAZ) == HIGH) {
    if (buttonActive2 == false) {
      buttonActive2 = true;
      buttonTimer2 = millis();
    }
    if ((millis() - buttonTimer2 > longPressTime) && (longPressActive2 == false)) {
      longPressActive2 = true;   
        if(digitalRead(FONTANAPUMPA)==LOW){
          client.publish("bozaSub/kuca/node1/fontanaPumpa/stanje", String("1").c_str(),false);
          digitalWrite(FONTANAPUMPA, HIGH);
        }
        else{
          client.publish("bozaSub/kuca/node1/fontanaPumpa/stanje", String("0").c_str(),false);
          digitalWrite(FONTANAPUMPA, LOW);
        }
      }
  }else {
    buttonActive2 = false;
    longPressActive2 = false;
  }
}
