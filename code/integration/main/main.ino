/****************************************************************************************************************//**
   \file    main.ino
   \brief   main file for arduino team, this is where the main loop will be executed
   \author  Alexander Nahrwold
   \date    26.1.2017
*******************************************************************************************************************/


/******************************************************************************************************************/
// include files
#include "driver_timer.h"               //!< our timer definitions for timer measurement
#include "driver_wifi.h"                //!< set up hardware Wifly modul
#include "driver_wifi_protocol.h"       //!< our Wifi protocol + function definitions
#include "driver_serial_uart.h"         //!< reveice/send messages to serial

/******************************************************************************************************************/
// globals


/******************************************************************************************************************/
// function declarations


void setup() {
  // put your setup code here, to run once:
  setupSerial( 9600 );  // setup the serial with 9600 baud
  setupWifiModule();
}

void loop() {
  // put your main code here, to run repeatedly:

  if( receiveHwSerial == true )
  {
    // process message
  }

}


