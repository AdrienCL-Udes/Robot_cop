#include <Arduino.h>
#include <LibRobus.h>
#define MASTERSPEED 0.3
#define ROTATION 3200.00
#define TICK_CM 133.7233
int lastMaster = 0;

void acc()
{
  float speed = 0.0;
  while (speed < MASTERSPEED)
  {
    MOTOR_SetSpeed(0, speed);
    MOTOR_SetSpeed(1, speed);
    speed += 0.05;
    delay(50);
  }
}
void dec()
{
  float speed = MASTERSPEED;
  while (speed > 0)
  {
    MOTOR_SetSpeed(0, speed);
    MOTOR_SetSpeed(1, speed);
    speed -= 0.05;
    delay(50);
  }
}

void PID();

void forward(float cm)
{
  bool end = false;
  acc();
  while (end == false)
  {
    PID();
    if (ENCODER_Read(1) >= (TICK_CM * cm) - 200)
    {
      end = true;
      dec();
      ENCODER_Reset(0);
      ENCODER_Reset(1);
    }
  }
}

//Calcule le nombre de pulse que doit faire la roue selon les paramètres reçus par la fonction
//rayonRoue et rayonArc sont en cm et angle est en degrès
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

  while (pulseEncodeur < pulse - 100)
  {
    pulseEncodeur = ENCODER_Read(roue);
  }

  MOTOR_SetSpeed(roue, 0);
  MOTOR_SetSpeed(roue2, 0);
  ENCODER_Reset(roue);
  ENCODER_Reset(roue2);
}

float calc(int master, int slave)
{
  float correction = MASTERSPEED;
  float diff;
  diff = (master - slave) / ROTATION;
  Serial.println(diff);

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

void go(int *tabData, char *tabMove, int tabSize)
{
  for (int i = 0; i < tabSize; i++)
  {
    switch (tabMove[i])
    {
    case 'F':
      forward(tabData[i]);
      break;

    case 'R':
      tourner2Roue(tabData[i], 0);
      break;

    case 'L':
      tourner2Roue(tabData[i], 1);
      break;

    default:
      break;
    }

    delay(300);
  }
}

void setup()
{
  BoardInit();
}

void loop()
{
  if (ROBUS_IsBumper(3))
  {
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    int tabData[] =  {223,  90, 92,  100, 50,  90,   50,  85,  95,  90,  42,  90, 120, 180, 248};
    char tabMove[] = {'F', 'L', 'F', 'R', 'F', 'R', 'F', 'L', 'F', 'R', 'F', 'L', 'F', 'L', 'F'};

    int tabSize = sizeof(tabData);

    int *pointerTabData = tabData;
    char *pointerTabMove = tabMove;

    go(pointerTabData, pointerTabMove, tabSize);
  }
}