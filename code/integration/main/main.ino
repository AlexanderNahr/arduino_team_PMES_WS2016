/****************************************************************************************************************//**
   \file    Main.ino
   \brief   main file for code execution
   \author  Alexander Nahrwold
   \date    Feb 20, 2017
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


void setup()
{
  
  Serial.begin(9600);     // Set data rate for the HW serial port
  while (!Serial) {;}     // wait for HW serial port to connect.

  if( setupTimer() == true )
  {
    Serial.print("TimerSetupSuccessful\r\n" );
  }
  else Serial.print("TimerSetupFailed\r\n" );
  myWiFiService.Init();
}



void loop() 
{
  myWiFiService.Run(true);
  delay( 1000 );
  if( myWiFiService.String_Is_Complete() )
  {
    timerRuntime();                     // example output timer
    
    if (myWiFiService.String_Is_Complete())
    {
        Serial.println("");
        Serial.print("String detected: ");
        String InputString=myWiFiService.Read();
        
        //DEBUGGING CODE
        //if (debug_WiFiService) {Serial.println("");Serial.println("MainLoop - if String_Is_Complete");delay(1000);}
      
      g_states = myParser.RunParser(InputString,0,0);
      String ParserReturnString = myParser.Get_String_from_Parser();
      switch( g_states )
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


