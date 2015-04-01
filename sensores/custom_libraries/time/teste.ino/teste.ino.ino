#include "Arduino.h"
#include "Time.h"

Time time;
void setup(){
  Serial.begin(9600);
  Serial.println("PT-BR");

}
void loop(){
  

  Serial.println(time.getTime(millis()));
  delay(1000);
}
