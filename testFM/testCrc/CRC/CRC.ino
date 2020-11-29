#include "Frame.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(Frame::BINtoString(11, Frame::getcrc(Frame::byteString2Int("00100000"))));
}

void loop() {
  // put your main code here, to run repeatedly:

}
