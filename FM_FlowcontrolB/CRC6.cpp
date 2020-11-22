#include "CRC6.h"

CRC6::CRC6(void) {
 
  
}
void CRC6::begin(void) {
  crc  remainder;
    for (int dividend = 0; dividend < 256; ++dividend)
    {
        remainder = dividend << (WIDTH - 6);

        for (uint8_t bit = 6; bit > 0; --bit)
        {  
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }

        crcTable[dividend] = remainder;
    }
}

crc CRC6::get_crc6(uint8_t const message[], int nBytes) {

    uint8_t data;

    crc remainder = 0;

    for (int byte = 0; byte < nBytes; ++byte)
    {
        data = message[byte] ^ (remainder >> (WIDTH - 6));
        remainder = crcTable[data] ^ (remainder << 6);
    }

    return (remainder);
}