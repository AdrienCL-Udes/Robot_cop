#include <Arduino.h>
#include <LibRobus.h>
#include "SD.h"     //Libraire pour lire la carte SD 
#include "TMRpcm.h" //Libraire pour jouerl'audio
#include "SPI.h"    //SPI de la libraire pour la carte SD

/*
Arduino Based Music Player
 ** MOSI - pin 51
 ** MISO - pin 50 *BRISER*
 ** SCK - pin 52
 ** ss - pin 53
*/
#define SPEAKER_PIN 11
#define SD_PIN 53

#define PIN_SUIVEUR_LIGNE A2
#define PIN_DISTANCE A4 
#define PIN_ROUGE 40
#define PIN_BLEU 41

#define VITESSE_AVANT 0.2
#define VITESSE_RECULE -0.2

TMRpcm music; //Lib object est nommé "music"

int del = 2;

//Fonction pour allumer les DEL sur le robot et éteindre les DEL après un 250 millisecondes
//Le paramètre couleur défini la couleur des DEL qu'elle active
//Si couleur est un chiffre pair la fonction allume les DEL rouges, si couleur est un nombre impaire la fonction allume les DEL bleus
void allumerDel(int &couleur)
{
  if (couleur % 2 == 1) 
  {
    digitalWrite(PIN_ROUGE, HIGH);
    delay(250);
    digitalWrite(PIN_ROUGE, LOW);
    couleur = 2;
  }
  else
  {
    digitalWrite(PIN_BLEU, HIGH);
    delay(250);
    digitalWrite(PIN_BLEU, LOW);
    couleur = 1;
  }
}

//Fonction qui lis une valeur de tension du suiveur de ligne
//La fonction renvoi false quand le suiveur de ligne détecte du noir et true quand il ne déctecte pas de noir
int estPasNoir()
{
  Serial.println(analogRead(PIN_SUIVEUR_LIGNE));
  if (analogRead(PIN_SUIVEUR_LIGNE) > 200)
  {
    return false;
  }

  return true;
}

//Fonction pour ouvrir le bras du robot
void ouvrirBras()
{
  SERVO_SetAngle(1, 165);
  //La boucle for permet d'allumer les DEL en même temps que d'ouvir le bras
  for (int j = 0; j < 8; j++)
  {
    allumerDel(del);
  }
}

//Fonction pour ouvrir le bras du robot
void fermerBras()
{
  SERVO_SetAngle(1, 65);
  //La boucle for permet d'allumer les DEL en même temps que d'ouvir le bras
  for (int j = 0; j < 8; j++)
  {
    allumerDel(del);
  }
}

//Fonction qui détectent la distance entre un objet et le capteur de distance et qui renvoie cette distance en CM
int distanceObjet()
{
  int resultatCm;
  resultatCm = (6787.0 / (analogRead(PIN_DISTANCE) - 3.0)) - 4.0;

  //Quand le capteur de distance ne détecte pas d'objet à une distance maximale de 80 cm il renvoi des valeurs aléatoire
  //Alors, pour ne pas avoir distance négative qui affecterait les autres fonctions du code, resultatCm devient positif
  if(resultatCm < 0){
    resultatCm *= -1;
  }
  return resultatCm;
}

