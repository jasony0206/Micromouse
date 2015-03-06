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
//capacitor: + between 5V, MCU Vin. - goes to GND

//3.9K * 2: range 70 - 650
//18K/2: range 70 - 750

//RESISTORS
//GND & receiver - lead: 18K's in parallel (9K)
//transistor base & MCU emitter pin: 3.9K
//transistor emitter lead & emitter - lead: 51ohms

//CAPACITOR
//0.1uF & 4.7uF in parallel (4.8uF):  - to GND, + to 5V and MCU Vin

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
