#include <Servo.h> //libreria Servo
Servo Servo1; //Il nome del servo è Servo1

int trigPin = 12;    // Trig Pin del sensore a ultrasuoni
int echoPin = 13;    // Echo Pin del sensore a ultrasuoni 
long duration, cm;

#define ML_Ctrl 4  //Define the direction control pin of the left motor
#define ML_PWM 6   //Define the PWM control pin of the left motor
#define MR_Ctrl 2  //Define the direction control pin of the right motor
#define MR_PWM 5   //Define the PWM control pin of the right motor

void setup() {
  Serial.begin(9600);
  Servo1.attach(10); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Servo1.write(90); // Primo movimento del servo al centro
}

void loop() {
  cm = Read_Ultrasonic_Value();
  if (cm < 25) {
    Car_Stop();
    int right, left;
    Check_Surroundings(right, left);
    if (right > left) {
      Car_back();
      delay(500);
      Car_right();
      delay(500);
    } else {
      Car_back();
      delay(500);
      Car_left();
      delay(500);
    }
  } else {
    Car_front();
  }
  Serial.println(cm);
  delay(100);
}

// Funzioni di movimento
void Car_front() {
  digitalWrite(MR_Ctrl, HIGH);
  analogWrite(MR_PWM, 110); //fix per il fatto che gira
  digitalWrite(ML_Ctrl, HIGH);
  analogWrite(ML_PWM, 100);
}
void Car_back() {
  digitalWrite(MR_Ctrl, LOW);
  analogWrite(MR_PWM, 160); //fix per il fatto che gira
  digitalWrite(ML_Ctrl, LOW);
  analogWrite(ML_PWM, 150);
}
void Car_left() {
  digitalWrite(MR_Ctrl, HIGH);
  analogWrite(MR_PWM, 100);
  digitalWrite(ML_Ctrl, LOW);
  analogWrite(ML_PWM, 100);
}
void Car_right() {
  digitalWrite(MR_Ctrl, LOW);
  analogWrite(MR_PWM, 150);
  digitalWrite(ML_Ctrl, HIGH);
  analogWrite(ML_PWM, 100);
}
void Car_Stop() {
  digitalWrite(MR_Ctrl, LOW);
  analogWrite(MR_PWM, 0);
  digitalWrite(ML_Ctrl, LOW);
  analogWrite(ML_PWM, 0);
}

// Leggi la distanza con il sensore a ultrasuoni
float Read_Ultrasonic_Value() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // Almeno 10 µs di impulso alto
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); // Leggi il tempo di eco
  return (duration / 2) / 29.1;      // Converti in centimetri
}

// Controlla i dintorni
void Check_Surroundings(int &right_distance, int &left_distance) {
  Servo1.write(35); // Gira il servo a destra
  delay(500);       // Aspetta che il servo si muova
  right_distance = Read_Ultrasonic_Value();

  Servo1.write(145); // Gira il servo a sinistra
  delay(500);        // Aspetta che il servo si muova
  left_distance = Read_Ultrasonic_Value();

  Servo1.write(90); // Riporta il servo al centro
  delay(500);
}
