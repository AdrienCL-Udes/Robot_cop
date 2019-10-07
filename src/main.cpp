#include <Arduino.h>
#include <LibRobus.h>
#define ROTATION 3200.00
#define WHEEL_SIZE 7.62
#define TICK_CM 133.7233
int lastMaster = 0;
float masterSpeed = 0.1;
float speedRate = 0.004;

void PID(bool);

void forward(float cm)
{
  bool end = false;
  masterSpeed = 0.1;
  bool haveToSlowDown = false;
  while(end == false)
  {
    Serial.print("Speed: ");
    Serial.println(masterSpeed);
    PID(haveToSlowDown);
    haveToSlowDown = (TICK_CM*cm)*0.65 <= ENCODER_Read(1);
    Serial.print("SlowDown: ");
    Serial.println(haveToSlowDown);

    Serial.println();
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

float calc(int master, int slave, bool haveToSlowDown)
{
  if(!haveToSlowDown)
  {
    if(masterSpeed < 1)
    {
      masterSpeed = masterSpeed + speedRate;
    }
  }
  else
  {
    if(masterSpeed >= 0.10)
    {
      masterSpeed = masterSpeed * 0.97;
    }
  }
  float correction = masterSpeed;
  float diff;
  diff = (master - slave)/ROTATION;
  //Serial.println(diff);

  if(diff>0)
  {
    correction = correction * (1.00 + diff);
  }
  else if(diff < 0)
  {
    correction = correction * (1.00 + diff);
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

void PID(bool haveToSlowDown)
{
  
  lastMaster = ENCODER_Read(1);
  int wheelSlave = ENCODER_Read(0);
  float correct = calc(lastMaster,wheelSlave,haveToSlowDown);
  //Serial.print("slave speed: ");
  //Serial.println(correct);
  MOTOR_SetSpeed(1,masterSpeed);
  MOTOR_SetSpeed(0,correct);
}

void setup() 
{
  BoardInit();
}

void loop()
{
  // if(lastMaster == 0 || lastMaster != ENCODER_Read(1))
  // {
  //   WriteEncodeur();
  // }
  if(ROBUS_IsBumper(0))
  {
    forward(60);
  }

  if(ROBUS_IsBumper(3))
  {
    forward(120);//
    turn(-90);//
    forward(68);//
    turn(90);//
    forward(95);//
    turn(45);//
    forward(160);//
    turn(-90);//
    forward(50);//
    turn(45);//
    forward(110);//
  }
  
}