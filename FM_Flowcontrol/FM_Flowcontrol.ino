#include "Frame.h"

void setup() {
  Serial.begin(9600);
  String test;
  test = Frame.enframe(0,15,1);
  Serial.print(test);
}

void loop() {
  // put your main code here, to run repeatedly:

}
