#include <Arduino.h>
#include <LibRobus.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>

#define PIN_RIGHT A0
#define PIN_LEFT A3
#define PIN_MIDDLE A2
#define PIN_DISTANCE A4

#define VERT 0
#define BLEU 1
#define JAUNE 2
#define ROUGE 3

#define PIN_NB 3
#define SPEED_FORWARD 0.3
#define SPEED_BACK -0.3

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

/*int getColorFromSensor() {
  uint16_t red,green,blue, clear = 0;

  int r = ROUGE; int g = VERT; int b = BLEU; int y = JAUNE;

  colorSensor.enable();
  colorSensor.getRawData(&red,&green,&blue,&clear);

  if(red > green && red > blue)
  {
    if(red > 125 && green < 100 && blue < 105)
    {
      return r;
    }
  }
  if(green > red && green > blue)
  {
    if(green <= 125 && green >= 110 && blue > 100)
    {
      return g;
    }
  }
  if(blue > red && blue > green)
  {
    if(blue <= 160 && blue >= 135)
    {
      return b;
    }
    
  }
  if(red > blue && green > blue)
  {
    if(red >= 250 && green > 240)
    {
      return y;
    }
  }
  return 1000;
}*/

//Cette fonction permet de faire tourner le servomoteur à un angle de 180
void ouvrirAvecServomoteur()
{
  SERVO_Enable(0);
  for (int i = 155; i > 60; i = i - 5)
  {
    SERVO_SetAngle(0, i);
    delay(45);
  }
  SERVO_Disable(0);
}

//Cette fonction permet de faire tourner le servomoteur à un angle de 0
void fermerAvecServomoteur()
{
  SERVO_Enable(0);
  for (int i = 60; i < 155; i = i + 5)
  {
    SERVO_SetAngle(0, i);
    delay(10);
  }
  SERVO_Disable(0);
}

int distanceBalle()
{
  int resultatCm;
  resultatCm = (6787.0 / (analogRead(PIN_DISTANCE) - 3.0)) - 4.0;
  return resultatCm;
}

//Calcule le nombre de pulse que doit faire la roue selon les paramètres reçus par la fonction
//rayonRoue et rayonArc sont en cm et angle est en degrès
float calculerNbPulse(int angle, float rayonRoue, float rayonArc)
{
  float nbPulse;

  nbPulse = (rayonArc * angle * 3200) / (360 * rayonRoue);

  return nbPulse;
}

//Cette fonction fait tourner le robot avec les deux moteurs avec le sens et l'angle entré par l'utilisateur
//roue = 0, le robot tourne vers la droite
//roue = 1, le robot tourne vers la gauche
//angle est en degré
void tourner2Roue(unsigned int angle, int roue)
{
  int pulseEncodeur = 0, pulse, roue2;
  ENCODER_Reset(roue);

  pulse = calculerNbPulse(angle / 2, 7.62 / 2, 19.3);

  if (roue == 0)
  {
    roue2 = 1;
  }
  else
  {
    roue2 = 0;
  }

  MOTOR_SetSpeed(roue, 0.2);
  MOTOR_SetSpeed(roue2, -0.2);

  while (pulseEncodeur < pulse - 50)
  {
    pulseEncodeur = ENCODER_Read(roue);
  }

  MOTOR_SetSpeed(roue, 0);
  MOTOR_SetSpeed(roue2, 0);
  delay(200);
}

void Move(float speed)
{
  MOTOR_SetSpeed(LEFT, speed);
  MOTOR_SetSpeed(RIGHT, speed);
}

void StopMove()
{
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
  delay(200);
}

bool getPinState(int8_t pin)
{
  return analogRead(pin) > 750;
}

