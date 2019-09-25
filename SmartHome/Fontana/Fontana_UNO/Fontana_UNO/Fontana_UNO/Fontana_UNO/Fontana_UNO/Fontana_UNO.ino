int PRSKALICA1 = 2; 
int PRSKALICA2 = 3; 
int PRSKALICA3 = 4; 
int PRSKALICA4 = 5;
int FONTANAPUMPA = 6; 
int RADIO = 7; 
int NEODREDJENO = 8;

volatile int vreme = 10; //minuta
volatile bool prskalica = false;
int prska = 0;
long lastMsg = 0;

int incomingByte;
    
int BUTTON1=9;
int BUTTON2=10;
int BUTTON3=11;
int BUTTON4=12;

    
volatile int lightsOn1 = 0,
    lightsOn2 = 0,
    lightsOn3 = 0,
    lightsOn4 = 0;


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
void setup() {
  Serial.begin(9600);
  
  pinMode(PRSKALICA1, OUTPUT);
  pinMode(PRSKALICA2, OUTPUT);
  pinMode(PRSKALICA3, OUTPUT);
  pinMode(PRSKALICA4, OUTPUT);
  pinMode(RADIO, OUTPUT);
  pinMode(FONTANAPUMPA, OUTPUT);
  pinMode(NEODREDJENO, OUTPUT);
  
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(BUTTON4, INPUT);
  
  digitalWrite(PRSKALICA1, LOW); 
  digitalWrite(PRSKALICA2, LOW); 
  digitalWrite(PRSKALICA3, LOW); 
  digitalWrite(PRSKALICA4, LOW); 
  digitalWrite(RADIO, LOW); 
  digitalWrite(FONTANAPUMPA, LOW); 
  digitalWrite(NEODREDJENO, LOW);
  delay(10000);//cekaj 10 sek
}



void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 'q') {//prskalica 1
      if(!prskalica){
        digitalWrite(PRSKALICA1, HIGH);
        Serial.print('q');
      }
      
    }
    if (incomingByte == 'w') {
      if(!prskalica){
        digitalWrite(PRSKALICA1, LOW); 
        Serial.print('w');        
      }
    }
    if (incomingByte == 'e') {//prskalica 2
      if(!prskalica){
        digitalWrite(PRSKALICA2, HIGH);
        Serial.print('e');
      }
    }
    if (incomingByte == 'r') {
      if(!prskalica){
        digitalWrite(PRSKALICA2, LOW); 
        Serial.print('r');
      }
    }
    if (incomingByte == 't') {//prskalica 3
      if(!prskalica){
        digitalWrite(PRSKALICA3, HIGH);
        Serial.print('t');
      }
    }
    if (incomingByte == 'y') {
      if(!prskalica){
        digitalWrite(PRSKALICA3, LOW); 
        Serial.print('y');
      }
    }
    if (incomingByte == 'u') {//prskalica 4
      if(!prskalica){
        digitalWrite(PRSKALICA4, HIGH);
        Serial.print('u');
      }
    }
    if (incomingByte == 'i') {
      digitalWrite(PRSKALICA4, LOW); 
      Serial.print('i');
    }
    if (incomingByte == 'o') {//fontana pumpa
      digitalWrite(FONTANAPUMPA, HIGH);
      lightsOn2 = 1;
      Serial.print('o');
    }
    if (incomingByte == 'p') {
      digitalWrite(FONTANAPUMPA, LOW); 
      lightsOn2 = 0;
      Serial.print('p');
    }
    if (incomingByte == 'a') {//radio
      digitalWrite(RADIO, HIGH);
      lightsOn3 = 1;
      Serial.print('a');
    }
    if (incomingByte == 's') {
      digitalWrite(RADIO, LOW); 
      lightsOn3 = 0;
      Serial.print('s');
    }
    if (incomingByte == 'd') {//neodredjeno
      digitalWrite(NEODREDJENO, HIGH);
      lightsOn4 = 1;
      Serial.print('d');
    }
    if (incomingByte == 'f') {
      digitalWrite(NEODREDJENO, LOW); 
      lightsOn4 = 0;
      Serial.print('f');
    }
    if (incomingByte == 'h') {//prskalicaAll on
      prskalica = true;
      Serial.print('h');
      lastMsg = millis();
      prska = 0;
    }
    if (incomingByte == 'j') {//prskalicaAll off
      prskalica = false;
      Serial.print('j');
      digitalWrite(PRSKALICA1,LOW);
      digitalWrite(PRSKALICA2,LOW);
      digitalWrite(PRSKALICA3,LOW);
      digitalWrite(PRSKALICA4,LOW);
    }
    if (incomingByte == 'k') {//provera stanja za prskalice all
      if(digitalRead(PRSKALICA1)==HIGH || digitalRead(PRSKALICA2)==HIGH || digitalRead(PRSKALICA3)==HIGH || digitalRead(PRSKALICA4)==HIGH){
        Serial.print('k');
      }
      else
        Serial.print('l');
    }
    if (incomingByte == 'l') {//provera stanja za fontanu pumpe
      if(digitalRead(FONTANAPUMPA)==HIGH){
        Serial.print('z');
      }
      else
        Serial.print('x');
    }
    if (incomingByte == 'z') {//provera stanja za radio
      if(digitalRead(RADIO)==HIGH){
        Serial.print('c');
      }
      else
        Serial.print('v');
    }
    if (incomingByte == 'x') {//provera stanja za neodredjeno
      if(digitalRead(NEODREDJENO)==HIGH){
        Serial.print('b');
      }
      else
        Serial.print('n');
    }
    if (incomingByte == '1') {//trajanje prskalice je 10 minuta
      vreme = 10;
      Serial.print('1');
    }
    if (incomingByte == '2') {//trajanje prskalice je 15 minuta
      vreme = 15;
      Serial.print('2');
    }
    if (incomingByte == '3') {//trajanje prskalice je 20 minuta
      vreme = 20;
      Serial.print('3');
    }
    if (incomingByte == 'm') {//provera trajanja vremena prskalica
      if(vreme==10)Serial.print('1');
      else if(vreme==15)Serial.print('2');
      else if(vreme==20)Serial.print('3');
    }
  }

