/*
Reads boost from MPX4250AP on Analog 0.
Converts vacuum readings to inHg.
*/
int mapsen = 0; // Set MAP sensor input on Analog port 0
float boost = 0; // Set boost value to 0
float mapval = 0; // Set raw map value to 0
volatile float peakboost = -30.0; // Set peak memory to low number so max displays correctly
float warnpsi = 20.5; // Set PSI for warning
float atmpsi = 13.9637; //Local atmospheric pressure
// include the library code:
#include <LiquidCrystal.h>
#include <LcdBarGraph.h>
#define REDLITE 6 //Red is connected to this Digital pin
#define GREENLITE 5 //Green is connected to this Digital pin
#define BLUELITE 3 //Blue is connected to this Digital pin
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
// you can change the overall brightness by range 0 -> 255
int brightness =255;
byte lcdNumCols = 16; // -- number of columns in the LCD
byte sensorPin = 0; // -- value for this example
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // -- creating LCD instance
LcdBarGraph lbg(&lcd, lcdNumCols);  // -- creating
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.clear();
  setBacklight(100,255,0);
  lcd.setCursor(4,0);
  lcd.print("Welcome");
  lcd.setCursor(4,1);
  lcd.print("Cameron");
  delay (1500);
  lcd.clear();
  lcd.setCursor(10,0);
  lcd.print("|");
}
void loop()
{
  // -- draw bar graph from the analog value readed
  lbg.drawValue( analogRead(sensorPin), 1024);
  // -- do some delay: frequent draw may cause broken visualization
  //delay(100);

 mapval= analogRead(mapsen); //Reads the MAP sensor raw value on analog port 0
 boost = ((((float)mapval/(float)1023+0.04)/.004)*.145)-atmpsi;
  if (boost <0) //If PSI is negative, convert to inHG and display VAC instead of PSI
  {
    //boost = boost*2.036021;
    lcd.setCursor(0,0);
    lcd.print("VAC");
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("PSI");
  }
  if (boost > peakboost) //Works the MAX
  {
    peakboost = boost;
    if (peakboost<-10) //Adjusts numbers over
    {
      lcd.setCursor(12,0);
      lcd.print(peakboost,1);
    }
    if (peakboost>-10)
    {
      lcd.setCursor(11,0);
      lcd.write(254);
      lcd.setCursor(12,0);
      lcd.print(peakboost,1);
    }
    if (peakboost >0)
    {
      lcd.setCursor(12,0);
      lcd.write(254);
      lcd.setCursor(13,0);
      lcd.print(peakboost,1);
    }
    if (peakboost >=10)
    {
      lcd.setCursor(12,0);
      lcd.print(peakboost,1);
    }
  }
  if (boost<-10)
  {
    lcd.setCursor(4,0);
    lcd.print(boost,1);
  }
  if (boost>-10)
  {
    lcd.setCursor(8,0);
    lcd.write(254);
    lcd.setCursor(4,0);
    lcd.print(boost,1);
  }
  if (boost >0)
  {
    lcd.setCursor(7,0);
    lcd.write(254);
    lcd.setCursor(4,0);
    lcd.print(boost,1);
  }
  if (boost > warnpsi) //If boost exceeds warnpsi then change backlight color
  {
    setBacklight(255,0,0);
  }
  else
  {
    setBacklight(100,255,0);
  }
  delay(50);
}
void setBacklight(uint8_t r, uint8_t g, uint8_t b) {
  // normalize the red LED - its brighter than the rest!
  r = map(r, 0, 255, 0, 200);
  g = map(g, 0, 255, 0, 255);
  r = map(r, 0, 255, 0, brightness);
  g = map(g, 0, 255, 0, brightness);
  b = map(b, 0, 255, 0, brightness);
  // common anode so invert!
  r = map(r, 0, 255, 255, 0);
  g = map(g, 0, 255, 255, 0);
  b = map(b, 0, 255, 255, 0);
  Serial.print("R = ");
  Serial.print(r, DEC);
  Serial.print(" G = ");
  Serial.print(g, DEC);
  Serial.print(" B = ");
  Serial.println(b, DEC);
  analogWrite(REDLITE, r);
  analogWrite(GREENLITE, g);
  analogWrite(BLUELITE, b);
}
