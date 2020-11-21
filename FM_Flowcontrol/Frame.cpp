#include "Frame.h"
static String Frame::enFrame(int datatype,int data,int angle){
    String strout;
    strout+= "11"+bitString(2,datatype)+(String)Frame::sender+(String)Frame::receiver+bitString(2,angle)+bitString(6,data)+"11";
    return strout; 
}
static String Frame::bitString(int digit,int num){ //return String From num for selected digit
    String strout;
    strout = String(num,BIN);
    while(strout.length()<digit){
        strout = "0"+strout;
    }
    return strout; 
}
void Frame::deFrame(String strin,String* datatype,String* sender,String* receiver){
    /*if(strin[0]==1&&){

    }*/
}
