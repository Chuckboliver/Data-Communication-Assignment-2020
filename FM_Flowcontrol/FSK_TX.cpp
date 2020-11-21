#include "FSK_TX.h"
static void FSK::TX_Flow(String Frame)
{
  //Retrieve data input
  //Choose cycle and delay then send

  int SEND_BIN_DATA = Frame.toInt();
  Serial.println("SEND_DATA : " + (String)SEND_BIN_DATA);
  for (int rounds = 15; rounds > 0; rounds -= 2)
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
    for(int nCycle = 0 ; nCycle < cyclePerBaud ; nCycle++){
      for(int nSample = 0 ; nSample < FSK::setSample ; nSample++){
        FSK::dac.setVoltage(FSK::S_DAC[nSample], false);
      }
    }
  }
  SEND_BIN_DATA >>= 2;
  FSK::dac.setVoltage(0, false);
}