//Cette fonction fait tourner le robot avec les deux moteurs avec le sens et l'angle entré par l'utilisateur
//roue1 = 0, le robot tourne vers la droite
//roue1 = 1, le robot tourne vers la gauche
//angle est un entier positif en degré
void tourner2Roue(unsigned int angle, int roue1)
{
  int roue2, temps;

  //Calcule le temps nécessaire pour que le robot tourne de l'angle en entré
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

//Fonction pour faire avancer les moteur du robot à une vitesse entré en paramètre 
void avancer(float vitesse)
{
  MOTOR_SetSpeed(LEFT, vitesse);
  MOTOR_SetSpeed(RIGHT, vitesse);
}

//Fonction pour faire arreter les moteurs du robbot
void arreter()
{
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
  delay(200);
}

//Fonction pour faire la démonstration de la fonctionalité de l'audio
/*
Au début de la fonction le robot va faire jouer son son de sirene. Le robot va être 
dans cette fontion tant que le bouton de droite n'est pas appuyer et il va allumer 
ses DEL pendant tout ce temps. S'il détectent un objet à distance de 15 cm ou moins,
il va faire jouer l'audio de ecatez.wav 2 fois pour demander à celui-ci de s'écarter.
Si l'objet est toujours encore à un distance de 15 cm après les avertissements. Le 
robot va jouer l'audio avertis.wav pour dire à celui-ci qu'il a été averti. Après le 
robot va jouer son son de sirene et il va se mettre avancer pour pousser l'objet et
il va reculer pour se remettre à sa position initiale.
*/
void fonctionnalitesAudio() {
  music.play("sirene.wav");
  do
  {
    allumerDel(del);
    if (distanceObjet() < 15)
    {
      for (int j = 0; j < 2; j++)
      {
        music.play("ecartez.wav");

        for (int i = 0; i < 11; i++)
        {
          allumerDel(del);
        }
      }
      
      if (distanceObjet() < 15)
      {
        music.play("avertis.wav");

        for (int i = 0; i < 18; i++)
        {
          allumerDel(del);
        }

        music.play("sirene.wav");
        avancer(0.4);
        for (int i = 0; i < 4; i++)
        {
          allumerDel(del);
        }
        arreter();
        delay(200);

        avancer(-0.4);
        for (int i = 0; i < 4; i++)
        {
          allumerDel(del);
        }
        arreter();
        delay(1000);
      }
    }
    
  } while (!ROBUS_IsBumper(RIGHT));
}

//Fonction pour la démonstration de toutes les fonctionnalités sauf l'audio, car celle-ci empêche le servomoteur de fonctionner
/*
Le robot va se mettre à avancer jusqu'à ce qu'il détetecte du noir. 
Pendant qu'il avance, s'il détecte un objet à une distance de 10 cm ou moins de lui,
il va l'avertire avec un buzzer trois fois. Si l'objet est toujours devant lui après 
les trois avertissement, le robot va ouvrir son bras pour repousser l'objet.
Pendant tout le temps qu'il avance, le robot va allumer ses DEL.
*/
void fonctionnalitesSansAudio() {
  avancer(VITESSE_AVANT);
  do
  {
    allumerDel(del);
    if (distanceObjet() < 10)
    {
      arreter();
      for (int i = 0; i < 3; i++)
      {
        AX_BuzzerON(500, 1000);
        delay(1000);
        allumerDel(del);
        
      }

      if (distanceObjet() < 10)
      {
        ouvrirBras();
        fermerBras();
      }

      avancer(VITESSE_AVANT);
    }
  } while (estPasNoir());
  arreter();
}

//Fonction pour initialiser le robot
void setup()
{
  BoardInit();
  pinMode(53, OUTPUT);
  pinMode(43, OUTPUT);
  digitalWrite(43, HIGH);
  pinMode(SPEAKER_PIN, OUTPUT);
  music.speakerPin = SPEAKER_PIN;

  pinMode(PIN_BLEU, OUTPUT);
  pinMode(PIN_ROUGE, OUTPUT);

  pinMode(PIN_SUIVEUR_LIGNE, INPUT_PULLUP);
  pinMode(PIN_DISTANCE, INPUT);
  fermerBras();

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
}

//Fonction pour montrer toutes les fonctionalités du robot
void loop()
{
  allumerDel(del);

  if (ROBUS_IsBumper(LEFT))
  {
    fonctionnalitesAudio();
  }

  if (ROBUS_IsBumper(REAR))
  {
    fonctionnalitesSansAudio();
  }
}