#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

// Traffic LEDs Road A
int redA = 2;
int yellowA = 3;
int greenA = 4;

// Traffic LEDs Road B
int redB = 5;
int yellowB = 6;
int greenB = 7;

// Pedestrian Button
int pedButton = 8;

// Emergency IR Sensor
int irSensor = 9;

// Buzzer
int buzzer = 10;

// LDR
int ldrPin = A0;

// Ultrasonic A
int trigA = 11;
int echoA = 12;

// Ultrasonic B
int trigB = A1;
int echoB = A2;

long getDistance(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  long distance = duration * 0.034 / 2;

  return distance;
}

void setup()
{
  pinMode(redA, OUTPUT);
  pinMode(yellowA, OUTPUT);
  pinMode(greenA, OUTPUT);

  pinMode(redB, OUTPUT);
  pinMode(yellowB, OUTPUT);
  pinMode(greenB, OUTPUT);

  pinMode(pedButton, INPUT_PULLUP);
  pinMode(irSensor, INPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(trigA, OUTPUT);
  pinMode(echoA, INPUT);

  pinMode(trigB, OUTPUT);
  pinMode(echoB, INPUT);

  lcd.init();
  lcd.backlight();
}

void countdown(int sec)
{
  for(int i=sec;i>=0;i--)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time Left:");
    lcd.setCursor(0,1);
    lcd.print(i);
    lcd.print(" sec");
    delay(1000);
  }
}

void loop()
{
  int lightLevel = analogRead(ldrPin);

  // Night Mode
  if(lightLevel < 200)
  {
    lcd.clear();
    lcd.print("Night Mode");

    digitalWrite(redA, LOW);
    digitalWrite(greenA, LOW);
    digitalWrite(redB, LOW);
    digitalWrite(greenB, LOW);

    digitalWrite(yellowA, HIGH);
    digitalWrite(yellowB, HIGH);
    delay(500);

    digitalWrite(yellowA, LOW);
    digitalWrite(yellowB, LOW);
    delay(500);

    return;
  }

  // Emergency Vehicle Detection
  if(digitalRead(irSensor)==LOW)
  {
    lcd.clear();
    lcd.print("Emergency!");

    digitalWrite(redA, LOW);
    digitalWrite(greenA, HIGH);

    digitalWrite(redB, HIGH);
    digitalWrite(greenB, LOW);

    delay(10000);
    return;
  }

  // Pedestrian Crossing
  if(digitalRead(pedButton)==LOW)
  {
    lcd.clear();
    lcd.print("Pedestrian");

    digitalWrite(redA, HIGH);
    digitalWrite(redB, HIGH);

    digitalWrite(greenA, LOW);
    digitalWrite(greenB, LOW);

    for(int i=0;i<5;i++)
    {
      tone(buzzer,1000);
      delay(500);
      noTone(buzzer);
      delay(500);
    }
  }

  long distA = getDistance(trigA, echoA);
  long distB = getDistance(trigB, echoB);

  int greenTimeA = 10;
  int greenTimeB = 10;

  // Traffic Density Logic
  if(distA < 20)
    greenTimeA = 20;

  if(distB < 20)
    greenTimeB = 20;

  // Road A Green
  digitalWrite(greenA, HIGH);
  digitalWrite(redA, LOW);

  digitalWrite(redB, HIGH);
  digitalWrite(greenB, LOW);

  countdown(greenTimeA);

  digitalWrite(greenA, LOW);
  digitalWrite(yellowA, HIGH);
  delay(3000);
  digitalWrite(yellowA, LOW);

  // Road B Green
  digitalWrite(redA, HIGH);

  digitalWrite(redB, LOW);
  digitalWrite(greenB, HIGH);

  countdown(greenTimeB);

  digitalWrite(greenB, LOW);
  digitalWrite(yellowB, HIGH);
  delay(3000);
  digitalWrite(yellowB, LOW);
}