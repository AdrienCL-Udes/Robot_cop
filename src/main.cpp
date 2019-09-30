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

//Calcule le nombre de tour que de doit faire la roue selon les paramètres reçu par la fonction
float calculerNbPulse(int angle, float rayonRoue, float rayonArc) {
  float nbPulse;

  nbPulse =  (rayonArc * angle*3200)/(360*rayonRoue);
  

  return nbPulse;
}

//Angle entre 0 et 90
void tourner(int angle, int roue) {
  int pulseEncodeur = 0, pulse;
  ENCODER_Reset(roue);
  
  pulse = calculerNbPulse(angle, 1.5, 8);

  MOTOR_SetSpeed(roue, 0.6);

  while (pulseEncodeur <= pulse-2) {
    pulseEncodeur = ENCODER_Read(roue);

    if(ROBUS_IsBumper(1)) {
      MOTOR_SetSpeed(roue, 0);
    }
  }

  MOTOR_SetSpeed(roue, 0);
}

