/*
 * NodeMCU - Fontana
 * Smart Home BOZANIC
 * Create by Bozanic Stefan ZHB
 * 5. july 2019 last update
 * 
 *  8 RELEY (6 x PRSKALICE, 1 x FONTANA PUMPA, 1 x NEODREDJENO)
 *  2 x INPUT (PRSKALICA SVE & FONTANA PUMPA)
 *  1 x IR
 *  EEPROM
 *  NTP
 *  2 x TIMER FOR PRSKALICA
 *  1 x TRAJANJE PRSKALICE
 *  
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>

#include <time.h>
#include <EEPROM.h>

IRsend irsend(14); //za IR D5

const char* ssid = "e665e4";
const char* password = "262751838";
const char* mqttServer = "192.168.0.100"; //iot.eclipse.org 192.168.0.200
const int mqttPort = 1883; // iot.eclipse.org - 1883  m23.cloudmqtt.com - 13634
const char* user= "ypdfqxdh";
const char* passw= "PuluE8hNViwT";

WiFiClient espClient;
PubSubClient client(espClient);

char Stanje[9];

//NTP
int dst = 0;

//prskalica
volatile bool prskalica = false;
volatile bool prskalica1 = false;
volatile bool prskalica2 = false;
volatile bool prskalica3 = false;
volatile bool prskalica4 = false;
volatile bool prskalica5 = false;
volatile bool prskalica6 = false;
int prska = 0;
long lastMsg = 0;

//timer
long lastMsgTimer = 0; // podeseno na minut 30000
byte timer = 0;
String vremeTajmera1;
String vremeTajmera2;
bool poslato = false;
String trajanje; // difoltno vreme

bool ukljuceno = false;

//izlazi
// D0 NEODREDJENO_ULAZ   -> PRSKALICA 6  -> na kraju nije nista
// D1 PRSKALICA1
// D2 PRSKALICA2
// D3 RADIO             -> PRSKALICA 5
// D4 PRSKALICA3
// D5 IR
// D6 PRSKALICA4
// D7 PRSKALICA_ULAZ
// D8 FONTANAPUMPA_ULAZ
// RX NEODREDJENO       -> PRSKALICA 6
// TX FONTANAPUMPA
int PRSKALICA1 = D1; 
int PRSKALICA2 = D2; 
int PRSKALICA3 = D4; 
int PRSKALICA4 = D6;
int PRSKALICA5 = D3;     // ranije bilo radio -> PRSKALICA5 ulaz
int PRSKALICA6 = 3;     // ranije bilo NEODREDJENO -> PRSKALICA6 ulaz

int FONTANAPUMPA = 1;    //TX
//int NEODREDJENO = 3;     //RX -> vise nije u funkciji sa prskalicama 5 i 6
   
//ulazi
int PRSKALICA_ULAZ = D7;
int FONTANAPUMPA_ULAZ = D8;

// tasteri
long buttonTimer1 = 0;   // odnosi se na sve prkalice
long buttonTimer2 = 0;   // odnosi se na pumpu fontane
long longPressTime = 250;
boolean buttonActive1 = false;
boolean buttonActive2 = false;
boolean longPressActive1 = false;
boolean longPressActive2 = false;

////////////////////////////////////////////////////////////////////
//          SETUP_WIFI
///////////////////////////////////////////////////////////////////
void setup_wifi() {
   delay(100);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) delay(500);
   randomSeed(micros());
}

////////////////////////////////////////////////////////////////////
//          CALLBACK
///////////////////////////////////////////////////////////////////
void callback(char* topic, byte* payload, unsigned int length) 
{
  char p =(char)payload[0];//-'0'

  String str(topic);

  // on/off prkalica 1 tajmer
  if (str =="bozaSub/kuca/node1/prskalica1"){
    if(p == '1'){
      //prskalica 1 on
      lastMsg = millis();
      prskalica1 = true;
      client.publish("bozaSub/kuca/node1/prskalica1/stanje", String("1").c_str(),false);
      digitalWrite(PRSKALICA1,ukljuceno);
    }
    else if(p == '0'){
      //prskalica 1 off
      prskalica1 = false;
      client.publish("bozaSub/kuca/node1/prskalica1/stanje", String("0").c_str(),false);
      digitalWrite(PRSKALICA1,!ukljuceno);      digitalWrite(PRSKALICA2,!ukljuceno);      digitalWrite(PRSKALICA3,!ukljuceno);
      digitalWrite(PRSKALICA4,!ukljuceno);      digitalWrite(PRSKALICA5,!ukljuceno);      digitalWrite(PRSKALICA6,!ukljuceno);
    }
  }

  // on/off prkalica 2 tajmer
  else if (str =="bozaSub/kuca/node1/prskalica2"){
    if(p == '1'){
      //prskalica 2 on
      lastMsg = millis();
      prskalica2 = true;
      client.publish("bozaSub/kuca/node1/prskalica2/stanje", String("1").c_str(),false);
      digitalWrite(PRSKALICA2,ukljuceno);
    }
    else if(p == '0'){
      //prskalica 2 off
      prskalica2 = false;
      client.publish("bozaSub/kuca/node1/prskalica2/stanje", String("0").c_str(),false);
      digitalWrite(PRSKALICA1,!ukljuceno);      digitalWrite(PRSKALICA2,!ukljuceno);      digitalWrite(PRSKALICA3,!ukljuceno);      
      digitalWrite(PRSKALICA4,!ukljuceno);      digitalWrite(PRSKALICA5,!ukljuceno);      digitalWrite(PRSKALICA6,!ukljuceno);
    }
  }

  // on/off prkalica 3 tajmer
  else if (str =="bozaSub/kuca/node1/prskalica3"){
    if(p == '1'){
      //prskalica 3 on
      lastMsg = millis();
      prskalica3 = true;
      client.publish("bozaSub/kuca/node1/prskalica3/stanje", String("1").c_str(),false);
      digitalWrite(PRSKALICA3,ukljuceno);
    }
    else if(p == '0'){
      //prskalica 3 off
      prskalica3 = false;
      client.publish("bozaSub/kuca/node1/prskalica3/stanje", String("0").c_str(),false);
      digitalWrite(PRSKALICA1,!ukljuceno);      digitalWrite(PRSKALICA2,!ukljuceno);      digitalWrite(PRSKALICA3,!ukljuceno);      
      digitalWrite(PRSKALICA4,!ukljuceno);      digitalWrite(PRSKALICA5,!ukljuceno);      digitalWrite(PRSKALICA6,!ukljuceno);
    }
  }

  // on/off prkalica 4 tajmer
  else if (str =="bozaSub/kuca/node1/prskalica4"){
    if(p == '1'){
      //prskalica 4 on
      lastMsg = millis();
      prskalica4 = true;
      client.publish("bozaSub/kuca/node1/prskalica4/stanje", String("1").c_str(),false);
      digitalWrite(PRSKALICA4,ukljuceno);
    }
    else if(p == '0'){
      //prskalica 4 off
      prskalica4 = false;
      client.publish("bozaSub/kuca/node1/prskalica4/stanje", String("0").c_str(),false);
      digitalWrite(PRSKALICA1,!ukljuceno);      digitalWrite(PRSKALICA2,!ukljuceno);      digitalWrite(PRSKALICA3,!ukljuceno);      
      digitalWrite(PRSKALICA4,!ukljuceno);      digitalWrite(PRSKALICA5,!ukljuceno);      digitalWrite(PRSKALICA6,!ukljuceno);
    }
  }

  // on/off prkalica 5 tajmer
  else if (str =="bozaSub/kuca/node1/prskalica5"){
    if(p == '1'){
      //prskalica 5 on
      lastMsg = millis();
      prskalica5 = true;
      client.publish("bozaSub/kuca/node1/prskalica5/stanje", String("1").c_str(),false);
      digitalWrite(PRSKALICA5,ukljuceno);
    }
    else if(p == '0'){
      //prskalica 5 off
      prskalica5 = false;
      client.publish("bozaSub/kuca/node1/prskalica5/stanje", String("0").c_str(),false);
      digitalWrite(PRSKALICA1,!ukljuceno);      digitalWrite(PRSKALICA2,!ukljuceno);      digitalWrite(PRSKALICA3,!ukljuceno);      
      digitalWrite(PRSKALICA4,!ukljuceno);      digitalWrite(PRSKALICA5,!ukljuceno);      digitalWrite(PRSKALICA6,!ukljuceno);
    }
  }

  // on/off prkalica 6 tajmer
  else if (str =="bozaSub/kuca/node1/prskalica6"){
    if(p == '1'){
      //prskalica 6 on
      lastMsg = millis();
      prskalica6 = true;
      client.publish("bozaSub/kuca/node1/prskalica6/stanje", String("1").c_str(),false);
      digitalWrite(PRSKALICA6,ukljuceno);
    }
    else if(p == '0'){
      //prskalica 6 off
      prskalica6 = false;
      client.publish("bozaSub/kuca/node1/prskalica6/stanje", String("0").c_str(),false);
      digitalWrite(PRSKALICA1,!ukljuceno);      digitalWrite(PRSKALICA2,!ukljuceno);      digitalWrite(PRSKALICA3,!ukljuceno);      
      digitalWrite(PRSKALICA4,!ukljuceno);      digitalWrite(PRSKALICA5,!ukljuceno);      digitalWrite(PRSKALICA6,!ukljuceno);
    }
  }
  
  // pingovanje tj. provera konektivnosti NodeMCU
  else if (str =="bozaSub/kuca/node1/ping"){
    client.publish("bozaSub/kuca/node1/ping/stanje", String("1").c_str(),false);
  }
  
  //on/off fontana pumpa
  else if (str =="bozaSub/kuca/node1/fontanaPumpa"){
    if(p == '1'){
      digitalWrite(FONTANAPUMPA, ukljuceno);
      client.publish("bozaSub/kuca/node1/fontanaPumpa/stanje", String("1").c_str(),false);
    } else if (p == '0'){
      digitalWrite(FONTANAPUMPA, !ukljuceno);
      client.publish("bozaSub/kuca/node1/fontanaPumpa/stanje", String("0").c_str(),false);
    }
  }

  // Slanje IR za fontanu boje
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

  // provera vrednosti na NodeMCU
  else if (str =="bozaSub/kuca/node1/stanje"){
    if(p == 's'){
      
      //kreiraj string stanja ---- prva sest cini prskalice, sedmo je fontana pumpe, osmo je neodredjeno, deveto je tajmer 
      
      if(digitalRead(PRSKALICA1) == LOW) Stanje[0]='1';
      else Stanje[0]='0';
      if(digitalRead(PRSKALICA2) == LOW) Stanje[1]='1';
      else Stanje[1]='0';
      if(digitalRead(PRSKALICA3) == LOW) Stanje[2]='1';
      else Stanje[2]='0';
      if(digitalRead(PRSKALICA4) == LOW) Stanje[3]='1';
      else Stanje[3]='0';
      if(digitalRead(PRSKALICA5) == LOW) Stanje[4]='1';
      else Stanje[4]='0';
      if(digitalRead(PRSKALICA6) == LOW) Stanje[5]='1';
      else Stanje[5]='0';
      
      if(digitalRead(FONTANAPUMPA) == LOW) Stanje[6]='1';
      else Stanje[6]='0';
      
      Stanje[7]='0'; // stavljeno da je 0 jer se vremenom izbacilo
      
      if(timer == 0) Stanje[8]='0';
      else Stanje[8]='1';
      
      client.publish("bozaSub/kuca/node1/stanje/stanje", Stanje,false);
      delay(20);
      
      client.publish("bozaSub/kuca/node1/vreme1/stanje", String(vremeTajmera1).c_str(),false);
      delay(20);
      
      client.publish("bozaSub/kuca/node1/vreme2/stanje", String(vremeTajmera2).c_str(),false);
      delay(20);
      
      client.publish("bozaSub/kuca/node1/trajanje/stanje", String(trajanje).c_str(),false);
    }
  }

  // on/off sve prskalice
  else if (str =="bozaSub/kuca/node1/prskalicaAll"){
    if(p=='1'){
      //prskalicaAll on
      prskalica1 = false;      prskalica2 = false;      prskalica3 = false;
      prskalica4 = false;      prskalica5 = false;      prskalica6 = false;
      prskalica = true;
      
      client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("1").c_str(),false);
      lastMsg = millis();
      prska = 0;
    }
    else if(p=='0'){
      //prskalicaAll off
      prskalica = false;
      prskalica1 = false;      prskalica2 = false;      prskalica3 = false;
      prskalica4 = false;      prskalica5 = false;      prskalica6 = false;
      
      client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false);
      
      digitalWrite(PRSKALICA1,!ukljuceno);      digitalWrite(PRSKALICA2,!ukljuceno);      digitalWrite(PRSKALICA3,!ukljuceno);
      digitalWrite(PRSKALICA4,!ukljuceno);      digitalWrite(PRSKALICA5,!ukljuceno);      digitalWrite(PRSKALICA6,!ukljuceno);
    }
  }

  // on/off tajmer 1
  else if (str =="bozaSub/kuca/node1/timer"){
    if (p=='0') timer = 0;
    else if(p=='1') timer = 1;
  }

  // podesavanje vremena tajmera 1
  else if (str=="bozaSub/kuca/node1/vreme1"){
    vremeTajmera1 = String((char)payload[0])+String((char)payload[1])+String((char)payload[2])+String((char)payload[3])+String((char)payload[4]);
    int i = 0;
    for(i=0;i<5;i++){
      EEPROM.write(i, (char)payload[i]);
      EEPROM.commit(); 
    }
  }

  // podesavanje vremena tajmera 2
  else if (str=="bozaSub/kuca/node1/vreme2"){
    vremeTajmera2 = String((char)payload[0])+String((char)payload[1])+String((char)payload[2])+String((char)payload[3])+String((char)payload[4]);
    int i = 0;
    for(i=0;i<5;i++){
      EEPROM.write(i+10, (char)payload[i]);
      EEPROM.commit(); 
    }
  }

  // podesavanje trajanje prskalice
  else if (str=="bozaSub/kuca/node1/trajanje"){
    if(p=='0') trajanje = 1;        //5
    else if(p=='1') trajanje = 10;  //10
    else if(p=='2') trajanje = 15;  //15
    else if(p=='3') trajanje = 20;  //20
    EEPROM.write(21, p);
    EEPROM.commit();
    client.publish("bozaSub/kuca/node1/trajanje/stanje", String(trajanje).c_str(),false);
  }

  // provera vremena na NodeMCU
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

  // podesavanje vremenske zone. Mora da se restartuje NodeMCU da bi imalo efekta
  else if (str =="bozaSub/kuca/node1/vremenskaZona"){
    EEPROM.write(20, p);
    EEPROM.commit(); 
  }
}

////////////////////////////////////////////////////////////////////
//          RECONNECT
///////////////////////////////////////////////////////////////////
void reconnect() {
  while (!client.connected())
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

////////////////////////////////////////////////////////////////////
//          SETUP
///////////////////////////////////////////////////////////////////
void setup() {
  setup_wifi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  EEPROM.begin(512);
  
  //IR Led
  irsend.begin();

  Stanje[0]='0'; // PRSKALICA1
  Stanje[1]='0'; // PRSKALICA2
  Stanje[2]='0'; // PRSKALICA3
  Stanje[3]='0'; // PRSKALICA4
  Stanje[4]='0'; // PRSKALICA5
  Stanje[5]='0'; // PRSKALICA6
  Stanje[6]='0'; // FONTANAPUMPA
  Stanje[7]='0'; // NEODREDJENO
  Stanje[8]='0'; // TIMER

  // vremena procitana iz EEPROM-a
  vremeTajmera1 = String((char)EEPROM.read(0))+String((char)EEPROM.read(1))+String((char)EEPROM.read(2))+String((char)EEPROM.read(3))+String((char)EEPROM.read(4));
  vremeTajmera2 = String((char)EEPROM.read(10))+String((char)EEPROM.read(11))+String((char)EEPROM.read(12))+String((char)EEPROM.read(13))+String((char)EEPROM.read(14));

  // trajanje jedne pskalice, citanje iz EEPROM-a
  if(EEPROM.read(21)=='0') trajanje = 1;
  else if(EEPROM.read(21)=='1') trajanje = 10;
  else if(EEPROM.read(21)=='2') trajanje = 15;
  else if(EEPROM.read(21)=='3') trajanje = 20;
  
  // NTP
  configTime(EEPROM.read(20)* 3600, dst, "pool.ntp.org","time.nist.gov");
  while(!time(nullptr)){
     delay(1000);
  }
  
  // inicijalizacija izlaza
  pinMode(PRSKALICA1, OUTPUT);  
  pinMode(PRSKALICA2, OUTPUT);  
  pinMode(PRSKALICA3, OUTPUT);
  pinMode(PRSKALICA4, OUTPUT);  
  pinMode(PRSKALICA5, OUTPUT);  
  pinMode(PRSKALICA6, OUTPUT);
  pinMode(FONTANAPUMPA, OUTPUT);  

  // inicijalizacija ulaza
  pinMode(PRSKALICA_ULAZ, INPUT);
  pinMode(FONTANAPUMPA_ULAZ, INPUT);

  // setovanje pocetnih vrednosti
  digitalWrite(PRSKALICA1, !ukljuceno);   
  digitalWrite(PRSKALICA2, !ukljuceno);   
  digitalWrite(PRSKALICA3, !ukljuceno); 
  digitalWrite(PRSKALICA4, !ukljuceno);   
  digitalWrite(PRSKALICA5, !ukljuceno);   
  digitalWrite(PRSKALICA6, !ukljuceno);
  digitalWrite(FONTANAPUMPA, !ukljuceno);  
}


////////////////////////////////////////////////////////////////////
//          LOOP
///////////////////////////////////////////////////////////////////
void loop() {    
  if (!client.connected()) {
    reconnect();
  }

  client.setCallback(callback);
  client.loop();

  //timer za ukljucivanje prskalica u odredjeno vreme
  long now = millis();
  if (now - lastMsgTimer > 30000) {
    if(timer == 1){
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

  // prkalica 1 tajmer
  if(prskalica1){
    if (now- lastMsg > 60000*trajanje.toInt()) {
      prskalica1 = false;
      digitalWrite(PRSKALICA1, !ukljuceno);
      client.publish("bozaSub/kuca/node1/prskalica1/stanje", String("0").c_str(),false);
    }
  }

  // prkalica 2 tajmer
  if(prskalica2){
    if (now- lastMsg > 60000*trajanje.toInt()) {
      prskalica2 = false;
      digitalWrite(PRSKALICA2, !ukljuceno);
      client.publish("bozaSub/kuca/node1/prskalica2/stanje", String("0").c_str(),false);
    }
  }

  // prkalica 3 tajmer
  if(prskalica3){
    if (now- lastMsg > 60000*trajanje.toInt()) {
      prskalica3 = false;
      digitalWrite(PRSKALICA3, !ukljuceno);
      client.publish("bozaSub/kuca/node1/prskalica3/stanje", String("0").c_str(),false);
    }
  }

  // prkalica 4 tajmer
  if(prskalica4){
    if (now- lastMsg > 60000*trajanje.toInt()) {
      prskalica4 = false;
      digitalWrite(PRSKALICA4, !ukljuceno);
      client.publish("bozaSub/kuca/node1/prskalica4/stanje", String("0").c_str(),false);
    }
  }

  // prkalica 5 tajmer
  if(prskalica5){
    if (now- lastMsg > 60000*trajanje.toInt()) {
      prskalica5 = false;
      digitalWrite(PRSKALICA5, !ukljuceno);
      client.publish("bozaSub/kuca/node1/prskalica5/stanje", String("0").c_str(),false);
    }
  }

  // prkalica 6 tajmer
  if(prskalica6){
    if (now- lastMsg > 60000*trajanje.toInt()) {
      prskalica6 = false;
      digitalWrite(PRSKALICA6, !ukljuceno);
      client.publish("bozaSub/kuca/node1/prskalica6/stanje", String("0").c_str(),false);
    }
  }
  
  //prskalice kada se ukljuce sve. Svaka deonica se razlikuje u mnozenju sa 60.000 (60x1000)
  if(prskalica){
    if(prska == 0){
      digitalWrite(PRSKALICA1,ukljuceno);
      digitalWrite(PRSKALICA2,!ukljuceno);
      digitalWrite(PRSKALICA3,!ukljuceno);
      digitalWrite(PRSKALICA4,!ukljuceno);
      digitalWrite(PRSKALICA5,!ukljuceno);
      digitalWrite(PRSKALICA6,!ukljuceno);
      client.publish("bozaSub/kuca/node1/prskalica1/stanje", String("1").c_str(),false);
      prska++;
    }
  

    if(prska == 1){
      if (now- lastMsg > 60000*trajanje.toInt()) {
        digitalWrite(PRSKALICA1,!ukljuceno);
        digitalWrite(PRSKALICA2,ukljuceno);
        digitalWrite(PRSKALICA3,!ukljuceno);
        digitalWrite(PRSKALICA4,!ukljuceno);
        digitalWrite(PRSKALICA5,!ukljuceno);
        digitalWrite(PRSKALICA6,!ukljuceno);
        client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false); delay(20);
        client.publish("bozaSub/kuca/node1/prskalica2/stanje", String("1").c_str(),false);
        prska++;
      }
    }

    if(prska == 2){
      if (now- lastMsg > 120000*trajanje.toInt()) {
        digitalWrite(PRSKALICA1,!ukljuceno);
        digitalWrite(PRSKALICA2,!ukljuceno);
        digitalWrite(PRSKALICA3,ukljuceno);
        digitalWrite(PRSKALICA4,!ukljuceno);
        digitalWrite(PRSKALICA5,!ukljuceno);
        digitalWrite(PRSKALICA6,!ukljuceno);
        client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false); delay(20);
        client.publish("bozaSub/kuca/node1/prskalica3/stanje", String("1").c_str(),false);
        prska++;
      }
    }

    if(prska == 3){
      if (now- lastMsg > 180000*trajanje.toInt()) {
        digitalWrite(PRSKALICA1,!ukljuceno);
        digitalWrite(PRSKALICA2,!ukljuceno);
        digitalWrite(PRSKALICA3,!ukljuceno);
        digitalWrite(PRSKALICA4,ukljuceno);
        digitalWrite(PRSKALICA5,!ukljuceno);
        digitalWrite(PRSKALICA6,!ukljuceno);
        client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false); delay(20);
        client.publish("bozaSub/kuca/node1/prskalica4/stanje", String("1").c_str(),false);
        prska++;
      }
    }

    if(prska == 4){
      if (now- lastMsg > 240000*trajanje.toInt()) {
        digitalWrite(PRSKALICA1,!ukljuceno);
        digitalWrite(PRSKALICA2,!ukljuceno);
        digitalWrite(PRSKALICA3,!ukljuceno);
        digitalWrite(PRSKALICA4,!ukljuceno);
        digitalWrite(PRSKALICA5,ukljuceno);
        digitalWrite(PRSKALICA6,!ukljuceno);
        client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false); delay(20);
        client.publish("bozaSub/kuca/node1/prskalica5/stanje", String("1").c_str(),false);
        prska++;
      }
    }

    if(prska == 5){
      if (now- lastMsg > 300000*trajanje.toInt()) {
        digitalWrite(PRSKALICA1,!ukljuceno);
        digitalWrite(PRSKALICA2,!ukljuceno);
        digitalWrite(PRSKALICA3,!ukljuceno);
        digitalWrite(PRSKALICA4,!ukljuceno);
        digitalWrite(PRSKALICA5,!ukljuceno);
        digitalWrite(PRSKALICA6,ukljuceno);
        client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false); delay(20);
        client.publish("bozaSub/kuca/node1/prskalica6/stanje", String("1").c_str(),false);
        prska++;
      }
    }
     
    if(prska == 6){
      if (now- lastMsg > 360000*trajanje.toInt()) {
        lastMsg = now;
        client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false); delay(20);
        prska=0;
        prskalica = false;
        digitalWrite(PRSKALICA1,!ukljuceno);
        digitalWrite(PRSKALICA2,!ukljuceno);
        digitalWrite(PRSKALICA3,!ukljuceno);
        digitalWrite(PRSKALICA4,!ukljuceno);
        digitalWrite(PRSKALICA5,!ukljuceno);
        digitalWrite(PRSKALICA6,!ukljuceno);
      }
    }
  }

  //prskalica preko ulaza
  if (digitalRead(PRSKALICA_ULAZ) == HIGH) {
    if (buttonActive1 == false) {
      buttonActive1 = true;
      buttonTimer1 = millis();
    }
    if ((millis() - buttonTimer1 > longPressTime) && (longPressActive1 == false)) {
      longPressActive1 = true;   
        if(digitalRead(PRSKALICA1) == HIGH && digitalRead(PRSKALICA2) == HIGH && digitalRead(PRSKALICA3) == HIGH && digitalRead(PRSKALICA4) == HIGH && digitalRead(PRSKALICA5) == HIGH && digitalRead(PRSKALICA6) == HIGH){
          prskalica = true;
          client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("1").c_str(),false); delay(50);
          lastMsg = millis();
          prska = 0;
        }
        else{
          prskalica = false;
          client.publish("bozaSub/kuca/node1/prskalicaAll/stanje", String("0").c_str(),false); delay(50);
          digitalWrite(PRSKALICA1,!ukljuceno);
          digitalWrite(PRSKALICA2,!ukljuceno);
          digitalWrite(PRSKALICA3,!ukljuceno);
          digitalWrite(PRSKALICA4,!ukljuceno);
          digitalWrite(PRSKALICA5,!ukljuceno);
          digitalWrite(PRSKALICA6,!ukljuceno);
        }
      }
  } else {
    buttonActive1 = false;
    longPressActive1 = false;
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
          digitalWrite(FONTANAPUMPA, ukljuceno);
        }
        else{
          client.publish("bozaSub/kuca/node1/fontanaPumpa/stanje", String("0").c_str(),false);
          digitalWrite(FONTANAPUMPA, !ukljuceno);
        }
      }
  }else {
    buttonActive2 = false;
    longPressActive2 = false;
  }
  
}
