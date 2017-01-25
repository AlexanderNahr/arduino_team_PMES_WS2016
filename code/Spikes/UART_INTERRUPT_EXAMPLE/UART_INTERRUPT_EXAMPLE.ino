/**
 * @brief Interrupt based UART Example
 * @author F.Bartsch
 */

#include <SoftwareSerial.h>

// Create SoftwareSerial object
SoftwareSerial mySerial(10, 11); // RX, TX

// set up glabal data 
static String rxString = "";

/**
 * @brief Interface to get rxString in other Modules
 * @details Use Funktion to  work with the Data in other modules. 
 */
String GetRxString(void){
  return (rxString);
}

// this flag indicates a received sting is ready to work with.
// note that that is the only global data structure in this module
boolean rxStringCompleteFlag = false;
boolean rxStringOverflowFlag = false;

void setup()
{
  // Set data rate for hw serial port on Arduino board Pin 0 and Pin 1
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect.
  }
  
  // Set data rate for the sw serial port
  mySerial.begin(9600);

  while (!mySerial) {
    ; // wait for sw serial port to connect.
  }

  rxString.reserve(200);
}

void loop() {
  
  // send something to wifi module if a complete string was received
  if (rxStringCompleteFlag) 
  {
    mySerial.write("STRING RECEIVED");
    
    // clear the string:
    rxString = "";
    rxStringCompleteFlag = false;
  }
  
  if (rxStringOverflowFlag)
  {
    mySerial.write("STRING TOO LONG!");
  }
}

/**
 * @brief rx-isr of hw uart
 */
void serialEvent() {
  
  while (Serial.available()) 
  {
    // catch the incoming char
    char inChar = (char)Serial.read();
    
    // add it to the inputString:
    rxString += inChar;

    if (rxString.length() >= 200)
    {
      rxString = ""
      rxStringOverflowFlag = true;
    }
    
    // set flag if incoming char is the termination sign "]".
    // this signals to the main loop to do somthing with the data
    if (inChar == ']')
    {
      rxStringCompleteFlag = true;
    }
  }
}

