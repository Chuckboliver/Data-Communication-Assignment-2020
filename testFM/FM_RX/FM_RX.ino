#include <Wire.h>
#include <TEA5767Radio.h>

TEA5767Radio radio = TEA5767Radio();
/* cbi this for increase analogRead Speed */
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) 
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
/*amplitude diff. for detecting rising or falling signal*/
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
uint32_t timePerBaud = 39200;



void setup() {
  sbi(ADCSRA,ADPS2) ; // this for increase analogRead Speed
  cbi(ADCSRA,ADPS1) ;
  cbi(ADCSRA,ADPS0) ;
  Serial.begin(9600);
  Serial.flush();
  Wire.begin();
  radio.setFrequency(102.2);
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
  if(micros()-baudTime>46000){
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
