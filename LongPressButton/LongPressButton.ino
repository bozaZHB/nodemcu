int FONTANAPUMPA = D0;    
int BUTTON2 = D1;
volatile int lightsOn2 = 0;

long buttonTimer = 0;
long longPressTime = 250;

boolean buttonActive = false;
boolean longPressActive = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(FONTANAPUMPA, OUTPUT);
  
  pinMode(BUTTON2, INPUT);
  
}



void loop() {

  if (digitalRead(BUTTON2) == LOW) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
      longPressActive = true;   
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
    buttonActive = false;
    longPressActive = false;
  }
  
  

}
