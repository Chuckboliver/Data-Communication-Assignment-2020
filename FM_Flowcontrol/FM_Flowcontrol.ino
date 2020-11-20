
#define sender 1  //config A or B
#define receiver 0 //config A or B 
String enFrame_data(int data,int angle){
    String strout;
    strout+= "1100"+String(sender)+String(receiver)+bitString(2,angle)+bitString(6,data)+"11";
    return strout;
}
String bitString(int digit,int num){ //return String From num for selected digit
  String strout;
  strout = String(num,BIN);
  while(strout.length()<digit){
    strout = "0"+strout;
  }
  return strout; 
}
void setup() {
  
}
void loop() {
  // put your main code here, to run repeatedly:

}
