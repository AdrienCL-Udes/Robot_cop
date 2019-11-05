#include <Arduino.h>
#include <LibRobus.h>

#define PIN_RIGHT A0
#define PIN_LEFT A2
#define PIN_MIDDLE A3
#define PIN_DISTANCE A4

#define VERT 0
#define BLEU 1
#define JAUNE 2
#define ROUGE 3

#define PIN_NB 3
#define SPEED_FORWARD 0.2
#define SPEED_BACK -0.2

//Cette fonction permet de faire tourner le servomoteur à un angle de 180 
void ouvrirAvecServomoteur() {
  SERVO_Enable(0);
  for(int i = 150; i > 60; i = i - 5){
    SERVO_SetAngle(0, i);
    delay(10);
  }
  SERVO_Disable(0);
}

//Cette fonction permet de faire tourner le servomoteur à un angle de 0
void fermerAvecServomoteur() {
  SERVO_Enable(0);
  for(int i = 60; i < 150; i = i + 5){
    SERVO_SetAngle(0, i);
    delay(10);
  }
  SERVO_Disable(0);
}


int distanceBalle() {
  int resultatCm;
  resultatCm=(6787.0/(analogRead(PIN_DISTANCE)-3.0))-4.0;
  return resultatCm;
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

void followLine(float speed)
{
  bool pinRight = getPinState(PIN_RIGHT);
  bool pinLeft = getPinState(PIN_LEFT);
  bool pinMiddle = getPinState(PIN_MIDDLE);

  if (pinMiddle && !pinRight && !pinLeft)
  {
    MOTOR_SetSpeed(LEFT, speed);
    MOTOR_SetSpeed(RIGHT, speed);
    //Serial.println("MIDDLE");
  }

  if (pinRight && !pinLeft)
  {
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, speed);
    //Serial.println("RIGHT");
  }

  if (pinLeft && !pinRight)
  {
    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, speed);
    //Serial.println("LEFT");
  }
}

//COLOR
//Vert = 0
//Bleu = 1
//Jaune = 2
//Rouge = 3
void goToColor(int color)
{
  int distance;
  MOTOR_SetSpeed(RIGHT, SPEED_FORWARD);
  MOTOR_SetSpeed(LEFT, SPEED_FORWARD);

  do
  {
    delay(200);
  } while (getPinState(PIN_RIGHT) && getPinState(PIN_LEFT) && getPinState(PIN_MIDDLE));

  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);

  switch (color)
  {
  case VERT:
    tourner2Roue(45, 0);
    break;

  case BLEU:
    tourner2Roue(135, 0);
    break;

  case JAUNE:
    tourner2Roue(45, 1);
    break;

  case ROUGE:
    tourner2Roue(135, 1);
    break;

  default:
    break;
  }

  MOTOR_SetSpeed(RIGHT, 0.3);
  MOTOR_SetSpeed(LEFT, 0.3);

  do
  {
    followLine(0.3);
    delay(200);
  } while (/* condition capteur de couleur */ 1);

  do{
    distance = distanceBalle();
  } while (distance > 10);
  
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);

  do {
    fermerAvecServomoteur();
    delay(300);
  } while(ROBUS_IsBumper(FRONT));

  MOTOR_SetSpeed(RIGHT, SPEED_BACK);
  MOTOR_SetSpeed(LEFT, SPEED_BACK);

  do
  {
    followLine(SPEED_BACK);
    delay(200);
  } while (getPinState(PIN_RIGHT) && getPinState(PIN_LEFT) && getPinState(PIN_MIDDLE));

  tourner2Roue(180, 0);

  ouvrirAvecServomoteur();

  MOTOR_SetSpeed(RIGHT, SPEED_BACK);
  MOTOR_SetSpeed(LEFT, SPEED_BACK);

  delay(750);

  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
}

void Move() {
  MOTOR_SetSpeed(LEFT, 0.15);
  MOTOR_SetSpeed(RIGHT, 0.15);
}

void setup()
{
  BoardInit();
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_MIDDLE, INPUT_PULLUP);
  pinMode(PIN_DISTANCE, INPUT);
  SERVO_Disable(0);
}

void loop()
{
  
  if (ROBUS_IsBumper(FRONT))
  {
    //goToColor(ROUGE);
    Serial.println(analogRead(PIN_DISTANCE));
    delay(500);
  }
}