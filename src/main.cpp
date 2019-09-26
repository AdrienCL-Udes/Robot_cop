#include <Arduino.h>
#include <LibRobus.h>
#define MASTERSPEED 0.5

float calc(int master, int slave)
{
  float diff = (master - slave)/float(master);
  int percent;
  if(diff>=0)
  {
    percent = 1 + diff;
  }
  else
  {
    percent = 1-diff;
  }
  float correction = MASTERSPEED*percent;
  return correction;

}

void PID()
{
  int wheelMaster = ENCODER_Read(1);
  int wheelSlave = ENCODER_Read(0);
  float correct = calc(wheelMaster,wheelSlave);
  MOTOR_SetSpeed(1,MASTERSPEED);
  MOTOR_SetSpeed(0,correct);
}

void reset()
{
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  MOTOR_SetSpeed(1,MASTERSPEED);
  MOTOR_SetSpeed(0,MASTERSPEED);
}

void WriteEncodeur()
{
  Serial.print("left wheel: ");
  Serial.print(ENCODER_Read(0)+"\n");
  Serial.print("Right wheel: ");
  Serial.print(ENCODER_Read(1)+"\n");
}

void setup() 
{
  reset();
  BoardInit();
}

void loop()
{
  PID();
  WriteEncodeur();  
  
}