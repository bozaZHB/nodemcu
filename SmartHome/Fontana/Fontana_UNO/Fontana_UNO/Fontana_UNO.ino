int PRSKALICA1 = 2; 
int PRSKALICA2 = 3; 
int PRSKALICA3 = 4; 
int PRSKALICA4 = 5; 
int RADIO = 6; 
int FONTANAPUMPA = 7; 
int NEODREDJENO = 8;

int vreme = 10; //minuta
volatile bool prskalica = false;
int prska = 0;
long lastMsg = 0;

int incomingByte;

int switchState1=0,
    switchState2 = 0,
    switchState3 = 0,
    switchState4 = 0;
    
int BUTTON1=9;
int BUTTON2=10;
int BUTTON3=11;
int BUTTON4=12;

int oldSwitchState1 = 0,
    oldSwitchState2 = 0,
    oldSwitchState3 = 0,
    oldSwitchState4 = 0;
volatile int lightsOn1 = 0,
    lightsOn2 = 0,
    lightsOn3 = 0,
    lightsOn4 = 0;


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
  
}



void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 'd') {
      digitalWrite(PRSKALICA1, HIGH);
      lightsOn1 = 1;
      Serial.print('d');
    }
    if (incomingByte == 'f') {
      digitalWrite(PRSKALICA1, LOW); 
      lightsOn1 = 0;
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
  }

//prskalica
if(prskalica){
    long now = millis();
    if(prska == 0){
        Serial.print('q');
        prska++;
    }

    if(prska == 1){
      if (now- lastMsg > 60000*vreme) {
        Serial.print('e');
        prska++;
      }
    }

    if(prska == 2){
      if (now- lastMsg > 120000*vreme) {
        Serial.print('t');
        prska++;
      }
    }
    
    if(prska == 3){
      if (now- lastMsg > 180000*vreme) {
        lastMsg = now;
        Serial.print('j');
        prska=0;
        prskalica = false;
      }
    }
  }


  switchState1 = digitalRead(BUTTON1); 
  if (switchState1 != oldSwitchState1){
    oldSwitchState1 = switchState1;
    if (switchState1 == HIGH){      
      if(!lightsOn1){
        Serial.print('d');
        digitalWrite(PRSKALICA1, HIGH);
        lightsOn1 = 1;
      }
      else{
        Serial.print('f');
        digitalWrite(PRSKALICA1, LOW);
        lightsOn1 = 0;
      }
    } 
  }
}
