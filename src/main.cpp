#include <Arduino.h>
#include <LibRobus.h>

//EQUIPE 2 ICI
// Constant use for tuning PID
const float Pfactor = 0.0002;
const float Ifactor = 0.00001;

// Init varaible for motor control
int integral = 0;
const float init_speed = 0.4;
float current_speed = init_speed;
const float Wheel_size_10 = 76.2;
const int numberTickWheel = 32000;

//Cette fonction permet de faire tourner le servomoteur à un angle de 180 
void ouvrirAvecServomoteur() {
  SERVO_Enable(0);
  delay(100);
  SERVO_SetAngle(0, 105);
  delay(500);
  SERVO_Disable(0);
}

//Cette fonction permet de faire tourner le servomoteur à un angle de 0
void fermerAvecServomoteur() {
  SERVO_Enable(0);
  delay(100);
  SERVO_SetAngle(0, 160);
  delay(500);
  SERVO_Disable(0);
}

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

int CALCUL_nbCompleteWheelRotation_10(float cm_distance)
{
  return cm_distance / Wheel_size_10;
}

int CALCUL_nbPartialWheelRotation(float cm_distance, int nbCompleteRotation)
{
  float leftover = cm_distance - nbCompleteRotation * Wheel_size_10;
  return leftover * numberTickWheel / Wheel_size_10;
}

void MOVE_forward(int distance)
{
  int nbTour_10 = CALCUL_nbCompleteWheelRotation_10(distance);
  int nbTick = CALCUL_nbPartialWheelRotation(distance, nbTour_10);

  Serial.print("Nb Tour: ");
  Serial.println(nbTour_10);
  Serial.print("Nb Tick: ");
  Serial.println(nbTick);
  
  int tmpTour = 0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  MOTOR_SetSpeed(0, init_speed);
  MOTOR_SetSpeed(1, init_speed);
  while (nbTour_10 > tmpTour || ENCODER_Read(0) <= nbTick)
  {
    PID();
    if (ENCODER_Read(0) > 32000)
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
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_MIDDLE, INPUT_PULLUP);
}

//THIS TOO
void loop()
{
  // put your main code here, to run repeatedly:
  SERVO_Disable(0);
  if(ROBUS_IsBumper(0)) {
    fermerAvecServomoteur();
    ENCODER_Reset(0);
    ENCODER_Reset(1);
    MOTOR_SetSpeed(1, -0.3);
    MOTOR_SetSpeed(0, -0.3);

    delay(1000);


    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
    ouvrirAvecServomoteur();
  } 
}