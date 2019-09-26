#include <Arduino.h>
#include <LibRobus.h>

//EQUIPE 2 ICI

int calculerNbTour(int angle, int rayonRoue, int rayonArc);
void tourner(int angle);

void setup() {
  // put your setup code here, to run once:
  BoardInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(ROBUS_IsBumper(0)) {
      tourner(45);
      delay(1000);

      tourner(-90);
      delay(1000);

      tourner(90);
      delay(1000);

      tourner(-45);
      delay(1000);
  }
  
}

//Calcule le nombre de tour que de doit faire la roue selon les paramètres reçu par la fonction
int calculerNbTour(int angle, int rayonRoue, int rayonArc) {
  int nbTour;

  nbTour = rayonArc * angle / (rayonRoue* 360);

  return nbTour;
}

//Angle entre -90 et 90
void tourner(int angle) {
  int roue, encodeurAvant, tourEncodeur = 0, tour;

  if(angle < 0) {
    roue = 1;
    encodeurAvant = ENCODER_Read(roue);
  } else {
    roue = 0;
    encodeurAvant = ENCODER_Read(roue);
  }
  tour = calculerNbTour(angle, 1.5, 8);

  MOTOR_SetSpeed(roue, 0.6);

  while (tourEncodeur < tour * 0.25 || tourEncodeur > tour * 1.75 ) {
    tourEncodeur = (ENCODER_Read(roue) - encodeurAvant)/3600;

    if(ROBUS_IsBumper(1)) {
      MOTOR_SetSpeed(roue, 0);
    }
  }

  MOTOR_SetSpeed(roue, 0);
}

