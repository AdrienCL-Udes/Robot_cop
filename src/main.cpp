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
}

//THIS TOO
void loop()
{
  // put your main code here, to run repeatedly:
<<<<<<< HEAD
  /// loll
=======
  while (1)
  {
    if (ROBUS_IsBumper(2))
    {
      MOVE_forward(30);
      integral = 0;
      current_speed = init_speed;
    }
    else if (ROBUS_IsBumper(3))
    {
      MOVE_forward(122);

      tourner2Roue(90, 1);

      MOVE_forward(90);
      
      tourner2Roue(90, 0);

      MOVE_forward(70);

      tourner2Roue(45, 0);

      MOVE_forward(172);

      tourner2Roue(90, 1);

      MOVE_forward(45);

      tourner2Roue(45,0);

      MOVE_forward(110);
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
>>>>>>> equipe_2
}

