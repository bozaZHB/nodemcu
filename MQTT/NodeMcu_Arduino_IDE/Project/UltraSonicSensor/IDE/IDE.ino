#include <NewPing.h>
 
#define TRIGGER_PIN  12  
#define ECHO_PIN     11  
#define MAX_DISTANCE 200 
 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

volatile  bool dole=false;
volatile  bool gore=false;

void setup() {
  Serial.begin(115200);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);

  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}
 
void loop() {
  if(digitalRead(4)==LOW) {
    dole=true;
    gore = false;
  }
  if(digitalRead(5)==LOW){
    dole = false;
    gore = false;
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  }
  if(dole){
    int vrednost = sonar.ping_cm();
    if(vrednost<20 && vrednost!=0) {
      digitalWrite(7, LOW);
      digitalWrite(6, HIGH);
    }
    if(vrednost>=20) {
      digitalWrite(6, LOW);
      gore=true;
      dole=false;
    }
    Serial.print("DOLE: ");
    Serial.println(sonar.ping_cm());
  }
  if(gore){
    int vrednost = sonar.ping_cm();
    digitalWrite(7, HIGH);
    if(vrednost<11 && vrednost!=0) {
      digitalWrite(7, LOW);
      gore=false;
    }
    Serial.print("GORE: ");
    Serial.println(sonar.ping_cm());
  }
  delay(200);
}
