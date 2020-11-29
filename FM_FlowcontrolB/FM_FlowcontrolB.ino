#include "Frame.h"
#include <Adafruit_ADS1015.h>
#include <TEA5767Radio.h>
TEA5767Radio radio = TEA5767Radio();
////////////////////////////////////FSK////////////////////////////////
////TX------VAR///////////
#define defaultFreq 1700
#define f0 400
#define f1 800
#define f2 1200
#define f3 1600
#include<Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;
const uint16_t S_DAC[4] = {2000, 4000, 2000, 0};
const int delay0 = (1000000 / f0 - 1000000 / defaultFreq) / 4;
const int delay1 = (1000000 / f1 - 1000000 / defaultFreq) / 4;
const int delay2 = (1000000 / f2 - 1000000 / defaultFreq) / 4;
const int delay3 = (1000000 / f3 - 1000000 / defaultFreq) / 4;
const int setSample = 4;
uint16_t delayFMConfig = 200;
////TX------VAR///////////
////RX------VAR///////////
#ifndef cbi
#define cbi(sfr, bit)(_SFR_BYTE(sfr)&=~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit)(_SFR_BYTE(sfr)|=_BV(bit))
#endif
#define r_slope 200

int prev = 0;
int count = 0;
uint16_t bitCount = 0;
uint32_t data = 0;
uint32_t baudTime = 0;
bool checkBaud = false;
bool checkPeak = false;
bool checkCyc = false;
int baseA ;
int aUp ;
int aDown ;
uint32_t timePerBaud = 39000;
////RX------VAR///////////
uint32_t byteString2Int(String arrays) {
  uint32_t num = 0;
  for (size_t i = 0 ; i < arrays.length() ; ++i) {
    //Serial.print((int)arrays[i]);
    num  = (num + (uint32_t)arrays[i] - 48) * 2;
  }
  return num / 2;
}
bool Timer(unsigned long currentTime, unsigned long interval) {
  return millis() - currentTime < interval;
}

void TX_Flow(String Frame) {
  //Retrieve data input
  //Choose cycle and delay then send
  uint32_t SEND_BIN_DATA = byteString2Int(Frame);
  
  Serial.println("SendFrame : " + Frame);
  Serial.println("-----------------------------------");
  for (int rounds = 15; rounds > 0; rounds -= 2)
  {
   
    uint32_t twoBitData = SEND_BIN_DATA & 3;
    
    uint32_t usedDelay, cyclePerBaud;
    if (twoBitData == 0)
    {
      cyclePerBaud = 16;
      usedDelay = delay0;
    }
    else if (twoBitData == 1)
    {
      cyclePerBaud = 32;
      usedDelay = delay1;
    }
    else if (twoBitData == 2)
    {
      cyclePerBaud = 48;
      usedDelay = delay2;
    }
    else
    {
      cyclePerBaud = 64;
      usedDelay = delay3;
    }
    for (size_t nCycle = 0 ; nCycle < cyclePerBaud ; ++nCycle) {
      for (size_t nSample = 0 ; nSample < setSample ; ++nSample) {
        dac.setVoltage(S_DAC[nSample], false);
        delayMicroseconds(usedDelay);
      }
    }
    SEND_BIN_DATA >>= 2;
  }
  dac.setVoltage(0, false);
}

