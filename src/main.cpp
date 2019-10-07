#include <Arduino.h>
#include <LibRobus.h>

#define TICK_CM 133.7233
#define MASTERSPEED 0.3
#define ROTATION 3200.00
int lastMaster = 0;

float calc(int master, int slave)
{
  float correction = MASTERSPEED;
  float diff;
  diff = (master - slave) / ROTATION;
  //Serial.println(diff);

  if (diff > 0)
  {
    correction = correction * (1.00 + diff);
  }
  else if (diff < 0)
  {
    correction = correction * (1.00 + diff);
  }

  return correction;
}

void PID()
{
  lastMaster = ENCODER_Read(1);
  int wheelSlave = ENCODER_Read(0);
  float correct = calc(lastMaster, wheelSlave);
  Serial.print("slave speed: ");
  Serial.println(correct);
  MOTOR_SetSpeed(1, MASTERSPEED);
  MOTOR_SetSpeed(0, correct);
}

float calculerNbPulse(int angle, float rayonRoue, float rayonArc)
{
  float nbPulse;

  nbPulse = (rayonArc * angle * 3200) / (360 * rayonRoue);

  return nbPulse;
}

//Cette fonction fait tourner le robot avec les deux moteurs avec le sens et l'angle entré par l'utilisateur
//roue = 0, le robot tourne vers la droite
//+roue = 1, le robot tourne vers la gauche
//angle est en degré
void tourner1Roue(unsigned int angle, int roue)
{
  int pulseEncodeur = 0, pulse;

  pulse = calculerNbPulse(angle, 7.62 / 2, 19.3);

  MOTOR_SetSpeed(roue, 0.3);

  while (pulseEncodeur < pulse)
  {
    pulseEncodeur = ENCODER_Read(roue);
  }

  MOTOR_SetSpeed(roue, 0);
  ENCODER_Reset(roue);
}

//Cette fonction fait tourner le robot avec les deux moteurs avec le sens et l'angle entré par l'utilisateur
//roue = 0, le robot tourne vers la droite
//+roue = 1, le robot tourne vers la gauche
//angle est en degré
void tourner2Roue(unsigned int angle, int roue)
{
  int pulseEncodeur = 0, pulse, roue2;

  pulse = calculerNbPulse(angle / 2, 7.62 / 2, 19.3);

  if (roue == 0)
  {
    roue2 = 1;
  }
  else
  {
    roue2 = 0;
  }

  MOTOR_SetSpeed(roue, 0.2);
  MOTOR_SetSpeed(roue2, -0.2);

  while (pulseEncodeur < pulse - 50)
  {
    pulseEncodeur = ENCODER_Read(roue);
  }

  MOTOR_SetSpeed(roue, 0);
  MOTOR_SetSpeed(roue2, 0);
  ENCODER_Reset(roue);
  ENCODER_Reset(roue2);
}

//This function will make the robot move forward a
//given distance in CM
void forward(float cm)
{
  bool end = false;
  while (end == false)
  {
    PID();
    if (ENCODER_Read(1) >= (TICK_CM * cm))
    {
      end = true;
      MOTOR_SetSpeed(0, 0);
      MOTOR_SetSpeed(1, 0);
      ENCODER_Reset(0);
      ENCODER_Reset(1);
    }
  }
}

//PUT THESE AT THE END
void setup()
{
  // put your setup code here, to run once:
  BoardInit();
}

void loop()
{
  int ddelay = 500;
  // put your main code here, to run repeatedly:
  while (1)
  {
    if (ROBUS_IsBumper(3))
    {
      tourner2Roue(45, 0);
      /*forward(122);
      delay(ddelay);
      tourner2Roue(90, 1);
      delay(ddelay);
      forward(90);
      delay(ddelay);
      tourner2Roue(90, 0);
      delay(ddelay);
      forward(95);
      delay(ddelay);
      tourner2Roue(45, 0);
      delay(ddelay);
      forward(160);
      delay(ddelay);
      tourner2Roue(90, 1);
      delay(ddelay);
      forward(50);
      delay(ddelay);
      tourner2Roue(45, 0);
      delay(ddelay);
      forward(110);
      delay(ddelay);
      tourner2Roue(360, 1);
      delay(ddelay);*/
    }
  }
}