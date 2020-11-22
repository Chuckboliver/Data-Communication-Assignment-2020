#include "Frame.h"
#include "CRC6.h"

static String Frame::BINtoString(int digit, int num) { //return String of num for selected digit
  String strout;
  strout = String(num, BIN);
  while (strout.length() < digit) {
    strout = "0" + strout;
  }
  return strout;
}
static String Frame::make_dataFrame(int data,int seq) {
  String strout;
  strout += "1" + String(seq) + "00" + BINtoString(11, data) + "1"; //will implement crc later
  return strout;
}
static String Frame::make_ackFrame(int seq) {
  String strout;
  strout += "1" + String(seq) + "01" + "00000000000" + "1";
  return strout;
}
static String Frame::make_UFrame(int code) { // 0 reset ->1 get -45->2 get 0->3 get 45
  String strout;
  strout += "1010" + BINtoString(11, code) + "1"; //will implement crc later
  return strout;
}
static String Frame::decodeFrame(String strin, String* ctrl, String* seq) {
  //for U Frame use ctrl+seq to check "010" to check command
  String encodeddata;
  seq = strin[1];
  ctrl = strin[2]+strin[3];
  for(int i=4;i<=14;i++){
    encodeddata += strin[i];
  }
  /*if(!decodeCRC(encodeddata)==-1){//w/ for new crc implementation
    return decodeCRC(encodeddata);
  }else{
    return "Error";
  }*/
  return "";
}
