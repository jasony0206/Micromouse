//emitter
const int e = 19;
//receiver
const int r = 18;

//transistor: emitter: led, base: MCU, collector: 5v, 
//resistors bw led & emitter, and possibly bw MCU & base
//emitter: - gnd, + emitter end of transistor
//receiver: - to resistor to MCU, + to 5v

int getIR(const int emitter, const int receiver){
  int ambientLight = 0;
  int apparentLight = 0; 
  digitalWrite(emitter, LOW);
  delay(100);
  ambientLight = analogRead(receiver);
  digitalWrite(emitter, HIGH);
  delay(100);
  apparentLight = analogRead(receiver);
  digitalWrite(emitter, LOW);
  return apparentLight - ambientLight;
  //return ambientLight;
}

void setup(){
 Serial.begin(9600);
 pinMode(e, OUTPUT); 
 pinMode(r, INPUT);
}

void loop(){
  int reading = getIR(e, r);
  Serial.println(reading, DEC);
  delay(100);
}
