#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_ADS1015.h>
#include <Wire.h>
#include <TEA5767Radio.h>
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) 
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

TEA5767Radio radio = TEA5767Radio();
#define defaultFreq 1700 // dac speed (Hz)
#define f0 400 // FSK f0
#define f1 800 // FSK f1
#define f2 1200 // FSK f2
#define f3 1600 // ASK f3
int delay0, delay1, delay2, delay3;
char inData[50];

int r_slope = 50;
int prev = 0;
int count = 0;
int bitCount = 0;
uint16_t data = 0;
uint32_t baudTime = 0;
bool checkBaud = false;
bool checkPeak = false;
bool checkCyc = false;
int baseA ;
int aUp ;
int aDown ;
uint32_t timePerBaud = 39000;

/*S_DAC : Amplitude (12bit) of sine wave at 0,90,180,270*/
const uint16_t S_DAC[4] = {2000,4000,2000,0};
Adafruit_MCP4725 dac;
void setup() {
  sbi(ADCSRA,ADPS2) ; // this for increase analogRead Speed
  cbi(ADCSRA,ADPS1) ;
  cbi(ADCSRA,ADPS0) ;
  dac.begin(0x62);//A2
  delay0 = (1000000 / f0 - 1000000 / defaultFreq) / 4;
  delay1 = (1000000 / f1 - 1000000 / defaultFreq) / 4;
  delay2 = (1000000 / f2 - 1000000 / defaultFreq) / 4;
  delay3 = (1000000 / f3 - 1000000 / defaultFreq) / 4;
  Serial.begin(9600);
  Serial.flush();
  Wire.begin();
  radio.setFrequency(105.2);
   baseA = analogRead(A3);
   Serial.println(baseA);
   for(int i=0;i<9;i++){
    baseA+=analogRead(A3);
   }
   baseA=baseA/10;
   Serial.println(baseA);
   aUp = baseA+120;
   aDown = baseA-120;
}


void loop() {
  if (Serial.available() > 0){
    FM_TX();
  }
  
  FM_RX();

}

void FM_TX(){
  
    String inp = Serial.readString();
    int count = 0;
    for(int i = 0; i<inp.length()-1;i++){
      inData[i] = inp[i];
      count++;
    }
    for(int i = 0; i<count;i++){
      for (int k = 7; k > 0; k -= 2){
        int tmp = inData[i] & 3;
        int Delay,cyc;
        
        if(tmp == 0){
          cyc = 16;
          Delay = delay0;
        }else if(tmp == 1){
          cyc = 32;
          Delay = delay1;
        }else if(tmp == 2){
          cyc = 48;
          Delay = delay2;
        }else{
          cyc = 64;
          Delay = delay3;
        }
        //Serial.print(cyc);
        for (int sl=0;sl<cyc;sl++)
        {
          for (int s=0;s<4;s++) //4 sample/cycle
          {
            dac.setVoltage(S_DAC[s], false);//modify amplitude
            delayMicroseconds(Delay);
          }
        }
        inData[i]>>=2;
      }
      //Serial.println();
      //delay(100);
    }
   dac.setVoltage(0, false);
   
  

}

void FM_RX(){
  int tmp = analogRead(A3);
  //Serial.println(tmp);
  if(tmp<aDown and checkPeak == false){
    checkPeak = true;
    if(checkBaud == false){
      count = 0;
      checkBaud = true;
      baudTime = micros();
    }
  }
  if(tmp>aUp and checkPeak==true ){
    checkPeak = false;
    count++;
  }
  if(checkBaud == true and tmp<aDown){
    if(micros()-baudTime > timePerBaud){
      if(count>3){
        Serial.print(count);
        uint16_t Bit = (tCyc(count)-16)/16;
        data+= Bit << (bitCount * 2);
        bitCount++;
        if (bitCount == 4){
            Serial.println(char(data));
            data = 0;
            bitCount = 0;
            count = 0;
            checkBaud = false;
          }
        count = 0;
        checkBaud = false;
      }
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
