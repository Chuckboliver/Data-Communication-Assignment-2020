#include "Frame.h"

void setup() {
  Serial.begin(9600);
  String test = Frame::enFrame(0,15,1);
  //OUTPUT ->         1100100100111111
  Serial.print(test);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    String sIn = Serial.readStringUntil('\n');
    if(sIn.equals("INIT"){
      String data = Frame::enframe(3,0,0);
      Serial.print(data)
    }
  }
}
