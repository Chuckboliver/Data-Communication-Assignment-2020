#ifndef FSK_TX_H
#define FSK_TX_H
#define defaultFreq 1700
#define f0 500
#define f1 800
#define f2 1100
#define f3 1400

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include<Wire.h>
#include <Adafruit_MCP4725.h>

class FSK {
  public:
    
    static Adafruit_MCP4725 dac;
    static const uint16_t S_DAC[4] = {1000, 2000, 1000, 0};
    static  const int delay0 = (1000000 / f0 - 1000000 / defaultFreq) / 4;
    static const int delay1 = (1000000 / f1 - 1000000 / defaultFreq) / 4;
    static  const int delay2 = (1000000 / f2 - 1000000 / defaultFreq) / 4;
    static  const int delay3 = (1000000 / f3 - 1000000 / defaultFreq) / 4;
    static  const int setSample = 4;
    static void TX_Flow(String);

};
#endif
