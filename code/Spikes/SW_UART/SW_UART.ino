/**
 * @brief Interrupt based UART Example
 * @author F.Bartsch
 */

// Include SoftwareSerial library
#include <SoftwareSerial.h>

// Create SoftwareSerial object
SoftwareSerial mySerial(10, 11); // RX, TX

// set up glabal data 
String rxString = "";
boolean rxStringCompleteFlag = false;

void setup()
{
  // Set data rate for HW Serial port on Arduino board Pin 0 and Pin 1
  Serial.begin(9600);

  while (!Serial)
  {
    ; // wait for serial port to connect.
  }

  rxString.reserve(200);

  // Set data rate for the SW SoftwareSerial port
  mySerial.begin(9600);
}

void loop() {
  // print the string when a newline arrives:
  if (rxStringCompleteFlag) 
  {
    mySerial.write("STRING RECEIVED");
    
    // clear the string:
    rxString = "";
    rxStringCompleteFlag = false;
  }
}

/**
 * @brief ISR of UART RX
 */
void serialEvent() {
  
  while (Serial.available()) 
  {
    // catch the incoming char
    char inChar = (char)Serial.read();
    
    // add it to the inputString:
    rxString += inChar;
    
    // set flag if incoming char is the termination sign "]".
    // this signals to the main loop to do somthing with the data
    if (inChar == ']')
    {
      rxStringCompleteFlag = true;
    }
  }
}

