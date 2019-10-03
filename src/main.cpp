#include <Arduino.h>
#include <LibRobus.h>
#define MASTERSPEED 0.3
#define ROTATION 3200.00
#define WHEEL_SIZE 7.62
#define TICK_CM 133.7233
int lastMaster = 0;
//dhfuewhduifhuhfushufhdutest doom
<<<<<<< HEAD

void PID();

void forward(float cm)
{
  bool end = false;
  while(end == false)
  {
    PID();
    if(ENCODER_Read(1) >= (TICK_CM * cm))
    {
      end = true;
      MOTOR_SetSpeed(0,0);
      MOTOR_SetSpeed(1,0);
      ENCODER_Reset(0);
      ENCODER_Reset(1);
    }
  }
}

void turn(int direction)
{
  bool end = false;
  switch (direction)
  {
  case 45:
    MOTOR_SetSpeed(0,0.2);
    while(end == false) {
      if(ENCODER_Read(0) >= 133.673443*15)
      {
        end = true;
        MOTOR_SetSpeed(0,0);
        ENCODER_Reset(0);
      }
    }
    break; 
  case 90:
    MOTOR_SetSpeed(0,0.2);
    while(end == false) {
      if(ENCODER_Read(0) >= 133.673443*30)
      {
        end = true;
        MOTOR_SetSpeed(0,0);
        ENCODER_Reset(0);
      }
    }
    break;
  case -45:
    MOTOR_SetSpeed(1,0.2);
    while(end == false) {
      if(ENCODER_Read(1) >= 133.673443*15)
      {
        end = true;
        MOTOR_SetSpeed(1,0);
        ENCODER_Reset(1);
      }
    }
    break;
  case -90:
    MOTOR_SetSpeed(1,0.2);
    while(end == false) {
      if(ENCODER_Read(1) >= 133.673443*30)
      {
        end = true;
        MOTOR_SetSpeed(1,0);
        ENCODER_Reset(1);
      }
    }
    break;
  
  default:
    break;
  }
}

=======
>>>>>>> equipe_1
float calc(int master, int slave)
{
  float correction = MASTERSPEED;
  float diff;
  diff = (slave - master)/ROTATION;

  if(diff>=0)
  {
    correction = correction * (1.00 + diff);
  }
  else
  {
    correction = correction * (1.00 - diff);
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
  if(lastMaster == 0 || lastMaster != ENCODER_Read(1))
  {
    WriteEncodeur();
  }
  if(ROBUS_IsBumper(3))
  {
    forward(90);
    turn(90);
    forward(50);
    turn(-90);
  }
  
}