int SMER = 2;
int POWER = 3;

volatile int lightsOn = 0;
volatile int powerOn = 0;

long buttonTimerSmer = 0;
long buttonTimerPower = 0;

long longPressTime = 250;

boolean buttonActiveSmer = false;
boolean buttonActivePower = false;

boolean longPressActiveSmer = false;
boolean longPressActivePower = false;

void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);

  pinMode(SMER, INPUT);
  pinMode(POWER, INPUT);
}

void loop() {
  // POWER
  if (digitalRead(POWER) == HIGH) {//high za pull down
    if (buttonActivePower == false) {
      buttonActivePower = true;
      buttonTimerPower = millis();
    }
    if ((millis() - buttonTimerPower > longPressTime) && (longPressActivePower == false)) {
      longPressActivePower = true;   
        if(!powerOn){
          powerOn = 1;
        }
        else{
          powerOn = 0;
        }
      }
  }else {
    buttonActivePower = false;
    longPressActivePower = false;
  }

  // SMER
  if (digitalRead(SMER) == HIGH) {//high za pull down
    if (buttonActiveSmer == false) {
      buttonActiveSmer = true;
      buttonTimerSmer = millis();
    }
    if ((millis() - buttonTimerSmer > longPressTime) && (longPressActiveSmer == false)) {
      longPressActiveSmer = true;   
        if(!lightsOn){
          digitalWrite(8, HIGH);
          lightsOn = 1;
        }
        else{
          digitalWrite(8, LOW);
          lightsOn = 0;
        }
      }
  }else {
    buttonActiveSmer = false;
    longPressActiveSmer = false;
  }

  
  if(powerOn == 1){
    digitalWrite(9, HIGH);
    delay(1);
    digitalWrite(9, LOW);
    delay(1);
  }

}
