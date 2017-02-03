/****************************************************************************************************************//**
   \file    driver_serial_uart.h
   \brief   Contains definition/declarations for the serial uart driver to receive messages through the hw uart
   \author  Alexander Nahrwold
   \date    2.1.2017
   \todo    
*******************************************************************************************************************/
#ifndef DRIVER_SERIAL_UART_H
#define DRIVER_SERIAL_UART_H

/******************************************************************************************************************/
// include files
#include "Arduino.h"
#include <SoftwareSerial.h>

/******************************************************************************************************************/
// globals

// serial related
extern bool g_rxStringCompleteFlag;    //!< flags 'true' when string is received from app
extern bool g_rxStringOverflowFlag;    //!< flags 'true' when overflow occured in string receive routine
extern const byte g_rxOverflowLimit;   //!< maximum number of char for receiving string

extern String g_rxString;              //!< contains the string received 
extern String g_txString;              //!< contains the string to send



/******************************************************************************************************************/
// function declaration

void serialEvent();
void setupSerial( unsigned int baudrate );
bool receiveHwSerial();

#endif  // DRIVER_SERIAL_UART_H
