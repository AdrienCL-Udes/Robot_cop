#include <Arduino.h>
#include <LibRobus.h>
#include "SD.h"     //Lib to read SD card
#include "TMRpcm.h" //Lib to play auido
#include "SPI.h"    //SPI lib for SD card

/*
Arduino Based Music Player
 ** MOSI - pin 51
 ** MISO - pin 50 *BRISER*
 ** SCK - pin 52
 ** ss - pin 53
*/
#define SPEAKER_PIN 11
#define SD_PIN 53

#define PIN_FOLLOWLINE A2
#define PIN_DISTANCE A4 //Robot A // A3 //Robot B
#define PIN_RED 40
#define PIN_BLUE 41

#define PIN_NB 2
#define VITESSE_AVANT 0.2
#define VITESSE_RECULE -0.2

TMRpcm music; //Lib object is named "music"

int led = 2;

void allumerled(int &i)
{
  if (i % 2 == 1)
  {
    digitalWrite(PIN_RED, HIGH);
    delay(250);
    digitalWrite(PIN_RED, LOW);
    i = 2;
  }
  else
  {
    digitalWrite(PIN_BLUE, HIGH);
    delay(250);
    digitalWrite(PIN_BLUE, LOW);
    i = 1;
  }
}

//Fonction qui retourne 1 quand il detecte une ligne noir
//Aussi non, elle retourne 0
int IsBlackLine()
{
  Serial.println(analogRead(PIN_FOLLOWLINE));
  if (analogRead(PIN_FOLLOWLINE) > 200)
  {
    return false;
  }

  return true;
}

void ouvrirBras()
{
  Serial.println("test");

  for (int j = 0; j < 2; j++)
  {
    allumerled(led);
  }
  SERVO_SetAngle(1, 165);
  for (int j = 0; j < 8; j++)
  {
    allumerled(led);
  }
}

void fermerBras()
{
  SERVO_SetAngle(1, 65);
  allumerled(led);
}

int distanceObjet()
{
  int resultatCm;
  resultatCm = (6787.0 / (analogRead(PIN_DISTANCE) - 3.0)) - 4.0;
  if(resultatCm < 0){
    resultatCm *= -1;
  }
  return resultatCm;
}

//Cette fonction fait tourner le robot avec les deux moteurs avec le sens et l'angle entré par l'utilisateur
//roue = 0, le robot tourne vers la droite
//roue = 1, le robot tourne vers la gauche
//angle est en degré
void tourner2Roue(unsigned int angle, int roue1)
{
  int roue2, temps;
  temps = (float)1050 * angle / 90;

  if (roue1 == LEFT)
  {
    roue2 = RIGHT;
  }
  else
  {
    roue2 = LEFT;
  }

  MOTOR_SetSpeed(roue1, 0.2);
  MOTOR_SetSpeed(roue2, -0.2);
  delay(temps);
  MOTOR_SetSpeed(roue1, 0);
  MOTOR_SetSpeed(roue2, 0);
  delay(200);
}

void avancer(float speed)
{
  MOTOR_SetSpeed(LEFT, speed);
  MOTOR_SetSpeed(RIGHT, speed);
}

void arreter()
{
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
  delay(200);
}

void avertissement()
{
  for (int j = 0; j < 2; j++)
  {
    music.play("ecartez.wav");

    for (int i = 0; i < 11; i++)
    {
      allumerled(led);
    }
  }

  if (distanceObjet() < 9)
  {
    music.play("avertis.wav");

    for (int i = 0; i < 10; i++)
    {
      allumerled(led);
    }

    for (int j = 0; j < 2; j++)
    {
      allumerled(led);
    }
    SERVO_SetAngle(0, 165);
    for (int j = 0; j < 8; j++)
    {
      allumerled(led);
    }

    allumerled(led);

    fermerBras();
  }
}

