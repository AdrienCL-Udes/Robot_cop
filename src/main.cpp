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

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS,TCS34725_GAIN_4X);

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
void ouvrirAvecServomoteur() {
  SERVO_Enable(0);
  for(int i = 155; i > 60; i = i - 5){
    SERVO_SetAngle(0, i);
    delay(45);
  }
  SERVO_Disable(0);
}

//Cette fonction permet de faire tourner le servomoteur à un angle de 0
void fermerAvecServomoteur() {
  SERVO_Enable(0);
  for(int i = 60; i < 155; i = i + 5){
    SERVO_SetAngle(0, i);
    delay(10);
  }
  SERVO_Disable(0);
}


int distanceBalle() {
  int resultatCm;
  resultatCm=(6787.0/(analogRead(PIN_DISTANCE)-3.0))-4.0;
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
}

void Move(float speed) {
  MOTOR_SetSpeed(LEFT, speed);
  MOTOR_SetSpeed(RIGHT, speed);
}

void StopMove() {
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
}

bool getPinState(int8_t pin)
{
  return analogRead(pin) > 750;
}

void goToColor(int color)
{
  switch (color)
  {
  case BLEU:
    tourner2Roue(90, 0);
    Move(SPEED_FORWARD);
    delay(2200);

    StopMove();

    tourner2Roue(45, 0);
    
    Move(SPEED_FORWARD);
    delay(1800);
    StopMove();
    
    do{
      delay(100);
      Move(SPEED_FORWARD);
    }while(distanceBalle() > 8);//détecte la balle
    delay(200);

    StopMove(); 
    for(int x = 0; x < 10; x++) {
      fermerAvecServomoteur();
      delay(500);
      if(ROBUS_IsBumper(FRONT)){
        x = 11;
      }
    }

    tourner2Roue(45,0);
    delay(300);
    Move(SPEED_BACK-0.1);
    delay(2600);
    StopMove();
    delay(300);

    tourner2Roue(90,1);
    delay(300);
    Move(SPEED_BACK-0.1);
    delay(3300);
    StopMove();
    delay(300);

    ouvrirAvecServomoteur();
    delay(1000);

    Move(SPEED_BACK);
    delay(1000);
    StopMove();
    delay(300);

    tourner2Roue(65,0);
    Move(SPEED_FORWARD*2);
    delay(1800);

    StopMove();

    break;

  case JAUNE:
    tourner2Roue(90, 1);
    Move(SPEED_FORWARD);
    delay(2200);

    StopMove();

    tourner2Roue(45, 1);
    
    Move(SPEED_FORWARD);
    delay(1800);
    StopMove();
    
    do{
      delay(100);
      Move(SPEED_FORWARD);
    }while(distanceBalle() > 8);//détecte la balle
    delay(200);

    StopMove(); 
    for(int x = 0; x < 10; x++) {
      fermerAvecServomoteur();
      delay(500);
      if(ROBUS_IsBumper(FRONT)){
        x = 11;
      }
    }

    tourner2Roue(45,1);
    delay(300);
    Move(SPEED_BACK-0.1);
    delay(2600);
    StopMove();
    delay(300);

    tourner2Roue(90,0);
    delay(300);
    Move(SPEED_BACK-0.1);
    delay(3300);
    StopMove();
    delay(300);

    ouvrirAvecServomoteur();
    
    delay(1000);

    Move(SPEED_BACK);
    delay(1000);
    StopMove();
    delay(300);

    tourner2Roue(65,1);
    Move(SPEED_FORWARD*2);
    delay(1800);

    StopMove();

    break;

  case ROUGE:
    tourner2Roue(90, 1);
    Move(SPEED_FORWARD);
    delay(2100);

    StopMove();

    tourner2Roue(100, 0);
    
    Move(SPEED_FORWARD);
    delay(4350);
    StopMove();

    tourner2Roue(48, 1);
    
    Move(SPEED_FORWARD);
    delay(1800);
    StopMove();
    
    do{
      delay(100);
      Move(SPEED_FORWARD);
    }while(distanceBalle() > 8);//détecte la balle

    delay(200);

    StopMove(); 
    
    for(int x = 0; x < 10; x++) {
      fermerAvecServomoteur();
      delay(500);
      if(ROBUS_IsBumper(FRONT)){
        x =11;
      }
    }

    tourner2Roue(45,0);
    delay(300);
    Move(SPEED_BACK-0.1);
    delay(2650);
    StopMove();
    delay(300);

    tourner2Roue(85,1);
    delay(300);
    Move(SPEED_BACK-0.1);
    delay(3100);
    StopMove();
    delay(300);

    ouvrirAvecServomoteur();
    delay(1000);

    Move(SPEED_BACK);
    delay(1000);
    StopMove();
    delay(300);

    tourner2Roue(65,1);
    Move(SPEED_FORWARD*2);
    delay(1800);
    StopMove();

    break;

  case VERT:
    tourner2Roue(90, 0);
    Move(SPEED_FORWARD);
    delay(2100);

    StopMove();

    tourner2Roue(100, 1);
    
    Move(SPEED_FORWARD);
    delay(4050);
    StopMove();

    tourner2Roue(52, 0);
    
    Move(SPEED_FORWARD);
    delay(1800);
    StopMove();
    
    do{
      delay(100);
      Move(SPEED_FORWARD);
    }while(distanceBalle() > 8);//détecte la balle

    delay(200);

    StopMove(); 
    
    for(int x = 0; x < 10; x++) {
      fermerAvecServomoteur();
      delay(500);
      if(ROBUS_IsBumper(FRONT)){
        x = 11;
      }
    }

    tourner2Roue(45,1);
    delay(300);
    Move(SPEED_BACK-0.1);
    delay(2550);
    StopMove();
    delay(300);

    tourner2Roue(85,0);
    delay(300);
    Move(SPEED_BACK-0.1);
    delay(3100);
    StopMove();
    delay(300);

    ouvrirAvecServomoteur();
    delay(1000);

    Move(SPEED_BACK);
    delay(1000);
    StopMove();
    delay(300);

    tourner2Roue(65,0);
    Move(SPEED_FORWARD*2);
    delay(1800);
    StopMove();

    break;
  default:
    break;
  }
}

void setup()
{
  BoardInit();
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_MIDDLE, INPUT_PULLUP);
  pinMode(PIN_DISTANCE, INPUT);
  ouvrirAvecServomoteur();

  Serial.begin(9600);
  Wire.begin();
  if(colorSensor.begin()){
    Serial.println("Color sensor found");
  } else {
    Serial.println("Color sensor not found");
  }
}


void loop()
{
  if(ROBUS_IsBumper(RIGHT)) {
    goToColor(ROUGE);
  }
  if(ROBUS_IsBumper(LEFT)) {
    goToColor(BLEU);
  }
  if (ROBUS_IsBumper(REAR))
  {
    goToColor(JAUNE);
  }
  if(ROBUS_IsBumper(FRONT)) {
    goToColor(VERT);
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