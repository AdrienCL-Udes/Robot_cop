#include <Arduino.h>
#include <LibRobus.h>
#include "SD.h"     //Lib to read SD card
#include "TMRpcm.h" //Lib to play auido
#include "SPI.h"    //SPI lib for SD card

/*
Arduino Based Music Player
 ** MOSI - pin 51 - jaune
 ** MISO - pin 50***** - brun
 ** SCK - pin 52 - orange
 ** ss - pin 53 - blanc
*/
#define SPEAKER_PIN 11
#define SD_PIN 53

#define PIN_FOLLOWLINE A2
#define PIN_DISTANCE A4 //Robot A // A3 //Robot B

#define PIN_NB 2
#define SPEED_FORWARD 0.3
#define SPEED_BACK -0.3

TMRpcm music; //Lib object is named "music"

void allumerled()
{
  digitalWrite(22, HIGH);
  digitalWrite(2, HIGH);
  delay(250);
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  delay(250);
  digitalWrite(3, LOW);
}

//Fonction qui retourne 1 quand il detecte une ligne blanche
//Aussi non, elle retourne 0
int IsWhiteLine()
{
  if (analogRead(PIN_FOLLOWLINE) < 965 && analogRead(PIN_FOLLOWLINE) > 750)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

//Cette fonction permet de faire tourner le servomoteur à un angle i entre 0 et 180
void servomoteur(int i)
{
  SERVO_SetAngle(0, i);
  delay(1000);
}

void ouvrirBras()
{
  SERVO_Enable(0);
  servomoteur(165);
  allumerled();
  SERVO_Disable(0);
}

void fermerBras()
{
  SERVO_Enable(0);
  servomoteur(65);
  allumerled();
  SERVO_Disable(0);
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
  pinMode(53, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  music.speakerPin = SPEAKER_PIN;

  pinMode(22, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);

  pinMode(PIN_FOLLOWLINE, INPUT_PULLUP);
  pinMode(PIN_DISTANCE, INPUT);
  fermerBras();
}

void loop()
{
  allumerled();

  /*if (ROBUS_IsBumper(REAR)){
    Move(1.0);
    delay(500);
    StopMove();
  }
  if(ROBUS_IsBumper(LEFT)) {

  }
  if(ROBUS_IsBumper(RIGHT)) {
    
    Serial.println("lol");
    while(distanceBalle() > 15) {
        allumerled();
    }
    ouvrirBras();
    delay(500);
    fermerBras();
  }
  if(ROBUS_IsBumper(FRONT)) {
    AX_BuzzerON(500, 1000);
  }*/

  if(ROBUS_IsBumper(RIGHT)) {
    while(distanceBalle() > 15) {
        allumerled();
    }
    allumerled();
    ouvrirBras();
    
    allumerled();
    fermerBras();
    
    allumerled();
  }
  if(ROBUS_IsBumper(LEFT)) {
    Move(0.2);
    allumerled();
    while(IsWhiteLine() == 0) {
      delay(200);
      Serial.println(analogRead(PIN_FOLLOWLINE));
    }
    StopMove();
  }
  if(ROBUS_IsBumper(REAR)) {

    Serial.println(analogRead(PIN_FOLLOWLINE));
    delay(500);
  }
}