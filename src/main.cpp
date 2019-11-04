#include <Arduino.h>
#include <LibRobus.h>

//programmation capteur de proximité marquis
#define capteur0 A0
// #define capteur1 A1
void capteurDistance(int distancemin)
{
  int roue, roue2;
  int resultatCm, mesure;
  mesure = analogRead(capteur0);
  resultatCm=(6787.0/(mesure-3.0))-4.0;
  while(resultatCm >= distancemin)
  {
    Serial.print(resultatCm);
     Serial.print("\n");
      if(resultatCm <= distancemin)
      {
        MOTOR_SetSpeed(roue,0);
        MOTOR_SetSpeed(roue2,0);
      }
      delay(1000);
    mesure = analogRead(capteur0);
    resultatCm=(6787.0/(mesure-3.0))-4.0;
  }

}
//int ROBUS_ReadIR(uint8_t id)

//EQUIPE 2 ICI
//Constant use for tuning PID
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

void tourner(int angle, int roue1, int roue2)
{
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  int nombreTick = 20 * angle * 3200 / (360 * 8) / 2;

  MOTOR_SetSpeed(roue1, init_speed);
  MOTOR_SetSpeed(roue2, init_speed * -1);
  while (nombreTick <= ENCODER_Read(roue1))
  {
    //wait
  }
  MOTOR_SetSpeed(roue1, 0);
  MOTOR_SetSpeed(roue2, 0);
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
    capteurDistance(15);

    if (ROBUS_IsBumper(3))
    {
      tourner(90, 0, 1);
      tourner(90, 1, 0);
    }
  }
}