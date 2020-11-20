#ifndef Frame_H
#define Frame_H
#define dataframe 0 //dataframe->00
#define ack 1 //ack->01
#define choose 2 //choose->10
#define all 3 //all->11

class Frame{
    public:
        static String enFrame(int ,int ,int);
        static void deFrame(String,String*,String*,String*);
        static String bitString(int digit,int num);
};
#endif