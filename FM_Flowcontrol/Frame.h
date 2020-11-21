#ifndef Frame_H
#define Frame_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#define dataframe 0 //dataframe->00
#define ack 1 //ack->01
#define choose 2 //choose->10
#define all 3 //all->11
class Frame{
    public:
        static const uint8_t sender = 1; //config A or B
        static const uint8_t receiver = 0; //config A or B
        static String enFrame(int ,int ,int);
        static void deFrame(String,String*,String*,String*);
        static String bitString(int,int);
};
#endif