void bleu() {
  tourner2Roue(90, 0); //Tourne vers la droite de 90
    Move(SPEED_FORWARD); //bouge du delay
    delay(2150);

    StopMove(); //arrete de bouger pour laisser du temps au roue

    tourner2Roue(43, 0); //tourne vers la droite de 45

    Move(SPEED_FORWARD); //bouge du delay
    delay(1800);
    StopMove(); //arrete de bouger pour laisser du temps au roue

    for(int x = 0; x <15; x++)
    {
      delay(100);
      Move(SPEED_FORWARD);
      if(distanceBalle() < 8) {
        x = 15;
      }
    } //détecte la balle
    delay(250);

    StopMove();
    for (int x = 0; x < 10; x++)
    { //ferme la pince jusqu'à ce qu'il touche le bumper avant
      fermerAvecServomoteur();
      delay(500);
      if (ROBUS_IsBumper(FRONT))
      {
        x = 11;
      }
      if (x == 9)
      {
        delay(1000);
        SERVO_Enable(0);
        SERVO_SetAngle(0, 165);
        delay(1000);
      }
    }

    tourner2Roue(45, 0); //tourne de 45 vers la droite
    delay(300);
    Move(SPEED_BACK - 0.1); //bouge du delay
    delay(2550);
    StopMove();
    delay(300);

    tourner2Roue(83, 1); //tourne de 90 vers la gauche
    delay(300);
    Move(SPEED_BACK - 0.1); //bouge du delay
    delay(3125);
    StopMove();
    delay(300);

    ouvrirAvecServomoteur(); //ouvre la pince
    delay(1000);

    Move(SPEED_BACK); //bouge du delay
    delay(1000);
    StopMove();
    delay(300);

    tourner2Roue(65, 0); //tourne à 65 vers la droite
    Move(SPEED_FORWARD * 2);
    delay(1800);
    StopMove();
}

void rouge() {
  tourner2Roue(90, 1);
    Move(SPEED_FORWARD);
    delay(2100);

    StopMove();

    tourner2Roue(93, 0);

    Move(SPEED_FORWARD);
    delay(3900);
    StopMove();

    tourner2Roue(45, 1);

    Move(SPEED_FORWARD);
    delay(1800);

    for(int x = 0; x <15; x++)
    {
      delay(100);
      Move(SPEED_FORWARD);
      if(distanceBalle() < 9) {
        x = 15;
      }
    } //détecte la balle

    delay(300);

    StopMove();

    for (int x = 0; x < 10; x++)
    {
      fermerAvecServomoteur();
      delay(500);
      if (ROBUS_IsBumper(FRONT))
      {
        x = 11;
      }
      if (x == 9)
      {
        delay(1000);
        SERVO_Enable(0);
        SERVO_SetAngle(0, 165);
        delay(1000);
      }
    }

    Move(SPEED_BACK);
    delay(200);

    tourner2Roue(45, 0);
    delay(300);
    Move(SPEED_BACK - 0.1);
    delay(2900);
    StopMove();
    delay(300);

    tourner2Roue(91, 1);
    delay(300);
    Move(SPEED_BACK - 0.1);
    delay(2900);
    StopMove();
    delay(300);

    ouvrirAvecServomoteur();
    delay(1000);

    Move(SPEED_BACK);
    delay(1400);
    StopMove();
    delay(300);

    tourner2Roue(65, 1);
    Move(SPEED_FORWARD * 2);
    delay(1800);
    StopMove();

    tourner2Roue(90, 0);
    Move(SPEED_FORWARD * 2);
    delay(700);
    StopMove();
}

