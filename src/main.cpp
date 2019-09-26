#include <Arduino.h>
#include <LibRobus.h>

//EQUIPE 2 ICI

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}

//Angle entre -90 et 90
void tourner(int angle) {
  //Robot tourne de 90 degrés vers la gauche
  if(angle == -90){
    MOTOR_SetSpeed(1, 0.6);

    delay(300);

    MOTOR_SetSpeed(1,0);

  //Robot tourne de 45 degrés vers la gauche
  }else if (angle == -45) {
    MOTOR_SetSpeed(1, 0.6);

    delay(150);

    MOTOR_SetSpeed(1,0);

  //Robot tourne de 45 degrés vers la droite
  } else if(angle == 45) {
    MOTOR_SetSpeed(0, 0.6);

    delay(150);

    MOTOR_SetSpeed(0,0);

  //Robot tourne de 90 degrés vers la droite
  } else if(angle == 90) {
    MOTOR_SetSpeed(0, 0.6);

    delay(300);

    MOTOR_SetSpeed(0,0);
  } else {
    Serial.println("L'angle choisi ne fait pas partie des choix possibles");
  }
  
}