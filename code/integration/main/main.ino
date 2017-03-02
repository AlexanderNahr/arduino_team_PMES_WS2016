/****************************************************************************************************************//**
   \file    Main.ino
   \brief   main file for code execution
   \author  Alexander Nahrwold
   \date    Mar 1, 2017
*******************************************************************************************************************/


/******************************************************************************************************************/
// include files
#include <Arduino.h>
#include "WiFiService.h"
#include "driver_timer.h"
#include "common.h"
#include "Parser.h"

/******************************************************************************************************************/
// include files
WiFiService myWiFiService;      //!< Create WiFiService object
Parser myParser;                //!< Create Parser object
STATES g_states;                //!< states for state machine

/****************************************************************************************************************//*
   \brief     initialization
/******************************************************************************************************************/
void setup()
{
  Serial.begin(9600);           // set data rate for the HW serial port
  while (!Serial){;}            // init HW serial port wait for HW serial port to connect.

  if ( setupTimer() == true )   // init timer module
  {
    Serial.print("TimerSetupSuccessful\r\n" );
  }
  else Serial.print("TimerSetupFailed\r\n" );

  myWiFiService.Init();         // init Wifi class
}


/****************************************************************************************************************//*
   \brief     main loop
/******************************************************************************************************************/
void loop()
{
  myWiFiService.Run(true);                                              // continous wifi check
  delay( 1000 );
  if ( myWiFiService.String_Is_Complete() )                             // package received
  {
    timerRuntime();                                                     // example output timer

    if (myWiFiService.String_Is_Complete())
    {
      Serial.println("");
      Serial.print("String detected: ");
      
      String received_string = myWiFiService.Read();                    // get string from Wifly
      g_states = myParser.RunParser(received_string, 0, 0);             // interpret string
      String parser_return_string = myParser.Get_String_from_Parser();  // get string for factory
      
      switch ( g_states )                                               // evaluate next steps
      {
        case ERROR_STATE:
          break;
        case LOGIN_SUCCESSFUL:
          break;
        case LOGIN_PW_WRONG:
          break;
        case LOGOUT_SUCCESSFUL:
          break;
        case LOGOUT_PW_WRONG:
          break;
        case ORDER_SUCCESSFUL:
          break;
        case ORDER_WRONG:
          break;
        case ORDER_PW_WRONG:
          break;
        case BROADCAST:
          break;
        case CLIENT_CONNECT:
          break;
        case CLIENT_DISCONNECT:
          break;
        default:
          break;
      }
    }
  }
}


