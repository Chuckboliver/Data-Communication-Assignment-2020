#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_ADS1015.h>
#define defaultFreq 1700 // dac speed (Hz)
#define f0 400 // FSK f0
#define f1 800 // FSK f1
#define f2 1200 // FSK f2
#define f3 1600 // ASK f3
int delay0, delay1, delay2, delay3;
char inData[50];
/*S_DAC : Amplitude (12bit) of sine wave at 0,90,180,270*/
const uint16_t S_DAC[4] = {2000,4000,2000,0};
Adafruit_MCP4725 dac;
void setup() {
  dac.begin(0x62);//A2
  delay0 = (1000000 / f0 - 1000000 / defaultFreq) / 4;
  delay1 = (1000000 / f1 - 1000000 / defaultFreq) / 4;
  delay2 = (1000000 / f2 - 1000000 / defaultFreq) / 4;
  delay3 = (1000000 / f3 - 1000000 / defaultFreq) / 4;
  Serial.begin(9600);
  Serial.flush();
}

void loop( ) {
  if (Serial.available() > 0){
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
        Serial.print(cyc);
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
      Serial.println();
      delay(100);
    }
   dac.setVoltage(0, false);
   
  }
  
}
