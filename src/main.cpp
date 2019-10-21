#include <Arduino.h>
#include <LibRobus.h>
#include <Adafruit_TCS34725.h>

#define anode false

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS,TCS34725_GAIN_4X);
byte gammatable[256];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  if(colorSensor.begin()){
    Serial.println("Color sensor found");
  }
}

void loop()
{
  if(ROBUS_IsBumper(REAR))
  {
    uint16_t red,green,blue, clear = 0;

    colorSensor.enable();
    colorSensor.getRawData(&red,&green,&blue,&clear);
    float average = (red+green+blue)/3;
    float r,g,b = 0;

    r = red/average;
    g = green/average;
    b = blue/average;
    Serial.print("The RGB value are: RGB( ");
    Serial.print(r);
    Serial.print(", ");
    Serial.print(g);
    Serial.print(", ");
    Serial.print(b);
    Serial.println(" )");
  } 
}