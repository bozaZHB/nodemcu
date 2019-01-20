#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
 
const char* ssid = "e665e4";
const char* password = "262751838";
 
const char *host = "192.168.0.100";  
 
void setup() {
  delay(1000);
  Serial.begin(9600);
  WiFi.mode(WIFI_OFF);    
  delay(1000);
  WiFi.mode(WIFI_STA);      
  
  WiFi.begin(ssid, password);    
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 
}
 

void loop() {
  HTTPClient http;  
 
  String postData;

  postData = "email=nodemcu&name=proba";
  
  http.begin("http://192.168.0.100/proba/insert.php");       
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");  
 
  int httpCode = http.POST(postData);   
  String payload = http.getString(); 
 
  Serial.println(httpCode);
  Serial.println(payload);   
 
  http.end();
  
  delay(5000); 
}
