#include "FSK_TX.h"
FSK::FSK(size_t dataSize)
{
  dac.begin(0x62);
  if (not Serial)
  {
    Serial.begin(115200);
  }
  inData = new char[dataSize];
}

void FSK::TX_Flow(String Frame)
{
  //Retrieve data input
  uint16_t lengths = Frame.length();
  for (size_t i = 0; i < lengths ; ++i)
  {
    inData[i] = inp[i];
  }

  //Choose cycle and delay then send
  for (size_t i = 0; i < lengths - 1; ++i)
  {
    for (size_t rounds = 7; rounds > 0; rounds -= 2)
    {
      int twoBitData = inData[i] & 3;
      int usedDelay, cyclePerBaud;
      if (twoBitData = 0)
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

      for (size_t nCycle = 0; nCycle < cyclePerBaud; ++nCycle)
      {
        for (size_t nSample = 0; nSample < setSample; ++nSample)
        {
          dac.setVoltage(S_DAC[nSample], false);
          delayMicroseconds(usedDelay);
        }
      }
    }
    inData[i] >>= 2;
  }
  dac.setVoltage(0, false);
}
