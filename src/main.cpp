#include <Arduino.h>
#include <LibRobus.h>

//EQUIPE 2 ICI
// Constant use for tuning PID
const float Pfactor = 0.0002;
const float Ifactor = 0.00001;

// Init varaible for motor control
int integral = 0;
const float init_speed = 0.6;
const float min_speed = 0.53;
const float max_speed = 0.67;
float current_speed = init_speed;

//Calcule le nombre de pulse que doit faire la roue selon les paramètres reçus par la fonction
//rayonRoue et rayonArc sont en cm et angle est en degrès
float calculerNbPulse(int angle, float rayonRoue, float rayonArc) {
  float nbPulse;

  nbPulse =  (rayonArc * angle* 3200)/(360*rayonRoue);
  
  return nbPulse;
}

//Cette fonction fait tourner le robot avec les deux moteurs avec le sens et l'angle entré par l'utilisateur
//roue = 0, le robot tourne vers la droite
//+roue = 1, le robot tourne vers la gauche
//angle est en degré
void tourner1Roue(unsigned int angle, int roue) {
  int pulseEncodeur = 0, pulse;
  ENCODER_Reset(roue);
  
  pulse = calculerNbPulse(angle, 7.62/2 , 19.3);

  MOTOR_SetSpeed(roue, 0.3);

  while (pulseEncodeur < pulse) {
    pulseEncodeur = ENCODER_Read(roue);
  }

  MOTOR_SetSpeed(roue, 0);
}

//Cette fonction fait tourner le robot avec les deux moteurs avec le sens et l'angle entré par l'utilisateur
//roue = 0, le robot tourne vers la droite
//+roue = 1, le robot tourne vers la gauche
//angle est en degré
void tourner2Roue(unsigned int angle, int roue) {
  int pulseEncodeur = 0, pulse, roue2;
  ENCODER_Reset(roue);
  
  pulse = calculerNbPulse(angle/2, 7.62/2 , 19.3);

  if(roue == 0){
    roue2 = 1;
  }else {
    roue2 = 0;
  }
  
  MOTOR_SetSpeed(roue, 0.2);
  MOTOR_SetSpeed(roue2, -0.2);

  while (pulseEncodeur < pulse-50) {
    pulseEncodeur = ENCODER_Read(roue);
  }

  MOTOR_SetSpeed(roue, 0);
  MOTOR_SetSpeed(roue2, 0);
}

// This fonction will adjuste the speed of motor 0
// to match motor 1
void PID()
{
  int current_position = ENCODER_Read(0);

  int error = ENCODER_Read(1) - current_position;
  integral = integral + error;
  int error_value = error;

  float adjustment = Pfactor * error_value + Ifactor * integral;

  current_speed = current_speed + adjustment;

  MOTOR_SetSpeed(0, current_speed);

  if (current_position > 10000)
  {
    integral = 0;
    MOTOR_SetSpeed(0, init_speed);
  }
  delay(100);
}

void MOVE_forward(int distance)
{
  //int runTime = getRuntime(distance);
  int nbTour_10 = 1;
  int nbCoche = 16000;
  int tmpTour = 0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  MOTOR_SetSpeed(0, init_speed);
  MOTOR_SetSpeed(1, init_speed);
  while (nbTour_10 > tmpTour || ENCODER_Read(0) <= nbCoche)
  {
    PID();
    if(ENCODER_Read(0) > 32000)
    {
      tmpTour = tmpTour + 1;
      ENCODER_Reset(0);
      ENCODER_Reset(1);
    }
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
}

//PUT THESE AT THE END
void setup()
{
  // put your setup code here, to run once:
  BoardInit();
}
//THIS TOO
void loop()
{
  // put your main code here, to run repeatedly:
  while (1)
  {
    if (ROBUS_IsBumper(3))
    {
      MOVE_forward(0);
      integral = 0;
      current_speed = init_speed;
    }
    else if(ROBUS_IsBumper(0)) {
      tourner1Roue(90, 0);
      delay(1000);
      tourner2Roue(90, 0);
      delay(1000);
      tourner1Roue(90, 0);
      delay(1000);
      tourner2Roue(90, 0);
      delay(1000);
      /*tourner(360, 0);
      delay(1000);

      tourner(45, 1);
      delay(1000);

      tourner(90, 1);
      delay(1000);*/
    }
  }
}