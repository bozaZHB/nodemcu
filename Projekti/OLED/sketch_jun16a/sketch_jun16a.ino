/*

This is a work in progress but hopefully it will help someone else by providing
a base to start and work from.
Please check out my Youtube videos here and consider a thumbs up if this helped you!
Youtube : http://www.youtube.com/mkmeorg
Website, Forum and store are at http://mkme.org

0X3C is address for cheap ebay units
Used I2C Scanner to find address
SDA connected to pin Analog 4
SCL connected to pin Analog 5
3.3V voltage
*/

#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
int i=0;




void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  //display.display();  //sples skrin
  //delay(2000);
  //display.clearDisplay(); // Clear the buffer.
}

void loop()
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);//velicina teksta
  display.setTextColor(WHITE);
  display.println(i);//21 karakter jedan red kada je velicina 1, 10 karaktera za velicinu 2
  display.println("Mozes jos");
  display.display();
  delay (500);
  i++;
}
