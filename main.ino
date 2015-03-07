//HBRIDGE pins
//Vcc1: to 5V
//Vcc2: to 7.4V (max power for motors)
//1,2EN*, 3,4 EN: to MCU
//1-4A*: to MCU
//1-4Y*: to motors
//sink 4 5 are wired to GND

//motor speed: 0 - 255. Use ANALOG. 12EN and 34EN.

//H-Br.  //MCU pin#
//LEFT WHEEL
//1,2EN  14 (wired)
//1A     10
//2A     9

//RIGHT WHEEL
//3,4EN  17
//3A     11
//4A     12

//EMITTERS: digital
//leftEmt: 22
//fLeftEmt: 20
//fRightEmt: 18
//rightEmt: 16

//RECEIVERS: analog
//leftRcv: 23
//fLeftRcv: 21
//fRightRcv: 19
//rightRcv: 17

#include "interrupt.h"
//LEFT WHEEL
const int leftMotorSpeed = 14;
const int a1 = 10;
const int a2 = 9;
//RIGHT WHEEL
const int rightMotorSpeed = 17;
const int a3 = 11;
const int a4 = 12;

//EMITTERS
const int leftEmt;
const int fleftEmt;
const int frightEmt;
const int rightEmt;
//RECEIVERS
const int leftRcv;
const int fleftRcv;
const int frightRcv;
const int rightRcv;


void setup() {
  Serial.begin(9600);
  //WHEEL LOGIC PINS
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(a3, OUTPUT);
  pinMode(a4, OUTPUT);
  pinMode(leftMotorSpeed, OUTPUT);
  pinMode(rightMotorSpeed, OUTPUT);
  //EMITTERS
  pinMode(leftEmt, OUTPUT);
  pinMode(fleftEmt, OUTPUT);
  pinMode(frightEmt, OUTPUT);
  pinMode(rightEmt, OUTPUT);
  //RECEIVERS
  pinMode(leftRcv, INPUT);
  pinMode(fleftRcv, INPUT);
  pinMode(frightRcv, INPUT);
  pinMode(rightRcv, INPUT);
}

void loop(){
  goF();
  delay(500);
}

int getIR(const int emitter, const int receiver){
  digitalWrite(emitter, LOW);
  delay(100); 
  int ambient = analogRead(receiver);
  digitalWrite(emitter, HIGH);
  delay(100);
  int apparent = analogRead(receiver);
  digitalWrite(emitter, LOW);
  return apparent - ambient;
}

void goForward(){
  digitalWrite(a1, HIGH);
  digitalWrite(a2, LOW);
  digitalWrite(a3, HIGH);
  digitalWrite(a4, LOW);
  analogWrite(leftMotorSpeed, 128);
  analogWrite(rightMotorSpeed, 128);
}

void goBackward(){
  digitalWrite(a1, LOW);
  digitalWrite(a2, HIGH);
  digitalWrite(a3, LOW);
  digitalWrite(a4, HIGH);
  analogWrite(leftMotorSpeed, 128);
  analogWrite(rightMotorSpeed, 128);
}

void turnRight(){
  //LEFT
  digitalWrite(a1, HIGH);
  digitalWrite(a2, LOW);
  //RIGHT  
  digitalWrite(a3, LOW);
  digitalWrite(a4, HIGH);
  analogWrite(leftMotorSpeed, 128);
  analogWrite(rightMotorSpeed, 128);
  delay(430);
}

void turnLeft(){
  //LEFT
  digitalWrite(a1, LOW);
  digitalWrite(a2, HIGH);
  //RIGHT  
  digitalWrite(a3, HIGH);
  digitalWrite(a4, LOW);
  analogWrite(leftMotorSpeed, 128);
  analogWrite(rightMotorSpeed, 128);
  delay(430);
}

void stop(){
  digitalWrite(a1, LOW);
  digitalWrite(a2, LOW);
  digitalWrite(a3, LOW);
  digitalWrite(a4, LOW);
  analogWrite(leftMotorSpeed,0);
  analogWrite(rightMotorSpeed,0); 
}
