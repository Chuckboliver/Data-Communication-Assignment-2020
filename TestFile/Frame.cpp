#include "Frame.h"
static uint32_t Frame::byteString2Int(String arrays) {
  uint32_t num = 0;
  for (size_t i = 0 ; i < arrays.length() ; ++i) {
    //Serial.print((int)arrays[i]);
    num  = (num + (uint32_t)arrays[i] - 48) * 2;
  }
  return num / 2;
}
static uint16_t Frame::getcrc(int data){
    data = data << 3;
    int cal = data;
    int GP = 11;
    String toptip = "";
    String test = "";
    for(int i = 0 ; i < 8 ; i++){
    GP = GP << 7 - i;
        toptip = BINtoString(11-i,cal);
        test = toptip[0];
    if(test.equals("1")){
      cal ^= GP;
    }
    else{
    cal ^= 0;
    }
        GP = GP >> 7-i;
    }
    //cal is remainder
    return data ^ cal;
}
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
  strout += "1" + String(seq) + "00" + BINtoString(11, getcrc(data)) + "1"; //will implement crc later
  return strout;
}
static String Frame::make_ackFrame(int seq) {
  String strout;
  strout += "1" + String(seq) + "01" + BINtoString(11, getcrc(0)) + "1";
  return strout;
}
static String Frame::make_UFrame(int code) { // 0 reset ->1 get -45->2 get 0->3 get 45
  String strout;
  strout += "1010" + BINtoString(11, getcrc(code)) + "1"; //will implement crc later
  return strout;
}
static String Frame::decodeFrame(String strin, String& ctrl, String& seq) {
  //for U Frame use ctrl+seq to check "010" to check command
  String encodeddata,remainder;
  seq = seq+ String(strin[1]);
  ctrl = ctrl + String(strin[2])+String(strin[3]);
  for(int i=4;i<=11;i++){
    encodeddata += String(strin[i]);
  }
  for(int i = 12;i<=14;i++){
    remainder+=String(strin[i]);
  }
  String decodeddata = BINtoString(11, getcrc(byteString2Int(encodeddata)));
  if(decodeddata.equals(encodeddata+remainder)){
    return encodeddata;
  }else{
    return "Error";
  }
  return "";
}
