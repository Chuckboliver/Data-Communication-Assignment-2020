#include "Frame.h"
////////////////////////////////////FSK////////////////////////////////
////TX------VAR///////////
#define defaultFreq 1700
#define f0 500
#define f1 800
#define f2 1100
#define f3 1400
#include<Wire.h>
#include <Adafruit_MCP4725.h>
#define SENDER 0
#define RECEIVER 1
Adafruit_MCP4725 dac;
const uint16_t S_DAC[4] = {1000, 2000, 1000, 0};
const int delay0 = (1000000 / f0 - 1000000 / defaultFreq) / 4;
const int delay1 = (1000000 / f1 - 1000000 / defaultFreq) / 4;
const int delay2 = (1000000 / f2 - 1000000 / defaultFreq) / 4;
const int delay3 = (1000000 / f3 - 1000000 / defaultFreq) / 4;
const int setSample = 4;
////TX------VAR///////////
////RX------VAR///////////
#ifndef cbi
#define cbi(sfr, bit)(_SFR_BYTE(sfr)&=~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit)(_SFR_BYTE(sfr)|=_BV(bit))
#endif
#define r_slope 200
int previousVoltage = 0;
int countCycle = 0;
uint16_t BAUD_COUNT = 0;
uint32_t DATA = 0;
uint16_t BIT_CHECK = -1;
bool CHECK_AMPLITUDE = false;
bool CHECK_BAUD = false;
uint32_t BAUD_BEGIN_TIME = 0;
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
  //Serial.println("test " + (String)byteString2Int("1111111111111111"));
  Serial.println("----------------------------");
  Serial.println("Send Frame : " + Frame);
  Serial.println("SEND_DATA : " + (String)SEND_BIN_DATA);
  Serial.println("----------------------------");
  for (int rounds = 15; rounds > 0; rounds -= 2)
  {
    //Serial.println("Round : " + (String)rounds);
    uint32_t twoBitData = SEND_BIN_DATA & 3;
    //Serial.println("TWOBITDATA : " + (String)twoBitData);
    uint32_t usedDelay, cyclePerBaud;
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
        //Serial.println(S_DAC[nSample]);
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
  while (BAUD_COUNT < 8) {
    if (not Timer(currentTime, 3000) and RESEND) { // Timer : if time out resend last frame.
      Serial.println("Time out!!!");
      Serial.println("Resend Frame : " + resendFrame);
      TX_Flow(resendFrame);
      return 0;
    }

    uint32_t Voltage = analogRead(A3);//Read analog from analog pin
    if (Voltage > r_slope and previousVoltage < r_slope and not CHECK_AMPLITUDE) { //Found amplitude -> Found Baud
      CHECK_AMPLITUDE = true;
      if (not CHECK_BAUD) {
        BAUD_BEGIN_TIME = micros();
        BIT_CHECK++;
      }
    }
    if (Voltage > r_slope and CHECK_AMPLITUDE) { // Count cycle
      countCycle++;
      CHECK_BAUD = true;
      CHECK_AMPLITUDE = false;
    }
    if (Voltage < r_slope and CHECK_BAUD) {
      if (micros() - BAUD_BEGIN_TIME > 9900) {
        //Serial.println("DATA : "+Frame::BINtoString(16, DATA));
        //Serial.println("nCyvle" + (String)countCycle);
        uint32_t twoBitData = (((countCycle - 5) / 3 ) & 3 ) << (BIT_CHECK * 2);
        //Serial.println("TWOBIT : "+(String)twoBitData);
        DATA |= twoBitData;
        BAUD_COUNT++;
        if (BAUD_COUNT == 8) {
          //Serial.println("DATA : " + (String)DATA);

          Serial.println("RECIEVE FRAME : " + Frame::BINtoString(16, (uint16_t)DATA));
          Serial.flush();
          uint32_t outputData = DATA;
          DATA = 0;
          BAUD_COUNT = 0;
          BIT_CHECK = -1;
          countCycle = 0;
          CHECK_BAUD = false;
          return (uint16_t)outputData; /// return Frame data as INT
        }
        CHECK_BAUD = false;
        countCycle = 0;
      }
    }
    previousVoltage = Voltage;
  }
}
////////////////////////////////////FSK////////////////////////////////
int myseq = 0;
int mode = -1;
int angle = -1;
int frame_arr[3] = {8, 9, 10};
String UFrame;
int framecounter = 0;
long timer;
void setup() {
  int state = 0;
  dac.begin(0x62);
  Serial.begin(9600);
  sbi(ADCSRA, ADPS2); // this for increase analogRead speed
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  Serial.println("Press Enter to Scan all data");
  while (state == 0) {
    if (Serial.available()) {
      String readed = Serial.readString();
      Serial.println(readed);
      if (readed.equals("send\n")) {
        mode = SENDER;
        Serial.println("I AM SENDER");
      }
      else if (readed.equals("receive\n")) {
        mode = RECEIVER;
        Serial.println("I AM RECEIVER");
      }
      state = 1;
    }
  }
  Serial.flush();
}

void loop() {
  if (mode == SENDER) {
    String dataFrame = Frame::make_dataFrame(frame_arr[framecounter], myseq);
    TX_Flow(dataFrame);
    uint32_t ACK = RX_Flow(dataFrame, true);
    while (ACK == 0) {
      ACK = RX_Flow(dataFrame, true);
    }
    String seq, ctrl;
    String decodeddata = Frame::decodeFrame(Frame::BINtoString(16, ACK), ctrl, seq);
    
    if (ctrl.equals("01") and seq.equals((String)((myseq + 1)%2))) {
      Serial.println("Retrieve ACK " );
      myseq = (myseq + 1) % 2;
      framecounter++;
    }
    if (framecounter == 3) {
      mode = RECEIVER;
      framecounter = 0;
      Serial.println("MODE : RECEIVER");
    }
  }

  if (mode == RECEIVER) {
    uint32_t dataFrame = RX_Flow("", false);
    while (dataFrame == 0) {
      dataFrame = RX_Flow("", false);
    }
    Serial.println("Received dataFrame : " + Frame::BINtoString(16, dataFrame));
    String ctrl, seq;
    String decodeddata = Frame::decodeFrame(Frame::BINtoString(16, dataFrame), ctrl, seq);
    if (ctrl.equals("00") and seq.equals((String)myseq) and not decodeddata.equals("Error")) {
      Serial.println("Decoded data : " + decodeddata);
      myseq = (myseq + 1) % 2;
      framecounter++;
    }
    String ACK = Frame::make_ackFrame(myseq);
    TX_Flow(ACK);
    if (framecounter == 3) {
      mode = SENDER;
      framecounter = 0;
      Serial.println("MODE : SENDER");
    }
  }
}