void setup()
{
  BoardInit();
  pinMode(53, OUTPUT);
  pinMode(43, OUTPUT);
  digitalWrite(43, HIGH);
  pinMode(SPEAKER_PIN, OUTPUT);
  music.speakerPin = SPEAKER_PIN;

  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_RED, OUTPUT);

  pinMode(PIN_FOLLOWLINE, INPUT_PULLUP);
  pinMode(PIN_DISTANCE, INPUT);
  fermerBras();

  SERVO_Enable(0);
  SERVO_Enable(1);

  if (!SD.begin(SD_PIN))
  {
    Serial.println("SD fail");
  }
  else
  {
    Serial.println("SD Good");
  }
  music.setVolume(5);
  //music.play("test.wav");
}

void loop()
{
  allumerled(led);

  if (ROBUS_IsBumper(RIGHT))
  {
    for (int i = 0; i < 2; i++)
    {
      allumerled(led);
      avancer(VITESSE_AVANT);

      for (int i = 0; i < 10; i++)
      {
        allumerled(led);
      }

      arreter();

      tourner2Roue(90, 1);
      delay(100);

      for (int j = 0; j < 18; j++)
      {
        allumerled(led);
        tourner2Roue(11, 0);

        if (distanceObjet() < 9)
        {
          ouvrirBras();

          fermerBras();
        }
      }
      tourner2Roue(87, 1);
    }

    for (int i = 0; i < 2; i++)
    {
      allumerled(led);
      music.play("sirene.wav");
      avancer(VITESSE_AVANT);

      for (int i = 0; i < 10; i++)
      {
        allumerled(led);
      }

      arreter();
      music.play("sirene.wav");
      tourner2Roue(90, 1);
      delay(100);

      for (int j = 0; j < 18; j++)
      {
        allumerled(led);
        tourner2Roue(11, 0);

        if (distanceObjet() < 15)
        {
          for (int j = 0; j < 2; j++)
          {
            music.play("ecartez.wav");

            for (int i = 0; i < 11; i++)
            {
              allumerled(led);
            }
          }

          if (distanceObjet() < 15)
          {
            music.play("avertis.wav");

            for (int i = 0; i < 18; i++)
            {
              allumerled(led);
            }

            music.play("sirene.wav");

            avancer(0.3);
            for (int i = 0; i < 4; i++)
            {
              allumerled(led);
            }
            arreter();
            delay(200);

            avancer(-0.3);
            for (int i = 0; i < 4; i++)
            {
              allumerled(led);
            }
            arreter();
            delay(200);
          }
          music.play("sirene.wav");
          delay(1000);
        }
      }
      tourner2Roue(87, 1);
      music.play("sirene.wav");
    }
    avancer(VITESSE_AVANT);
    do
    {
      allumerled(led);
    } while (IsBlackLine());
  }

  if (ROBUS_IsBumper(LEFT))
  {
    do
    {
      allumerled(led);
      if (distanceObjet() < 15)
      {
        for (int j = 0; j < 2; j++)
        {
          music.play("ecartez.wav");

          for (int i = 0; i < 11; i++)
          {
            allumerled(led);
          }
        }
        
        if (distanceObjet() < 14)
        {
          music.play("avertis.wav");

          for (int i = 0; i < 18; i++)
          {
            allumerled(led);
          }

          music.play("sirene.wav");
          avancer(0.4);
          for (int i = 0; i < 4; i++)
          {
            allumerled(led);
          }
          arreter();
          delay(200);

          avancer(-0.4);
          for (int i = 0; i < 4; i++)
          {
            allumerled(led);
          }
          arreter();
          delay(1000);
        }
      }
      
    } while (!ROBUS_IsBumper(RIGHT));
  }

  if (ROBUS_IsBumper(REAR))
  {
    avancer(VITESSE_AVANT);
    do
    {
      allumerled(led);
      if (distanceObjet() < 10)
      {
        arreter();
        for (int i = 0; i < 3; i++)
        {
          AX_BuzzerON(500, 1000);
          delay(1000);
          allumerled(led);
        }

        if (distanceObjet() < 10)
        {
          ouvrirBras();
          fermerBras();
        }

        avancer(VITESSE_AVANT);
      }
    } while (IsBlackLine());
    arreter();
  }

  if (ROBUS_IsBumper(FRONT))
  {
    music.play("test.wav");
  }
}