//prskalica
if(prskalica){
    long now = millis();
    if(prska == 0){
        digitalWrite(PRSKALICA1,HIGH);
        digitalWrite(PRSKALICA2,LOW);
        digitalWrite(PRSKALICA3,LOW);
        digitalWrite(PRSKALICA4,LOW);
        Serial.print('q');
        prska++;
    }

    if(prska == 1){
      if (now- lastMsg > 60000*vreme) {
        digitalWrite(PRSKALICA1,LOW);
        digitalWrite(PRSKALICA2,HIGH);
        digitalWrite(PRSKALICA3,LOW);
        digitalWrite(PRSKALICA4,LOW);
        Serial.print('e');
        prska++;
      }
    }

    if(prska == 2){
      if (now- lastMsg > 120000*vreme) {
        digitalWrite(PRSKALICA1,LOW);
        digitalWrite(PRSKALICA2,LOW);
        digitalWrite(PRSKALICA3,HIGH);
        digitalWrite(PRSKALICA4,LOW);
        Serial.print('t');
        prska++;
      }
    }

    if(prska == 3){
      if (now- lastMsg > 180000*vreme) {
        digitalWrite(PRSKALICA1,LOW);
        digitalWrite(PRSKALICA2,LOW);
        digitalWrite(PRSKALICA3,LOW);
        digitalWrite(PRSKALICA4,HIGH);
        Serial.print('u');
        prska++;
      }
    }
    
    if(prska == 4){
      if (now- lastMsg > 240000*vreme) {
        lastMsg = now;
        Serial.print('j');
        prska=0;
        lightsOn1 = 0;
        prskalica = false;
        digitalWrite(PRSKALICA1,LOW);
        digitalWrite(PRSKALICA2,LOW);
        digitalWrite(PRSKALICA3,LOW);
        digitalWrite(PRSKALICA4,LOW);
      }
    }
  }


//prskalica
  if (digitalRead(BUTTON1) == LOW) {
    if (buttonActive1 == false) {
      buttonActive1 = true;
      buttonTimer1 = millis();
    }
    if ((millis() - buttonTimer1 > longPressTime) && (longPressActive1 == false)) {
      longPressActive1 = true;   
        if(!lightsOn1){
          prskalica = true;
          Serial.print('h');
          lastMsg = millis();
          prska = 0;
          lightsOn1 = 1;
        }
        else{
          prskalica = false;
          lightsOn1 = 0;
          Serial.print('j');
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


//taster za fontanu pumpe
  if (digitalRead(BUTTON2) == LOW) {
    if (buttonActive2 == false) {
      buttonActive2 = true;
      buttonTimer2 = millis();
    }
    if ((millis() - buttonTimer2 > longPressTime) && (longPressActive2 == false)) {
      longPressActive2 = true;   
        if(!lightsOn2){
          Serial.print('o');
          digitalWrite(FONTANAPUMPA, HIGH);
          lightsOn2 = 1;
        }
        else{
          Serial.print('p');
          digitalWrite(FONTANAPUMPA, LOW);
          lightsOn2 = 0;
        }
      }
  }else {
    buttonActive2 = false;
    longPressActive2 = false;
  }



//taster za radio
  if (digitalRead(BUTTON3) == LOW) {
    if (buttonActive3 == false) {
      buttonActive3 = true;
      buttonTimer3 = millis();
    }
    if ((millis() - buttonTimer3 > longPressTime) && (longPressActive3 == false)) {
      longPressActive3 = true;   
        if(!lightsOn3){
          Serial.print('a');
          digitalWrite(RADIO, HIGH);
          lightsOn3 = 1;
        }
        else{
          Serial.print('s');
          digitalWrite(RADIO, LOW);
          lightsOn3 = 0;
        }
      }
  }else {
    buttonActive3 = false;
    longPressActive3 = false;
  }

  
//taster za neodredjeno
  if (digitalRead(BUTTON4) == LOW) {
    if (buttonActive4 == false) {
      buttonActive4 = true;
      buttonTimer4 = millis();
    }
    if ((millis() - buttonTimer4 > longPressTime) && (longPressActive4 == false)) {
      longPressActive4 = true;   
        if(!lightsOn4){
          Serial.print('d');
          digitalWrite(NEODREDJENO, HIGH);
          lightsOn4 = 1;
        }
        else{
          Serial.print('f');
          digitalWrite(NEODREDJENO, LOW);
          lightsOn4 = 0;
        }
      }
  }else {
    buttonActive4 = false;
    longPressActive4 = false;
  }
      
}
