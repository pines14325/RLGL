#include "rgb_lcd.h"

uint8_t laserPin = 4;
uint8_t redLEDPin = 1;
uint8_t greenLEDPin = 2;
uint8_t buttonPin = 3;
uint8_t buttonState = LOW;
uint8_t oldButtonState = LOW;
bool gameState = false;
bool safeState = true;
bool moveState = true;
bool gameComplete = false;
rgb_lcd lcd;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(buttonPin,INPUT);
  lcd.print("Press the Button to Begin");
  pinMode(redLEDPin,OUTPUT);
  pinMode(greenLEDPin,OUTPUT);
}

bool scanMovement()
{
  if(safeState == false){
    return safeState;
  }
  double total1 = 0;
  double avg1 = 0;
  double total2 = 0;
  double avg2 = 0;
  for(int i = 0; i < 100; i++)
  {
    pinMode(laserPin,OUTPUT);
    digitalWrite(laserPin,HIGH);
    delayMicroseconds(2);
    digitalWrite(laserPin,LOW);
    pinMode(laserPin,INPUT);
    uint64_t movementTime = pulseIn(laserPin,HIGH);
    total1 += movementTime;
  }
  avg1 = total1 / 100;
  for(int i = 0; i < 100; i++)
  {
    pinMode(laserPin,OUTPUT);
    digitalWrite(laserPin,HIGH);
    delayMicroseconds(2);
    digitalWrite(laserPin,LOW);
    pinMode(laserPin,INPUT);
    uint64_t movementTime = pulseIn(laserPin,HIGH);
    total2 += movementTime;
  }
  avg2 = total2 / 100;
  Serial.println(avg1);
  Serial.println(avg2);
  if(abs(avg2 - avg1) > 30){
    safeState = false;
    Serial.println("LOL");
  }
  else{
    safeState = true;
  }
  return safeState;
}

bool gameCompletionCheck()
{
  double total3 = 0;
  double avg3 = 0;
  for(int i = 0; i < 100; i++)
  {
    pinMode(laserPin,OUTPUT);
    digitalWrite(laserPin,HIGH);
    delayMicroseconds(2);
    digitalWrite(laserPin,LOW);
    pinMode(laserPin,INPUT);
    uint64_t movementTime = pulseIn(laserPin,HIGH);
    total3 += movementTime;
  }
  Serial.println(total3);
  avg3 = total3 / 100;
  if(avg3 < 500){
    gameComplete = true;
  }
  else{
    gameComplete = false;
  }
  return gameComplete;
}

void gameInProgress()
{
  lcd.clear();
  while(safeState == true && gameComplete == false){
    if(moveState == true){
      lcd.clear();
      digitalWrite(greenLEDPin, HIGH);
      digitalWrite(redLEDPin, LOW);
      lcd.print("GO GO GO!!!");
      int randomMoveTime = rand() % 3 + 2;
      delay(randomMoveTime * 1000);
      gameCompletionCheck();
      moveState = false;
    }
    else{
      lcd.clear();
      digitalWrite(greenLEDPin, LOW);
      digitalWrite(redLEDPin, HIGH);
      lcd.print("STOP RIGHT THERE!");
      scanMovement();
      delay(1000);
      scanMovement();
      delay(1000);
      scanMovement();
      moveState = true;
    }
  }
  if(safeState == false){
    digitalWrite(redLEDPin,LOW);
    digitalWrite(greenLEDPin,LOW);
    lcd.clear();
    lcd.print("You Died");
    delay(10000);
    gameState = false;
    safeState = true;
    moveState = true;
    gameComplete = false;
    lcd.clear();
    lcd.print("Press the Button to Begin");
    delay(1000);
    return;
  }
  if(gameComplete == true){
    digitalWrite(redLEDPin,LOW);
    digitalWrite(greenLEDPin,LOW);
    lcd.clear();
    lcd.print("You got Lucky!!!");
    delay(10000);
    gameState = false;
    safeState = true;
    moveState = true;
    gameComplete = false;
    lcd.clear();
    lcd.print("Press the Button to Begin");
    delay(1000);
    return;
  }
}

void loop() {
  if(gameState == false){
    lcd.scrollDisplayLeft();
    delay(500);
    buttonState = digitalRead(buttonPin);
    if(oldButtonState == LOW && buttonState == HIGH)
    {
      gameState = !gameState;
      lcd.clear();
    }
    oldButtonState = buttonState;
    delay(100);
  }
  else{
    lcd.clear();
    lcd.print("READY?");
    delay(3000);
    gameInProgress();
  }
}
  
  
  
  