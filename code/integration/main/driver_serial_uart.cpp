/****************************************************************************************************************//**
   \file    driver_serial_uart.h
   \brief   Contains definition/declarations for the serial uart driver to receive messages through the hw uart
   \author  Alexander Nahrwold
   \date    2.1.2017
   \todo    
*******************************************************************************************************************/

#include "driver_serial_uart.h"

/******************************************************************************************************************/
// globals
SoftwareSerial mySerial(10, 11);        //!< RX, TX

bool g_rxStringCompleteFlag   = false;    //!< flags 'true' when string is received from app
bool g_rxStringOverflowFlag   = false;    //!< flags 'true' when overflow occured in string receive routine
const byte g_rxOverflowLimit  = 200;      //!< maximum number of char for receiving string


static String g_rxString        = "";       //!< contains the string received 
static String g_txString        = "";       //!< contains the string to send


/****************************************************************************************************************//**
 * @brief receive message from hw serial port 
 * @details typically this will be the arduino serial monitor, or in the beginning to setup the wifi module
 * @date  2.1.2017
 ******************************************************************************************************************/
bool receiveHwSerial()
{
  bool return_value = false; 
  
  // send something to wifi module if a complete string was received
  if (g_rxStringCompleteFlag) 
  {
    mySerial.write("STRING RECEIVED");
    
    // clear the string:
    g_rxString = "";
    g_rxStringCompleteFlag = false;
    return_value = true;
  }
  
  if (g_rxStringOverflowFlag)
  {
    mySerial.write("STRING TOO LONG!");
    return_value = false;
  }
  return( return_value );
}
/****************************************************************************************************************//**
 * @brief rx-isr of hw uart
******************************************************************************************************************/
void serialEvent() {
  
  while (Serial.available()) 
  {
    // catch the incoming char
    char inChar = (char)Serial.read();
    
    // add it to the inputString:
    g_rxString += inChar;

    if (g_rxString.length() >= g_rxOverflowLimit)
    {
      g_rxString = "";
      g_rxStringOverflowFlag = true;
    }
  }
  
  g_rxStringCompleteFlag = true;
}

/****************************************************************************************************************//**
 * @brief setup hardware serial serial (usually wifi comm) and software serial (debug print
 * @date  2.1.2017
 ******************************************************************************************************************/
void setupSerial(unsigned int baudrate)
{
  // Set data rate for HARDWARE serial port on Arduino board Pin 0 and Pin 1
  Serial.begin( baudrate );

  while (!Serial) {
    ; // wait for serial port to connect.
  }
  
  // Set data rate for the SOFTWARE serial port (debug communication
  mySerial.begin( baudrate );

  while (!mySerial) {
    ; // wait for sw serial port to connect.
  }

  g_rxString.reserve( g_rxOverflowLimit );
}
