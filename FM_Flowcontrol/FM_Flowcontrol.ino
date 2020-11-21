#include "Frame.h"
int mode = 0;
int allframe[30] ;
void setup() {
  Serial.begin(9600);
  String test = Frame::enFrame(0,15,1);
  
  Serial.print(test);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*if(Serial.available()){
    String sIn = Serial.readStringUntil('\n');
    if(sIn.equals("INIT")){
      String data = Frame::enFrame(3,0,0);
      Serial.print(data);//will change to send by FSK later
      flushRX(); //w/ for implementation
      long timer = millis();
      while(mode == 0){
        while(!availble){
          if (millis() - current >= 3000){
            Serial.println("Timeout");
            Serial.print(data);//retransmit
            flushRX();//w/ for implementation
            timer = millis();
          }
        }
        String receiverack = RX.getString();//w/ for implementation
        if(checkack()){//check act is correctly receive w/ for implement
          mode =1;
        }
      }
      int framecounter = 0;
      for(int i = 0;i<allframe.length();i++){//reset frame array
        allframe[i] = 0;
      }
    }
  }
  while(mode==1){
    waitforserial();//waiting for implementation
    if(available){
      
      String receivedata = RX.read(); //w/ for implementation
      if(receivedata[receivedata.length()])
      String sender="",receiver="",datatype="";
      allframe[framecounter] = Frame::deframe(receivedata,datatype,sender,receiver);
      framecounter +=1;
    }
  }*/
}
