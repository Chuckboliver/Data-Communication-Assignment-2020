#include "Frame.h"

////////////////////////////////////FSK////////////////////////////////
#define defaultFreq 1700
#define f0 500
#define f1 800
#define f2 1100
#define f3 1400
#include<Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;
const uint16_t S_DAC[4] = {1000, 2000, 1000, 0};
const int delay0 = (1000000 / f0 - 1000000 / defaultFreq) / 4;
const int delay1 = (1000000 / f1 - 1000000 / defaultFreq) / 4;
const int delay2 = (1000000 / f2 - 1000000 / defaultFreq) / 4;
const int delay3 = (1000000 / f3 - 1000000 / defaultFreq) / 4;
const int setSample = 4;
int byteString2Int(String arrays) {
  int num = 0;
  for (size_t i = 0 ; i < arrays.length() ; ++i) {
    //Serial.print((int)arrays[i]);
    num  = (num + (int)arrays[i] - 48) * 2;
  }
  return num / 2;
}
void TX_Flow(String Frame) {
  //Retrieve data input
  //Choose cycle and delay then send
  int SEND_BIN_DATA = byteString2Int(Frame);
  Serial.println("Frame : " + Frame);
  Serial.println("SEND_DATA : " + (String)SEND_BIN_DATA);
  for (size_t rounds = 15; rounds > 0; rounds -= 2)
  {
    int twoBitData = SEND_BIN_DATA & 3;
    Serial.print("TWOBITDATA : " + String(twoBitData));
    int usedDelay, cyclePerBaud;
    if (twoBitData == 0)
    {
      cyclePerBaud = 5;
      usedDelay = delay0;
    }
    else if (twoBitData == 1)
    {
      cyclePerBaud = 8;
      usedDelay = delay1;
    }
    else if (twoBitData == 2)
    {
      cyclePerBaud = 11;
      usedDelay = delay2;
    }
    else
    {
      cyclePerBaud = 14;
      usedDelay = delay3;
    }
    for (size_t nCycle = 0 ; nCycle < cyclePerBaud ; ++nCycle) {
      for (size_t nSample = 0 ; nSample < setSample ; ++nSample) {
        dac.setVoltage(S_DAC[nSample], false);
      }
    }
  }
  SEND_BIN_DATA >>= 2;
  dac.setVoltage(0, false);
}
////////////////////////////////////FSK////////////////////////////////

int mode = 0;
int allframe[30] ;
void setup() {
  Serial.begin(9600);
  String test = Frame::enFrame(0, 15, 1);
}

void loop() {
  // put your main code here, to run repeatedly:

  while (mode == 0) { //insert command INIT to start scan
    if (Serial.available()) {
      String sIn = Serial.readStringUntil('\n');
      if (sIn.equals("INIT")) {
        String data = Frame::enFrame(3, 0, 0);
        Serial.print(data);//will change to send by FSK later
        flushRX(); //w/ for implementation
        long timer = millis();
        while (mode == 0) {
          while (!availble) {
            if (millis() - current >= 3000) {
              Serial.println("Timeout");
              Serial.print(data);//retransmit
              flushRX();//w/ for implementation
              timer = millis();
            }
          }
          String receiverack = RX.getString();//w/ for implementation
          if (checkack()) { //check act is correctly receive w/ for implement
            mode = 1;
          }
        }
        int framecounter = 0;
        for (int i = 0; i < allframe.length(); i++) { //reset frame array
          allframe[i] = 0;
        }
      }
    }
  }
  while (mode == 1) { //receiving data from sender
    waitforserial();//waiting for implementation
    if (available) {

      String receivedata = RX.read(); //w/ for implementation
      if (receivedata[receivedata.length() - 1].equals("1")) {
        mode = 2;
      }
      String sender = "", receiver = "", datatype = "";
      allframe[framecounter] = Frame::deframe(receivedata, datatype, sender, receiver);
      framecounter += 1;
      displayalldata(); // w/ for implementation to display all code of color
    }
  }
  while (mode == 2) { //choose next command
    if(Serial.available()){
      String sIn = Serial.readStringUntil('\n');
      
    }
  }
}