void vert() {
  tourner2Roue(90, 0);
    Move(SPEED_FORWARD);
    delay(2100);

    StopMove();

    tourner2Roue(90, 1);

    Move(SPEED_FORWARD);
    delay(3750);
    StopMove();

    tourner2Roue(50, 0);

    Move(SPEED_FORWARD);
    delay(1800);

    for(int x = 0; x <15; x++)
    {
      delay(100);
      Move(SPEED_FORWARD);
      if(distanceBalle() < 9) {
        x = 15;
      }
    } //détecte la balle

    delay(300);

    StopMove();

    for (int x = 0; x < 10; x++)
    {
      fermerAvecServomoteur();
      delay(500);
      if (ROBUS_IsBumper(FRONT))
      {
        x = 11;
      }
      if (x == 9)
      {
        delay(1000);
        SERVO_Enable(0);
        SERVO_SetAngle(0, 165);
        delay(1000);
      }
    }

    Move(SPEED_BACK);
    delay(200);

    tourner2Roue(45, 1);
    delay(300);
    Move(SPEED_BACK - 0.1);
    delay(2600);
    StopMove();
    delay(300);

    tourner2Roue(97, 0);
    delay(300);
    Move(SPEED_BACK - 0.1);
    delay(3200);
    StopMove();
    delay(300);

    ouvrirAvecServomoteur();
    delay(1000);

    Move(SPEED_BACK);
    delay(1400);
    StopMove();
    delay(300);

    tourner2Roue(65, 0);
    Move(SPEED_FORWARD * 2);
    delay(1800);
    StopMove();
}

void jaune() {
  tourner2Roue(90, 1);
    Move(SPEED_FORWARD);
    delay(2125);

    StopMove();

    tourner2Roue(45, 1);

    Move(SPEED_FORWARD);
    delay(1800);
    StopMove();

    for(int x = 0; x <15; x++)
    {
      delay(100);
      Move(SPEED_FORWARD);
      if(distanceBalle() < 9) {
        x = 15;
      }
    } //détecte la balle
    delay(325);

    StopMove();
    for (int x = 0; x < 10; x++)
    {
      fermerAvecServomoteur();
      delay(500);
      if (ROBUS_IsBumper(FRONT))
      {
        x = 11;
      }
      if (x == 9)
      {
        delay(1000);
        SERVO_Enable(0);
        SERVO_SetAngle(0, 165);
        delay(1000);
      }
    }

    Move(SPEED_BACK);
    delay(200);

    tourner2Roue(40, 1);
    delay(300);
    Move(SPEED_BACK - 0.1);
    delay(2525);
    StopMove();
    delay(300);

    tourner2Roue(103, 0);
    delay(300);
    Move(SPEED_BACK - 0.1);
    delay(3275);
    StopMove();
    delay(300);

    ouvrirAvecServomoteur();

    Move(SPEED_BACK);
    delay(1500);
    StopMove();
    delay(300);

    tourner2Roue(75, 1);
    Move(SPEED_FORWARD * 2);
    delay(1600);
    StopMove();

    tourner2Roue(90, 0);
    Move(SPEED_FORWARD * 2);
    delay(900);
    StopMove();
}

void setup()
{
  BoardInit();
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_MIDDLE, INPUT_PULLUP);
  pinMode(PIN_DISTANCE, INPUT);
  ouvrirAvecServomoteur();
}

void loop()
{
  if (ROBUS_IsBumper(REAR))
  {
    bleu();
  }
  /*Serial.print("Couleur : ");
    int color;
    color = getColorFromSensor();
    switch (color)
    {
    case BLEU:
      Serial.println("Bleu");
      break;
      case ROUGE:
      Serial.println("ROUGE");
      break;
      case VERT:
      Serial.println("VERT");
      break;
      case JAUNE:
      Serial.println("JAUNE");
      break;
    
    default:
    Serial.println("Erreur");
      break;
    }*/

  /*Serial.print("Distance : ");
    Serial.println(distanceBalle());
    delay(200);*/

  /*Serial.print("Gauche : ");
    Serial.println(getPinState(PIN_LEFT));
    Serial.print("Droite : ");
    Serial.println(getPinState(PIN_RIGHT));
     Serial.print("Milieu : ");
    Serial.println(getPinState(PIN_MIDDLE));

    delay(500);*/
  /*Serial.println("Start");
    do {
      Serial.println("lol");
    } while(!getPinState(PIN_RIGHT) || !getPinState(PIN_MIDDLE) || !getPinState(PIN_LEFT));
    Serial.println("Finish");
    
    
    Serial.println("Start");
    do {
      Serial.println("lol");
    } while(!getPinState(PIN_RIGHT) && !getPinState(PIN_MIDDLE) && !getPinState(PIN_LEFT));
    Serial.println("Finish");*/
}