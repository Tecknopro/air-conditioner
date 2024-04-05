#include<mcp_can.h>
----------------------------------


MCP_CAN CAN(10);

const int pinCan = 2;
const int pinButton = 3;
const int pinTouchsensor = 4;
const int pinLed = 7;

volatile uint8_t recvFlag = 0;

void recvHandler(){
     recvFlag = 1;
}

void setup() {
  // put your setup code here, to run once:
   attachInterrupt(digitalPinToInterrupt(pinCan),recvHandler,FALLING);
  
  pinMode(pinButton,INPUT);
  pinMode(pinTouchsensor,INPUT);
  pinMode(pinLed,OUTPUT);

  Serial.begin(9600);

  if(CAN.begin(MCP_STDEXT, CAN_250KBPS,MCP_8MHZ) == CAN_OK){
    Serial.println("CAN init OK");
  }else{
    Serial.println("CAN init Error");
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long id;
  unsigned char len;
  unsigned char data[8];
  char buf[80];

  if (digitalRead(pinButton)) {
    data[0] = 1;
    if (CAN.sendMsgBuf(0x100, 1, data) == CAN_OK) {
      Serial.println("send OK");    
    } else {
      Serial.println("send Error");
    }
  }

  if (digitalRead(pinTouchsensor)) {
    data[0] = 0;
    if (CAN.sendMsgBuf(0x100, 1, data) == CAN_OK) {
      Serial.println("send OK");    
    } else {
      Serial.println("send Error");
    }
  }


  
  if(recvFlag){
    recvFlag = 0;

    CAN.readMsgBuf(&id,&len,data);
    sprintf(buf,"id:0x%3lx dlc:%d",id,(int)len);
    Serial.println(buf);

    if(id ==0x100){
      digitalWrite(pinLed,data[0]);
    }
  }

}
