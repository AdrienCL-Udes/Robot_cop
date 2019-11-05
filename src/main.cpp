#include <Arduino.h>
#include <LibRobus.h>
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
#define SPEED_FORWARD 0.2
#define SPEED_BACK -0.2

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS,TCS34725_GAIN_4X);

int getColorFromSensor() {
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
}


//Cette fonction permet de faire tourner le servomoteur à un angle de 180 
void ouvrirAvecServomoteur() {
  SERVO_Enable(0);
  for(int i = 150; i > 60; i = i - 5){
    SERVO_SetAngle(0, i);
    delay(10);
  }
  SERVO_Disable(0);
}

//Cette fonction permet de faire tourner le servomoteur à un angle de 0
void fermerAvecServomoteur() {
  SERVO_Enable(0);
  for(int i = 60; i < 150; i = i + 5){
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

bool getPinState(int8_t pin)
{
  return analogRead(pin) > 750;
}

void followLine(float speed)
{
  bool pinRight = getPinState(PIN_RIGHT);
  bool pinLeft = getPinState(PIN_LEFT);
  bool pinMiddle = getPinState(PIN_MIDDLE);

  if (pinMiddle && !pinRight && !pinLeft)
  {
    MOTOR_SetSpeed(LEFT, speed);
    MOTOR_SetSpeed(RIGHT, speed);
    //Serial.println("MIDDLE");
  }

  if (pinRight && !pinLeft)
  {
    if(speed < 0) {
      MOTOR_SetSpeed(LEFT, speed);
      MOTOR_SetSpeed(RIGHT, 0);
    } else {
      MOTOR_SetSpeed(RIGHT, speed);
      MOTOR_SetSpeed(LEFT, 0);
    }
    //Serial.println("RIGHT");
  }

  if (pinLeft && !pinRight)
  {
    if(speed < 0) {
      MOTOR_SetSpeed(RIGHT, speed);
      MOTOR_SetSpeed(LEFT, 0);
    } else {
      MOTOR_SetSpeed(LEFT, speed);
      MOTOR_SetSpeed(RIGHT, 0);
    }
    
    
    
    //Serial.println("LEFT");
  }
}

//COLOR
//Vert = 0
//Bleu = 1
//Jaune = 2
//Rouge = 3
void goToColor(int color)
{
  MOTOR_SetSpeed(RIGHT, SPEED_BACK);
  MOTOR_SetSpeed(LEFT, SPEED_BACK);

  do
  {
    delay(200);
  } while (!getPinState(PIN_RIGHT) && !getPinState(PIN_LEFT) && !getPinState(PIN_MIDDLE));

  switch (color)
  {
  case BLEU:
    tourner2Roue(90, 1);
    break;

  case JAUNE:
    tourner2Roue(90, 0);
    break;

  default:
    break;
  }

  /*int distance;
  MOTOR_SetSpeed(RIGHT, SPEED_FORWARD);
  MOTOR_SetSpeed(LEFT, SPEED_FORWARD);

  do
  {
    delay(200);
  } while (!getPinState(PIN_RIGHT) && !getPinState(PIN_LEFT) && !getPinState(PIN_MIDDLE));

  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);

  switch (color)
  {
  case VERT:
    tourner2Roue(45, 0);
    break;

  case BLEU:
    tourner2Roue(135, 0);
    break;

  case JAUNE:
    tourner2Roue(135, 1);
    break;

  case ROUGE:
    tourner2Roue(52, 1);
    break;

  default:
    break;
  }*/

  MOTOR_SetSpeed(RIGHT, -0.3);
  MOTOR_SetSpeed(LEFT, -0.3);

  do
  {
    followLine(-0.3);
    delay(200);
  } while (getColorFromSensor() != color);

  AX_BuzzerON(50,350);

  /*do{
    distance = distanceBalle();
  } while (distance > 10);
  
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);

  do {
    fermerAvecServomoteur();
    delay(300);
  } while(ROBUS_IsBumper(FRONT));

  MOTOR_SetSpeed(RIGHT, SPEED_BACK);
  MOTOR_SetSpeed(LEFT, SPEED_BACK);

  do
  {
    followLine(SPEED_BACK);
    delay(200);
  } while (!getPinState(PIN_RIGHT) && !getPinState(PIN_LEFT) && !getPinState(PIN_MIDDLE));

  tourner2Roue(180, 0);

  ouvrirAvecServomoteur();

  MOTOR_SetSpeed(RIGHT, SPEED_BACK);
  MOTOR_SetSpeed(LEFT, SPEED_BACK);

  delay(750);

  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);*/
}

void Move() {
  MOTOR_SetSpeed(LEFT, 0.15);
  MOTOR_SetSpeed(RIGHT, 0.15);
}
void StopMove() {
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
}

void setup()
{
  BoardInit();
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_MIDDLE, INPUT_PULLUP);
  pinMode(PIN_DISTANCE, INPUT);
  SERVO_Disable(0);

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
  if(ROBUS_IsBumper(FRONT)) {
    Serial.print("Couleur : ");
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
      }
  }
  if (ROBUS_IsBumper(REAR))
  {
    /*bool x = false;
    float speed = 0.2;
    do {
      followLine(speed);
      if (getPinState(PIN_RIGHT) && getPinState(PIN_LEFT) && getPinState(PIN_MIDDLE))
      {
        x = true;
      }
      delay(150);
      
    }while(!x);
    StopMove();*/
    goToColor(BLEU);
    /*Serial.print("Gauche : ");
    Serial.println(getPinState(PIN_LEFT));
    Serial.print("Droite : ");
    Serial.println(getPinState(PIN_RIGHT));
     Serial.print("Milieu : ");
    Serial.println(getPinState(PIN_MIDDLE));

    Serial.print("Gauche In: ");
    Serial.println(analogRead(PIN_LEFT));
    Serial.print("Droite In: ");
    Serial.println(analogRead(PIN_RIGHT));
     Serial.print("Milieu In: ");
    Serial.println(analogRead(PIN_MIDDLE));
    delay(500);*/
  }
}