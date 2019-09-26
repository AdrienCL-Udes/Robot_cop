#include <Arduino.h>
#include <LibRobus.h>

//EQUIPE 2 ICI
// Constant use for tuning PID
const float Pfactor = 0.00005;
const float Ifactor = 0.00001;

// Init varaible for motor control
int integral = 0;
const float init_speed = 0.6;
const float min_speed = 0.5;
const float max_speed = 0.7;
const float min_adjust = -0.04;
const float max_adjust = 0.04;
float current_speed = init_speed;

// This fonction will adjuste the speed of motor 0
// to match motor 1
void PID()
{
  int current_position = ENCODER_Read(0);

  int error = ENCODER_Read(1) - current_position;
  integral = integral + error;
  int value_integral = integral;

  float adjustment = Pfactor * error + Ifactor * value_integral;

  if (adjustment < min_adjust)
  {
    adjustment = min_adjust;
  }
  if (adjustment > max_adjust)
  {
    adjustment = max_adjust;
  }

  current_speed = current_speed + adjustment;

  if (current_speed < min_speed)
  {
    current_speed = min_speed;
  }
  if (current_speed > max_speed)
  {
    current_speed = max_speed;
  }

  MOTOR_SetSpeed(0, current_speed);
  Serial.print("integral: ");
  Serial.println(integral);
  Serial.print("adjustment: ");
  Serial.println(adjustment);
  Serial.print("current_speed: ");
  Serial.println(current_speed);
  Serial.print("current_position: ");
  Serial.println(current_position);
  Serial.print("error: ");
  Serial.println(error);
  Serial.println();
  Serial.println();
  delay(100);
}

void MOVE_forward(int distance)
{
  //int runTime = getRuntime(distance);
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  MOTOR_SetSpeed(0, init_speed);
  MOTOR_SetSpeed(1, init_speed);
  while (ENCODER_Read(0) <= 32000)
  {
    PID();
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
}

//PUT THESE AT THE END
void setup()
{
  // put your setup code here, to run once:
  BoardInit();
}
//THIS TOO
void loop()
{
  // put your main code here, to run repeatedly:
  while (1)
  {
    if (ROBUS_IsBumper(3))
    {
      MOVE_forward(0);
      integral = 0;
      current_speed = init_speed;
    }
  }
}