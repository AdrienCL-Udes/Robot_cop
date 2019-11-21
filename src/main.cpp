#include <Arduino.h>
#include <LibRobus.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>

#define PIN_RIGHT A0
#define PIN_LEFT A3
#define PIN_MIDDLE A2
#define PIN_DISTANCE A4

#define VERT 0
#define BLEU 1
#define JAUNE 2
#define ROUGE 3

#define PIN_NB 3
#define SPEED_FORWARD 0.3
#define SPEED_BACK -0.3

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

//Cette fonction permet de faire tourner le servomoteur à un angle de 180
void ouvrirAvecServomoteur(int i)
{
  SERVO_SetAngle(0, i);
  delay(1000);
}

//Cette fonction permet de faire tourner le servomoteur à un angle de 0
void fermerAvecServomoteur(int i)
{
  SERVO_SetAngle(0, i);
  delay(1000);
}

int distanceBalle()
{
  int resultatCm;
  resultatCm = (6787.0 / (analogRead(PIN_DISTANCE) - 3.0)) - 4.0;
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
//roue = 1, le robot tourne vers la gauche
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
  delay(200);
}

void Move(float speed)
{
  MOTOR_SetSpeed(LEFT, speed);
  MOTOR_SetSpeed(RIGHT, speed);
}

void StopMove()
{
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
  delay(200);
}

void setup()
{
  BoardInit();
  AudioInit();
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_MIDDLE, INPUT_PULLUP);
  pinMode(PIN_DISTANCE, INPUT);
  SERVO_Enable(0);
}

void loop()
{
  if (ROBUS_IsBumper(REAR))
  {
    ouvrirAvecServomoteur(155);
    Serial.println("lol");
  }
  if(ROBUS_IsBumper(LEFT)) {
      fermerAvecServomoteur(65);
  }
  if(ROBUS_IsBumper(RIGHT)) {
    Move(0.3);
    while(distanceBalle() > 15) {
        delay(250);
    }
    StopMove();
  }
  if(ROBUS_IsBumper(FRONT)) {
    AX_BuzzerON(500, 1000);
  }
}