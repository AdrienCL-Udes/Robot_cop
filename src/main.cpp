#include <Arduino.h>
#include <LibRobus.h>

//EQUIPE 2 ICI

float calculerNbPulse(int angle, float rayonRoue, float rayonArc);
void tourner(int angle, int roue);

void setup() {
  // put your setup code here, to run once:
  BoardInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(ROBUS_IsBumper(0)) {
      tourner(45, 0);
      delay(1000);

      tourner(90, 0);
      delay(1000);

      tourner(45, 1);
      delay(1000);

      tourner(90, 1);
      delay(1000);
  }
  
}

//Calcule le nombre de pulse que doit faire la roue selon les paramètres reçus par la fonction
//rayonRoue et rayonArc sont en cm et angle est en degrès
float calculerNbPulse(int angle, float rayonRoue, float rayonArc) {
  float nbPulse;

  nbPulse =  (rayonArc * angle*3200)/(360*rayonRoue);
  
  return nbPulse;
}

//Quand roue = 0, le robot tourne vers la droite
//Quand roue = 1, le robot tourne vers la gauche
void tourner(int angle, int roue) {
  int pulseEncodeur = 0, pulse;
  ENCODER_Reset(roue);
  
  pulse = calculerNbPulse(angle, 7.65/2 , 19.3);

  MOTOR_SetSpeed(roue, 0.6);

  while (pulseEncodeur <= pulse) {
    pulseEncodeur = ENCODER_Read(roue);

    if(ROBUS_IsBumper(1)) {
      MOTOR_SetSpeed(roue, 0);
    }
  }

  MOTOR_SetSpeed(roue, 0);
}

