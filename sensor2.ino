//emitter
const int e = 19;
//receiver
const int r = 18;

//transistor: emitter: led, base: MCU, collector: 5v, 
//resistors bw led & emitter, and possibly bw MCU & base
//emitter: - gnd, + emitter end of transistor
//receiver: - to MCU, + to 12K resistor to 5v
//low ohms -> low values
//12K: 650, 3.9K*2: 345

int getIR(const int emitter, const int receiver){
  int ambient = 0;
  int apparent = 0; 
  digitalWrite(emitter, LOW);
  delay(100);
  ambient = analogRead(receiver);
  digitalWrite(emitter, HIGH);
  delay(100);
  apparent = analogRead(receiver);
  digitalWrite(emitter, LOW);
  return apparent - ambient;
  //return apparentLight;
}

void setup(){
 Serial.begin(9600);
 pinMode(e, OUTPUT); 
 pinMode(r, INPUT);
}

void loop(){
  //digitalWrite(e, HIGH);
  int reading = getIR(e, r);
  Serial.println(reading, DEC);
  delay(100);
}
