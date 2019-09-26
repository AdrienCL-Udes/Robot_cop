#include <Arduino.h>
#include <LibRobus.h>
#define MASTERSPEED 0.5
#define ROTATION 3200.00
int lastMaster = 0;

float calc(int master, int slave)
{
  float correction = MASTERSPEED;
  float diff;
  diff = (master - slave)/ROTATION;

  if(diff>=0)
  {
    correction += diff;
  }
  else
  {
    correction -=diff;
  }
  
  return correction;

}

void WriteEncodeur()
{
  Serial.print("left wheel: ");
  Serial.print(ENCODER_Read(0));
  Serial.println();
  Serial.print("Right wheel: ");
  Serial.print(ENCODER_Read(1));
  Serial.println();
  Serial.println();
  delay(50);
}

void PID()
{
  
  lastMaster = ENCODER_Read(1);
  int wheelSlave = ENCODER_Read(0);
  float correct = calc(lastMaster,wheelSlave);
  Serial.print("slave speed: ");
  Serial.println(correct);
  MOTOR_SetSpeed(1,MASTERSPEED);
  MOTOR_SetSpeed(0,correct);
}

void setup() 
{
  BoardInit();
}

void loop()
{
  PID(); 
  if(lastMaster == 0 || lastMaster != ENCODER_Read(1))
  {
    WriteEncodeur();
  }
  
}