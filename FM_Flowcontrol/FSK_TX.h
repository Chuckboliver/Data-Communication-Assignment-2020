#ifndef FSK_TX_H
#define FSK_TX_H
#include<Wire.h>
#include<Adafruit_MCP4725.h>
#define defaultFreq 1700
#define f0 500
#define f1 800
#define f2 1100
#define f3 1400
const uint16_t S_DAC[4] = {1000, 2000, 1000, 0}; 
Adafruit_MCP4725 dac;
class FSK {
  public:
    char *inData;
    int delay0 = (1000000 / f0 - 1000000 / defaultFreq) / 4;
    int delay1 = (1000000 / f1 - 1000000 / defaultFreq) / 4;
    int delay2 = (1000000 / f2 - 1000000 / defaultFreq) / 4;
    int delay3 = (1000000 / f3 - 1000000 / defaultFreq) / 4;
    int setSample = 4;
    FSK(size_t dataSize);
    void TX_Flow();
       
};
#endif