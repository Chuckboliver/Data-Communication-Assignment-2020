#ifndef CRC6_H
#define CRC6_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

typedef uint8_t crc;
#define POLYNOMIAL 0x05
#define WIDTH  (6 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))

class CRC6 {
  public:
   CRC6();
   void begin();
   crc get_crc6(uint8_t const message[], int nBytes);
	 
  private:
   uint8_t crcTable[256];

};

#endif