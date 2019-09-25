int BUTTON1 = 2,
    BUTTON2 = 3,
    BUTTON3 = 4,
    BUTTON4 = 5,
    BUTTON5 = 6,
    BUTTON6 = 7,
    LED1 = 8,
    LED2 = 9,
    LED3 = 10,
    LED4 = 11,
    LED5 = 12,
    LED6 = 13,
    switchState1 = 0,
    switchState2 = 0,
    switchState3 = 0,
    switchState4 = 0,
    switchState5 = 0,
    switchState6 = 0;
volatile int oldSwitchState1 = 0,
    oldSwitchState2 = 0,
    oldSwitchState3 = 0,
    oldSwitchState4 = 0,
    oldSwitchState5 = 0,
    oldSwitchState6 = 0,
    lightsOn1 = 0,
    lightsOn2 = 0,
    lightsOn3 = 0,
    lightsOn4 = 0,
    lightsOn5 = 0,
    lightsOn6 = 0;
int incomingByte;
void setup() {
  Serial.begin(9600);

  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(BUTTON4, INPUT);
  pinMode(BUTTON5, INPUT);
  pinMode(BUTTON6, INPUT);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 'H'){
      digitalWrite(LED2, HIGH);
      lightsOn2 = 1;
    }
    else if (incomingByte == 'L'){
      digitalWrite(LED2, LOW); 
      lightsOn2 = 0;
    }
  }

  
  switchState1 = digitalRead(BUTTON1); // read the pushButton State
  if (switchState1 != oldSwitchState1){
    oldSwitchState1 = switchState1;
    if (switchState1 == HIGH){      
      if(!lightsOn1){
        //client.publish("bozaSub", "1");
        digitalWrite(LED1, HIGH);
        lightsOn1 = 1;
      }
      else{
        //client.publish("bozaSub", "0");
        digitalWrite(LED1, LOW);
        lightsOn1 = 0;
      }
    } 
  }

  switchState2 = digitalRead(BUTTON2); // read the pushButton State
  if (switchState2 != oldSwitchState2){
    oldSwitchState2 = switchState2;
    if (switchState2 == HIGH){      
      if(!lightsOn2){
        //client.publish("bozaSub", "3");
        digitalWrite(LED2, HIGH);
        lightsOn2 = 1;
      }
       else{
        //client.publish("bozaSub", "2");
        digitalWrite(LED2, LOW);
        lightsOn2 = 0;
       }
    } 
  }

  switchState3 = digitalRead(BUTTON3); // read the pushButton State
  if (switchState3 != oldSwitchState3){
    oldSwitchState3 = switchState3;
    if (switchState3 == HIGH){      
      if(!lightsOn3){
        //client.publish("bozaSub", "3");
        digitalWrite(LED3, HIGH);
        lightsOn3 = 1;
      }
       else{
        //client.publish("bozaSub", "2");
        digitalWrite(LED3, LOW);
        lightsOn3 = 0;
       }
    } 
  }

  switchState4 = digitalRead(BUTTON4); // read the pushButton State
  if (switchState4 != oldSwitchState4){
    oldSwitchState4 = switchState4;
    if (switchState4 == HIGH){      
      if(!lightsOn4){
        //client.publish("bozaSub", "3");
        digitalWrite(LED4, HIGH);
        lightsOn4 = 1;
      }
       else{
        //client.publish("bozaSub", "2");
        digitalWrite(LED4, LOW);
        lightsOn4 = 0;
       }
    } 
  }

  switchState5 = digitalRead(BUTTON5); // read the pushButton State
  if (switchState5 != oldSwitchState5){
    oldSwitchState5 = switchState5;
    if (switchState5 == HIGH){      
      if(!lightsOn5){
        //client.publish("bozaSub", "3");
        digitalWrite(LED5, HIGH);
        lightsOn5 = 1;
      }
       else{
        //client.publish("bozaSub", "2");
        digitalWrite(LED5, LOW);
        lightsOn5 = 0;
       }
    } 
  }

  switchState6 = digitalRead(BUTTON6); // read the pushButton State
  if (switchState6 != oldSwitchState6){
    oldSwitchState6 = switchState6;
    if (switchState6 == HIGH){      
      if(!lightsOn6){
        //client.publish("bozaSub", "3");
        digitalWrite(LED6, HIGH);
        lightsOn6 = 1;
      }
       else{
        //client.publish("bozaSub", "2");
        digitalWrite(LED6, LOW);
        lightsOn6 = 0;
       }
    } 
  }
}
