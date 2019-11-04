#include <Arduino.h>
#include <LibRobus.h>

//programmation capteur de proximité marquis
#define capteur0 A0
#define capteur1 A1
void setup()
{
Serial.begin(9600);
}
void loop()
{
  int sensorPins[]= {capteur0, capteur1}; //array contenant les pins pour chq capteur IR
  int distanceCapteurs[2]; //initialisation d'un array pour storer les valeurs de chq capteur
  int id=0; //initialisation de la variable id
  int roue, roue2;

  for (int i = 0; i < 2; i++) //loop de 0 a 1 pour passer a travers chq capteur IR
  {
    id = sensorPins[i]; //assigner le nom du pin a la variable id pour le capteur d'interet
    distanceCapteurs[i] = ROBUS_ReadIR(id); //assigner la sortie (distance) de ROBUS_READIR au array de valeur mesurees
  
    if(distanceCapteurs[i]<10) //distance minimal mesuré par le capteur (a tester)
    {
      MOTOR_SetSpeed(roue, 0); //initialisation de la vitesse a 0
      MOTOR_SetSpeed(roue2, 0); //initialisation de la vitesse a 0
    }
  }
}
int ROBUS_ReadIR(uint8_t id)