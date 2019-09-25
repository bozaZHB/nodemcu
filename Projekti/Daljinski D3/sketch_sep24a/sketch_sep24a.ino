/*
//podesavanje vremenske zone u ROM memoriji

*/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <time.h>
#include <EEPROM.h>

IRsend irsendTv(D0); //an IR led for TV
IRsend irsendD3(D1); //an IR led for D3
int RECV_PIN = D2; //an IR detector/demodulatord is connected to GPIO pin D2

const char* ssid = "e665e4";
const char* password = "262751838";
const char* mqtt_server = "192.168.0.100";

WiFiClient espClient;
PubSubClient client(espClient);
IRrecv irrecv(RECV_PIN);
decode_results results;

long lastMsg = 0;
int value = 0;
char msg[50];

int BUZZER = D7;
bool timer = false;

String vremeTajmera = "06:00";

int dst = 0;

bool vrstaTajmera = false; //false je samo Tv, true je alarm

bool flagFastChoosing = false;

void setup_wifi() {
  delay(100);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
  randomSeed(micros());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  char p =(char)payload[0];//-'0'
  String str(topic);
  if (str =="bozaSub/kuca/tv/kanal"){
    if(p=='0'){
      irsendD3.sendNEC(0x60DF926D, 32);delay(50);
    }
    else if(p=='1'){
      irsendD3.sendNEC(0x60DFC837, 32);delay(50);
    }
    else if(p=='2'){
      irsendD3.sendNEC(0x60DF08F7, 32);delay(50);
    }
    else if(p=='3'){
      irsendD3.sendNEC(0x60DF8877, 32);delay(50);
    }
    else if(p=='4'){
      irsendD3.sendNEC(0x60DFF00F, 32);delay(50);
    }
    else if(p=='5'){
      irsendD3.sendNEC(0x60DF30CF, 32);delay(50);
    }
    else if(p=='6'){
      irsendD3.sendNEC(0x60DFB04F, 32);delay(50);
    }
    else if(p=='7'){
      irsendD3.sendNEC(0x60DFD02F, 32);delay(50);
    }
    else if(p=='8'){
      irsendD3.sendNEC(0x60DF10EF, 32);delay(50);
    }
    else if(p=='9'){
      irsendD3.sendNEC(0x60DF906F, 32);delay(50);
    }
    else if(p=='i'){
      irsendD3.sendNEC(0x60DF22DD, 32);delay(50);
    }
    else if(p=='q'){
      irsendD3.sendNEC(0x60DFE01F, 32);delay(50);
    }
    else if(p=='w'){
      irsendD3.sendNEC(0x60DFD22D, 32);delay(50);
    }
    else if(p=='e'){
      irsendD3.sendNEC(0x60DFC03F, 32);delay(50);
    }
    else if(p=='u'){
      irsendD3.sendNEC(0x60DF3AC5, 32);delay(50);
    }
    //tv
    else if(p=='r'){
      irsendTv.sendNEC(0x707028D7, 32);delay(50);
    }
    else if(p=='t'){
      irsendTv.sendNEC(0x7070A857, 32);delay(50);
    }
    else if(p=='y'){
      // tv on/off i d3 on/off
      irsendTv.sendNEC(0x707030CF, 32);delay(50);
      irsendD3.sendNEC(0x60DF708F, 32);delay(50);
    }
  }
  else if (str =="bozaSub/kuca/buzzer"){
    digitalWrite(BUZZER, HIGH);delay(200);
    digitalWrite(BUZZER, LOW);delay(200); 
    digitalWrite(BUZZER, HIGH);delay(200);
    digitalWrite(BUZZER, LOW); 
  }
  else if (str =="bozaSub/kuca/tv/kanal/n1"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/rts1"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DFD02F, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/o2"){
    irsendD3.sendNEC(0x60DFF00F, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/prva"){
    irsendD3.sendNEC(0x60DFB04F, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/fox"){
    irsendD3.sendNEC(0x60DF906F, 32);delay(150);
    irsendD3.sendNEC(0x60DF30CF, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/foxLife"){
    irsendD3.sendNEC(0x60DFD02F, 32);delay(150);
    irsendD3.sendNEC(0x60DF8877, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/comedyCentral"){
    irsendD3.sendNEC(0x60DF10EF, 32);delay(150);
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/hbo"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF926D, 32);delay(150);
    irsendD3.sendNEC(0x60DF926D, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/hbo2"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF926D, 32);delay(150);
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/hbo3"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF926D, 32);delay(150);
    irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/cinemax"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF926D, 32);delay(150);
    irsendD3.sendNEC(0x60DF8877, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/cinemax2"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF926D, 32);delay(150);
    irsendD3.sendNEC(0x60DFF00F, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/axn"){
    irsendD3.sendNEC(0x60DFD02F, 32);delay(150);
    irsendD3.sendNEC(0x60DFD02F, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/sk1"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/sk2"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/sk3"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF8877, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/skhd"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF926D, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/es1"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF8877, 32);delay(150);
    irsendD3.sendNEC(0x60DF926D, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/es2"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF8877, 32);delay(150);
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/ng"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF906F, 32);delay(150);
    irsendD3.sendNEC(0x60DF906F, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/discovery"){
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF10EF, 32);delay(150);
    irsendD3.sendNEC(0x60DFC837, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/sci"){
    irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
    irsendD3.sendNEC(0x60DF926D, 32);delay(150);
    irsendD3.sendNEC(0x60DF926D, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/ngWild"){
    irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
    irsendD3.sendNEC(0x60DF926D, 32);delay(150);
    irsendD3.sendNEC(0x60DFB04F, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/pinkMusic"){
    irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
    irsendD3.sendNEC(0x60DFF00F, 32);delay(150);
    irsendD3.sendNEC(0x60DF30CF, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/vh1"){
    irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
    irsendD3.sendNEC(0x60DF30CF, 32);delay(150);
    irsendD3.sendNEC(0x60DF926D, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/s2"){
    irsendD3.sendNEC(0x60DFD02F, 32);delay(150);
    irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
    irsendD3.sendNEC(0x60DF926D, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/tdi"){
    irsendD3.sendNEC(0x60DFD02F, 32);delay(150);
    irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
    irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/play"){
    irsendD3.sendNEC(0x60DFD02F, 32);delay(150);
    irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
    irsendD3.sendNEC(0x60DF8877, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/topFm"){
    irsendD3.sendNEC(0x60DFD02F, 32);delay(150);
    irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
    irsendD3.sendNEC(0x60DF30CF, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/kanal/classicalGreats"){
    irsendD3.sendNEC(0x60DFD02F, 32);delay(150);
    irsendD3.sendNEC(0x60DF10EF, 32);delay(150);
    irsendD3.sendNEC(0x60DFF00F, 32);delay(150);
    irsendD3.sendNEC(0x60DF3AC5, 32);
  }
  else if (str =="bozaSub/kuca/tv/timer/status"){
    if(p=='1'){
      timer = true;
    }
    else
      timer = false;
  }
  else if (str =="bozaSub/kuca/tv/timer/vreme"){
    vremeTajmera = String((char)payload[0])+String((char)payload[1])+String((char)payload[2])+String((char)payload[3])+String((char)payload[4]);
  }
  else if (str =="bozaSub/kuca/tv/timer/vrsta"){
    if(p=='1'){
      vrstaTajmera = true; //alarm
    }
    else
      vrstaTajmera = false; //samo tv
  }
  else if (str =="bozaSub/kuca/tv/timer/stanje"){
    client.publish("bozaSub/kuca/tv/timer/vreme/stanje", String(vremeTajmera).c_str(),false);
    client.publish("bozaSub/kuca/tv/timer/status/stanje", String(timer).c_str(),false);
    client.publish("bozaSub/kuca/tv/timer/vreme/vrsta", String(vrstaTajmera).c_str(),false);
  }
  else if (str =="bozaSub/kuca/tv/timer/stanje/vreme"){
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    String minuta = String(p_tm->tm_min);
    String sati = String(p_tm->tm_hour);
    if(minuta.length()==1) minuta = "0"+minuta;
    if(sati.length()==1) sati = "0"+sati;
    String vremeTrenutno = sati+":"+minuta;
    client.publish("bozaSub/kuca/tv/timer/vreme/vrsta", String(vremeTrenutno).c_str(),false);
  }
  else if (str =="bozaSub/kuca/tv/vremenskaZona"){
    EEPROM.write(0, p);
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
      client.subscribe("bozaSub/#");
    } else {
      delay(500);
    }
  }
}

void setup() {
  EEPROM.begin(512);
  
  Serial.begin(9600);
  Serial.end();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  irrecv.enableIRIn(); 

  irsendTv.begin();
  irsendD3.begin();

  pinMode(BUZZER, OUTPUT);

  //NTP
  configTime(EEPROM.read(0)* 3600, dst, "pool.ntp.org","time.nist.gov");
  while(!time(nullptr)){
     delay(1000);
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //IR 
  if (irrecv.decode(&results)){
    String msg="";
    msg= msg+ results.value;
    if(msg.substring(0,10)=="1886389754" || msg.substring(0,10)=="1625249959")
        client.publish("bozaSub/sijalica/sve", "0",false);
    else if(msg.substring(0,10)=="1886422394" || msg.substring(0,10)=="1625286679"){
        client.publish("bozaSub/sijalica/jedan", "0",false);
        client.publish("bozaSub/sijalica/dva", "1",false);
    }
    else if(msg.substring(0,10)=="1886406074" || msg.substring(0,10)=="1625233639"){
        client.publish("bozaSub/sijalica/jedan", "1",false);
        client.publish("bozaSub/sijalica/dva", "0",false);
    }
    else if(msg.substring(0,10)=="1886438714" || msg.substring(0,10)=="1625282599")
        client.publish("bozaSub/sijalica/sve", "1",false);
    else if(msg.substring(0,10)=="1625241799"){
        flagFastChoosing = false;
    }
    else if(msg.substring(0,10)=="1625232109"){
        flagFastChoosing = true;
    }
    
     if (flagFastChoosing){
      //1
      if(msg.substring(0,10)=="1625278519"){
          //sk1
          delay(150);
          irsendD3.sendNEC(0x60DFC837, 32);delay(150);
          irsendD3.sendNEC(0x60DFC837, 32);delay(150);
          irsendD3.sendNEC(0x60DFC837, 32);delay(150);
          irsendD3.sendNEC(0x60DF3AC5, 32);
      }
      //2
      else if(msg.substring(0,10)=="1625229559"){
          //sk2
          delay(150);
          irsendD3.sendNEC(0x60DFC837, 32);delay(150);
          irsendD3.sendNEC(0x60DFC837, 32);delay(150);
          irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
          irsendD3.sendNEC(0x60DF3AC5, 32);
      }
      //3
      else if(msg.substring(0,10)=="1625262199"){
          //sk3
          delay(150);
          irsendD3.sendNEC(0x60DFC837, 32);delay(150);
          irsendD3.sendNEC(0x60DFC837, 32);delay(150);
          irsendD3.sendNEC(0x60DF8877, 32);delay(150);
          irsendD3.sendNEC(0x60DF3AC5, 32);
      }
      //4
      else if(msg.substring(0,10)=="1625288719"){
          //hbo1
          delay(150);
          irsendD3.sendNEC(0x60DFC837, 32);delay(150);
          irsendD3.sendNEC(0x60DF926D, 32);delay(150);
          irsendD3.sendNEC(0x60DF926D, 32);delay(150);
          irsendD3.sendNEC(0x60DF3AC5, 32);
      }
      //5
      else if(msg.substring(0,10)=="1625239759"){
          //hbo2
          delay(150);
          irsendD3.sendNEC(0x60DFC837, 32);delay(150);
          irsendD3.sendNEC(0x60DF926D, 32);delay(150);
          irsendD3.sendNEC(0x60DFC837, 32);delay(150);
          irsendD3.sendNEC(0x60DF3AC5, 32);
      }
      //6
      else if(msg.substring(0,10)=="1625272399"){
          //hbo3
          delay(150);
          irsendD3.sendNEC(0x60DFC837, 32);delay(150);
          irsendD3.sendNEC(0x60DF926D, 32);delay(150);
          irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
          irsendD3.sendNEC(0x60DF3AC5, 32);
      }
      //7
      else if(msg.substring(0,10)=="1625280559"){
          //sci
          delay(150);
          irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
          irsendD3.sendNEC(0x60DF926D, 32);delay(150);
          irsendD3.sendNEC(0x60DF926D, 32);delay(150);
          irsendD3.sendNEC(0x60DF3AC5, 32);
      }
      //8
      else if(msg.substring(0,10)=="1625231599"){
          //wild
          delay(150);
          irsendD3.sendNEC(0x60DF08F7, 32);delay(150);
          irsendD3.sendNEC(0x60DF926D, 32);delay(150);
          irsendD3.sendNEC(0x60DFB04F, 32);delay(150);
          irsendD3.sendNEC(0x60DF3AC5, 32);
      }
      //9
      else if(msg.substring(0,10)=="1625264239"){
          //classic
          delay(150);
          irsendD3.sendNEC(0x60DFD02F, 32);delay(150);
          irsendD3.sendNEC(0x60DF10EF, 32);delay(150);
          irsendD3.sendNEC(0x60DFF00F, 32);delay(150);
          irsendD3.sendNEC(0x60DF3AC5, 32);
      }
      //0
      else if(msg.substring(0,10)=="1625264749"){
          //hd
          delay(150);
          irsendD3.sendNEC(0x60DFC837, 32);delay(150);
          irsendD3.sendNEC(0x60DFC837, 32);delay(150);
          irsendD3.sendNEC(0x60DF926D, 32);delay(150);
          irsendD3.sendNEC(0x60DF3AC5, 32);
      }
    }
    irrecv.resume();
   }


  //timer
  long now = millis();
  if (now - lastMsg > 1000) {
    if(timer){
      time_t now = time(nullptr);
      struct tm* p_tm = localtime(&now);
      String minuta = String(p_tm->tm_min);
      String sati = String(p_tm->tm_hour);
      if(minuta.length()==1) minuta = "0"+minuta;
      if(sati.length()==1) sati = "0"+sati;
      String vremeTrenutno = sati+":"+minuta;
      if(vremeTajmera==vremeTrenutno){
        if(vrstaTajmera){
          //alarm
          //tv
          irsendTv.sendNEC(0x707030CF, 32);delay(50);
          //d3
          irsendD3.sendNEC(0x60DF708F, 32);delay(50);
          //zujalica
          digitalWrite(BUZZER, HIGH);delay(1000);
          digitalWrite(BUZZER, LOW);delay(200); 
          digitalWrite(BUZZER, HIGH);delay(200);
          digitalWrite(BUZZER, LOW);delay(200);
          digitalWrite(BUZZER, HIGH);delay(200);
          digitalWrite(BUZZER, LOW);delay(200);
          digitalWrite(BUZZER, HIGH);delay(200);
          digitalWrite(BUZZER, LOW); 
          //svetlo
          client.publish("bozaSub/sijalica/dva", "1",false);delay(500);
          client.publish("bozaSub/sijalica/dva", "0",false);delay(500);

          client.publish("bozaSub/sijalica/dva", "1",false);delay(500);
          client.publish("bozaSub/sijalica/dva", "0",false);delay(500);

          client.publish("bozaSub/sijalica/dva", "1",false);delay(500);
          client.publish("bozaSub/sijalica/dva", "0",false);delay(500);

          client.publish("bozaSub/sijalica/dva", "1",false);delay(7000);
          //kanal
          irsendD3.sendNEC(0x60DF08F7, 32);delay(50);
          irsendD3.sendNEC(0x60DF30CF, 32);delay(50);
          irsendD3.sendNEC(0x60DF926D, 32);delay(50);
          irsendD3.sendNEC(0x60DF3AC5, 32);
        }
        else
          irsendTv.sendNEC(0x707030CF, 32);delay(50);
          irsendD3.sendNEC(0x60DF708F, 32);delay(50);
          timer = false;
        }
      }
      lastMsg = now;
    }
}
