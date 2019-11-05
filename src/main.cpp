#include <Arduino.h>
#include <LibRobus.h>

#define PIN_RIGHT A0
#define PIN_LEFT A1
#define PIN_MIDDLE A2
#define PIN_DISTANCE A3

#define PIN_NB 3
#define SPEED -0.2

void move(float speed)
{
  MOTOR_SetSpeed(RIGHT, speed);
  MOTOR_SetSpeed(LEFT, speed);
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
void tourner2Roue(unsigned int angle, int roue)
{
  int pulseEncodeur = 0, pulse, roue2;
  ENCODER_Reset(roue);

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
}

bool getPinState(int8_t pin)
{
  return analogRead(pin) > 650;
}

bool followLine()
{
  bool pinRight = getPinState(PIN_RIGHT);
  bool pinLeft = getPinState(PIN_LEFT);
  bool pinMiddle = getPinState(PIN_MIDDLE);

  if (pinMiddle && !pinRight && !pinLeft)
  {
    move(SPEED);
  }

  if (pinRight && !pinLeft)
  {
    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, SPEED);
  }

  if (pinLeft && !pinRight)
  {
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, SPEED);
  }

  if (!pinMiddle && !pinRight && !pinLeft)
  {
    MOTOR_SetSpeed(LEFT, SPEED);
    MOTOR_SetSpeed(RIGHT, SPEED);
  }

  if (pinMiddle && pinRight && pinLeft)
  {
    return false;
  }

  return true;
}

//COLOR
//Vert = 0
//Bleu = 1
//Jaune = 2
//Rouge = 3
void goToColor(int color)
{
  switch (color)
  {
  case 0:
    delay(300);
    move(-0.2);
    delay(3000);
    move(0.0);
    delay(300);

    tourner2Roue(45, LEFT);

    delay(200);

    move(SPEED);
    do
    {
      delay(100);
    } while (followLine());

    move(0.0);

    delay(200);

    move(0.2);

    delay(500);

    move(0.0);

    tourner2Roue(160, RIGHT);

    delay(200);

    move(0.2);

    delay(1000);
    break;

  case 1:
    delay(300);
    move(-0.2);
    delay(2000);
    move(0.0);
    delay(300);

    tourner2Roue(135, LEFT);

    delay(200);

    move(SPEED);

    delay(3000);

    tourner2Roue(30, LEFT);

    do
    {
      delay(100);
    } while (followLine());

    move(0.0);

    delay(200);

    move(0.2);

    delay(500);

    move(0.0);

    tourner2Roue(160, RIGHT);

    delay(200);

    move(0.2);

    delay(1000);
    break;

  case 2:
    tourner2Roue(45, 1);
    break;

  case 3:
    delay(300);
    move(-0.2);
    delay(3000);
    move(0.0);
    delay(300);

    tourner2Roue(30, RIGHT);

    delay(200);

    move(SPEED);
    do
    {
      delay(100);
    } while (followLine());

    move(0.0);

    delay(200);

    move(0.2);

    delay(500);

    move(0.0);

    tourner2Roue(190, LEFT);

    delay(200);

    move(0.2);

    delay(1000);
    break;

  default:
    break;
  }

  move(0.0);
  delay(200);
  SERVO_SetAngle(0, 80);
}

void goGrabBall()
{
  delay(200);
  tourner2Roue(45, 0);
  delay(200);

  MOTOR_SetSpeed(RIGHT, 0.2);
  MOTOR_SetSpeed(LEFT, -0.2);

  Serial.println(analogRead(PIN_DISTANCE));
  while (analogRead(PIN_DISTANCE) < 150)
  {
    delay(50);
    //wait
  }

  delay(100);
  move(0.0);

  delay(200);

  move(0.2);

  while (analogRead(PIN_DISTANCE) < 300)
  {
    delay(50);
    //wait
  }

  while (analogRead(PIN_DISTANCE) >= 200)
  {
    delay(50);
    //wait
  }

  delay(100);
  move(0.0);

  SERVO_SetAngle(0, 125);
  delay(500);
}

void setup()
{
  BoardInit();
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_MIDDLE, INPUT_PULLUP);
  pinMode(PIN_DISTANCE, INPUT);

  SERVO_Enable(0);
  SERVO_SetAngle(0, 80);
}

void loop()
{
  delay(1000);
  goGrabBall();

  goToColor(1);

  while (1)
  {
    delay(60000);
  }
}