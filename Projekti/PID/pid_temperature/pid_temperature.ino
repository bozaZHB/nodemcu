#include <DallasTemperature.h>
#include <OneWire.h>
#include <PID_v1.h>

#define ONE_WIRE_BUS D1          
#define PIN_OUTPUT D2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);    

double Setpoint, Input, Output;
double Kp=5, Ki=3, Kd=3;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup(void){
  
  Serial.begin(9600); 
  sensors.begin();

  Setpoint = 27;

  myPID.SetMode(AUTOMATIC);
  
}

void loop(void){ 
  
  sensors.requestTemperatures(); 
  Input = sensors.getTempCByIndex(0);
  
  myPID.Compute();
  analogWrite(PIN_OUTPUT, Output);

  Serial.println("Temperature is: ");
  Serial.println(Input);  
  Serial.println(Output);  
  
  delay(500);

}
