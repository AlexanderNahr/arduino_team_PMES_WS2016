/**
 * @brief Interrupt based UART Example
 * @author F.Bartsch
 */
#include <SoftwareSerial.h>

void setup()
{
  UartInit();
}

extern SoftwareSerial SW_Serial;
extern String rxString;
extern boolean rxStringOverflowFlag;
extern boolean rxStringCompleteFlag;

void loop() {
  
  // send something to wifi module if a complete string was received
  if (rxStringCompleteFlag) 
  {
    // DO SOMETHING WITH THE RECEIVED STRING!
    // CALL PROTOKOLL HERE
    
    SW_Serial.write("STRING RECEIVED");
    
    // clear the string:
    rxString= "";
    rxStringCompleteFlag = false;
  }
  
  if (rxStringOverflowFlag)
  {
    SW_Serial.write("STRING TOO LONG!");
  }
}

