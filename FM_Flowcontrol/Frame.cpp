#include "Frame.h"
String Frame::enFrame(int datatype,int data,int angle){
    
    String strout;
    strout+= "11"+bitString(2,datatype)+(String)sender+(String)receiver+bitString(2,angle)+bitString(6,data)+"11";
    return strout; 
}
String Frame::bitString(int digit,int num){ //return String From num for selected digit
    String strout;
    strout = String(num,BIN);
    while(strout.length()<digit){
        strout = "0"+strout;
    }
    return strout; 
}
void Frame::deFrame(String strin,String* datatype,String* sender,String* receiver){
    if(strin[0]==1&&)
}
