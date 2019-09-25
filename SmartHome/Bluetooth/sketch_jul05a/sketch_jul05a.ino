#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3); // RX | TX

char c = ' ';
const int ruter = 12; 
const int sijalica = 13; 
void setup() 
{
    Serial.begin(9600);
    Serial.println("Arduino is ready");
    Serial.println("Remember to select Both NL & CR in the serial monitor");
    BTserial.begin(9600); //change to match your BT module
    pinMode(ruter, OUTPUT);
    pinMode(sijalica, OUTPUT);
    digitalWrite(ruter, HIGH);
    digitalWrite(sijalica, LOW);
}
 
void loop()
{
    if (BTserial.available())
    {  
        c = BTserial.read();
        Serial.write(c);
        if (c == '1'){
          digitalWrite(ruter, HIGH);
        }
        else if (c == '2'){
          digitalWrite(ruter, LOW);
        }
        else if (c == '3'){
          digitalWrite(sijalica, HIGH);
        }
        else if (c == '4'){
          digitalWrite(sijalica, LOW);
        }
    }
    
    if (Serial.available()) // Keep reading from Arduino Serial Monitor and send to HC-05
    {
        c =  Serial.read();
        BTserial.write(c);  
    }
 
}
