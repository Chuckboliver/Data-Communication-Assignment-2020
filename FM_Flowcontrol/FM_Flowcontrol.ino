#include "Frame.h"
#define sender 1  //config A or B
#define receiver 0 //config A or B 

void setup() {
  Serial.begin(9600);
  String test;
  test = Frame.enframe(0,15,1);
  Serial.print(test);
}

void loop() {
  // put your main code here, to run repeatedly:

}
