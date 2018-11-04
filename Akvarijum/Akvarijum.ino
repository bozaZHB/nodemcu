int BUTTON = 8;
int RelejGore = 9;
int RelejDole = 10;
int MIN = A0;
int MAX = A1;
bool  dole = false,
      gore = false;
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON,INPUT);
  pinMode(RelejGore,OUTPUT);
  pinMode(RelejDole,OUTPUT);
  digitalWrite(RelejGore, HIGH);
  digitalWrite(RelejDole, HIGH);
}

void loop() {
  //sensorValue = analogRead(sensorPin);
  if(digitalRead(BUTTON)==HIGH) dole = true;
  if(dole){
    if(analogRead(MIN)>200){
      digitalWrite(RelejDole, LOW);
      digitalWrite(RelejGore, HIGH);
      Serial.println("DOLE");
    }
    else{
      delay(1000);
      dole = false;
      gore = true;
    }
  }
  if(gore){
      if(analogRead(MAX)<200){
        Serial.println("GORE");
        digitalWrite(RelejDole, HIGH);
        digitalWrite(RelejGore, LOW);        
      }
      else{
        Serial.println("GOTOVO");
        digitalWrite(RelejDole, HIGH);
        digitalWrite(RelejGore, HIGH);
        gore = false;
      }
    }
  
}