uint16_t RX_Flow(String resendFrame, bool RESEND) {
  unsigned long currentTime = millis();
  while (bitCount < 8) {
    if (not Timer(currentTime, 3456) and RESEND) { // Timer : if time out resend last frame.
      Serial.println("Time out!!!");
      Serial.print("Re");
      TX_Flow(resendFrame);
      return 0;
    }

    uint32_t tmp = analogRead(A3);//Read analog from analog pin
  if(tmp>aDown and checkPeak == false){
      checkPeak = true;
      if(checkBaud == false){
        checkBaud = true;
        baudTime = micros();
      }
    }
  if(tmp>aUp and checkPeak==true ){
    checkPeak = false;
    count++;
  }
 if (checkBaud == true and tmp<aDown) {
      if (micros()-baudTime > timePerBaud) {
        if(count>3){
        uint32_t Bit = (tCyc(count)-16)/16;
        data+= Bit << (bitCount * 2);
        bitCount++;
        if (bitCount == 8) {

          Serial.println("RECIEVE FRAME : " + Frame::BINtoString(16, (uint16_t)data));
          Serial.println("-----------------------------------");
          Serial.flush();
          uint32_t outputData = data;
          data = 0;
          bitCount = 0;
          count = 0;
          checkBaud = false;
          return (uint16_t)outputData; /// return Frame data as INT
        }
        count = 0;
        checkBaud = false;
        }
        count = 0;
        checkBaud = false;
      }
    }
    if(micros()-baudTime>56000){
     count = 0;
     bitCount = 0;
     data = 0;
     baudTime = 0;
     checkBaud = false;
     checkPeak = false;
  }
    prev = tmp;
  }
}
int tCyc(int cyc){
  if(cyc<24){
    cyc = 16;
  }else if(cyc<40){
    cyc = 32;  
  }
  else if(cyc<56){
    cyc = 48;
  }else{
    cyc = 64;
  }
  return cyc;
}
////////////////////////////////////FSK////////////////////////////////
int myseq = 0;
int mode = -1;
int angle = -1;
uint32_t frame_arr[30] ;
String UFrame;
int framecounter = 0;
long timer;
void setup() {
  dac.begin(0x62);
  Serial.begin(9600);
  sbi(ADCSRA, ADPS2); // this for increase analogRead speed
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  Serial.flush();
  Wire.begin();
  radio.setFrequency(102.2);
   baseA = analogRead(A3);
   for(int i=0;i<9;i++){
    baseA+=analogRead(A3);
   }
   baseA=baseA/10;
   aUp = baseA+120;
   aDown = baseA-120;
}
void SEND(int maxFrame, int nextMode) {
  String dataFrame = Frame::make_dataFrame(frame_arr[framecounter], myseq);
  Serial.print("#" + (String)framecounter);
  TX_Flow(dataFrame);
  uint32_t ACK = RX_Flow(dataFrame, true);
  while (ACK == 0) {
    ACK = RX_Flow(dataFrame, true);
  }
  String seq, ctrl;
  String decodeddata = Frame::decodeFrame(Frame::BINtoString(16, ACK), ctrl, seq);

  if (ctrl.equals("01") and seq.equals((String)((myseq + 1) % 2)) and not decodeddata.equals("Error")) {
    Serial.println("Retrieve ACK " );
    myseq = (myseq + 1) % 2;
    framecounter++;
  }
  else{
    Serial.println("DropFrame!!!");
  }
  if (framecounter == maxFrame) {
    mode = nextMode;
    framecounter = 0;
    Serial.println("MODE : " + (String)nextMode);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  if ( mode == -1) {
    Serial.println("-----------------------------------");
    uint32_t ReceiveU = RX_Flow("", false);
    while (ReceiveU == 0) {
      ReceiveU = RX_Flow("", false);
    }
    String seq, ctrl;
    String decodeddata = Frame::decodeFrame(Frame::BINtoString(16, ReceiveU), ctrl, seq);
    String Uctrl = seq + ctrl;
    if (Uctrl.equals("010") and decodeddata.equals("00010000")) {
      Serial.println("UFrame Received : ");
      mode = 0;
      Serial.println("CHANGE MODE TO 0");
      Serial.print("00");
    }
  }
  if (mode == 0) {///cam send 3
    framecounter = 0;
    if (Serial.available()) {
      String data = Serial.readStringUntil('\n');
      Serial.println("DATA : " + data);

      for (int rounds = 0 ; rounds < 3 ; rounds++) {
        String ang = "";
        String pic = "";
        for (int angle = 0 ; angle < 2 ; angle++) {
          ang += data[6 * rounds + angle];
        }
        for (int i = 0 ; i < 4 ; i++) {
          pic += data[6 * rounds + i + 2];
        }
        if (ang.equals(Frame::BINtoString(2, rounds + 1))) {
        Serial.println("ANG correct : " + pic);
        frame_arr[framecounter++] = Frame::byteString2Int(pic);
        Serial.println(frame_arr[framecounter - 1]);
        }
      }
      mode = 1;
      framecounter = 0;
    }

  }
  if (mode == 1) {
    SEND(3, 2);
  }
  
  if (mode == 2) {
    Serial.println("-----------------------------------");
    uint32_t ReceiveU = RX_Flow("", false);
    while (ReceiveU == 0) {
      ReceiveU = RX_Flow("", false);
    }
    
    String seq, ctrl;
    String decodeddata = Frame::decodeFrame(Frame::BINtoString(16, ReceiveU), ctrl, seq);
    String Uctrl = seq + ctrl;
    if (Uctrl.equals("010")) {
      Serial.println("UFrame Received : ");
      uint32_t data = Frame::byteString2Int(decodeddata);
      if (not decodeddata.equals("Error")) {
        //Serial.println(data);
        if (data == 16) {
          mode = 0;
          Serial.println("00");
          Serial.flush();
        }
        else{
          mode = 3;
          Serial.println(Frame::BINtoString(4, data));
          Serial.flush();
        }
       
      }
    }
  }
  
  
  if (mode == 3) {
   
    if(Serial.available()){
      String data = Serial.readStringUntil('\n');
      uint32_t tmp;
      Serial.println("data : " + data);
      
      int index = 0;
      for(int i = 0 ; i < 21 ; i++){
        String readed = "";
        while(data[index] != '/'){
          readed += data[index];
          index++;
        }
        index++;
        frame_arr[i] = (uint32_t)readed.toInt();
      }
      mode = 4;
      framecounter = 0;
    }
  }
  if (mode == 4) {
    SEND(21, 2);
  }
}
