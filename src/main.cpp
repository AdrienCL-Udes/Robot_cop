#include <Arduino.h>
#include <LibRobus.h>
#include <Adafruit_TCS34725.h>
// red == 0, green == 1, blue == 2, , yellow == 3

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS,TCS34725_GAIN_4X);

int getColorFromSensor()
{
  uint16_t red,green,blue, clear = 0;
  int r = 0; int g = 1; int b = 2; int y = 3;

  colorSensor.enable();
  colorSensor.getRawData(&red,&green,&blue,&clear);

  Serial.print("The RGB value are: TEST( ");
  Serial.print(red);
  Serial.print(", ");
  Serial.print(green);
  Serial.print(", ");
  Serial.print(blue);
  Serial.println(" )");

  if(red > green && red > blue)
  {
    if(red > 125 && green < 100 && blue < 105)
    {
      return r;
    }
  }
  if(green > red && green > blue)
  {
    if(green <= 125 && green >= 110 && blue > 100)
    {
      return g;
    }
  }
  if(blue > red && blue > green)
  {
    if(blue <= 160 && blue >= 135)
    {
      return b;
    }
    
  }
  if(red > blue && green > blue)
  {
    if(red >= 250 && green > 240)
    {
      return y;
    }
  }
  return 1000;
}

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
  
}
