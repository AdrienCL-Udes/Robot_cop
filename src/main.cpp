#include <Arduino.h>
#include <LibRobus.h>

//EQUIPE 2 ICI
// Constant use for tuning PID
const float Pfactor = 0.0002;
const float Ifactor = 0.00001;

// Init varaible for motor control
int integral = 0;
const float init_speed = 0.6;
float current_speed = init_speed;
const float Wheel_size_10 = 76.2;
const int numberTickWheel = 32000;

// This fonction will adjuste the speed of motor 0
// to match motor 1
void PID()
{
  int current_position = ENCODER_Read(0);

  int error = ENCODER_Read(1) - current_position;
  integral = integral + error;
  int error_value = error;

  float adjustment = Pfactor * error_value + Ifactor * integral;

  current_speed = current_speed + adjustment;

  MOTOR_SetSpeed(0, current_speed);

  if (current_position > 10000)
  {
    integral = 0;
    MOTOR_SetSpeed(0, init_speed);
  }
  delay(100);
}

void MOVE_forward(int distance)
{
  //int runTime = getRuntime(distance);
  int nbTour_10 = CALCUL_nbCompleteWheelRotation_10(distance);
  int nbTick = CALCUL_nbPartialWheelRotation(distance, nbTour_10);
  int tmpTour = 0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  MOTOR_SetSpeed(0, init_speed);
  MOTOR_SetSpeed(1, init_speed);
  while (nbTour_10 > tmpTour || ENCODER_Read(0) <= nbTick)
  {
    PID();
    if (ENCODER_Read(0) > 32000)
    {
      tmpTour = tmpTour + 1;
      ENCODER_Reset(0);
      ENCODER_Reset(1);
    }
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

int CALCUL_nbCompleteWheelRotation_10(float cm_distance)
{
  return cm_distance / Wheel_size_10;
}

int CALCUL_nbPartialWheelRotation(float cm_distance, int nbCompleteRotation)
{
  float leftover = cm_distance - nbCompleteRotation * Wheel_size_10;
  return leftover * numberTickWheel / Wheel_size_10;
}

//THIS TOO
void loop()
{
  // put your main code here, to run repeatedly:
  while (1)
  {
    if (ROBUS_IsBumper(3))
    {
      MOVE_forward(30);
      integral = 0;
      current_speed = init_speed;
    }
  }
}