#include <arduino.h>

volatile static int numTicks=0;

void gotTick(){
   numTicks++;
   
 }
 
 int numTick(){
   return numTicks;
 }
