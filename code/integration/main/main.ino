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
#include "Auftragsverwaltung.h"

/******************************************************************************************************************/
// include files
WiFiService myWiFiService;                //!< Create WiFiService object
Parser myParser;                          //!< Create Parser object
Auftragsverwaltung myAuftragsverwaltung;  //!< Create Auftragsverwaltungs object
STATES g_states;                          //!< states for state machine

String AnswerOrder;
byte g_error_count = 0;                   //!< counts erroneous messages received, resets when client disconnects
#define MAX_ERROR_COUNT_SESSION 5         //!< max number of allowed erroneous message per sessions 


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
  timerRuntime();                                                       // example output timer
  myWiFiService.Run(true);                                              // continous wifi check
  delay( 1000 );
  String parser_return_string = "";
  if ( myWiFiService.String_Is_Complete() )                             // package received
  {
    if (myWiFiService.String_Is_Complete())
    {
      Serial.println("");
      Serial.println("String detected!");
      
      String received_string = myWiFiService.Read();                    // get string from Wifly
      g_states = myParser.RunParser(received_string, numberoforders, RemainingTime_Sek);             // interpret string
      parser_return_string = myParser.Get_String_from_Parser();  // get string for factory
      Serial.print("Parser responds with: ");
      Serial.println(parser_return_string);
      switch ( g_states )
      {
        case ERROR_STATE:                           // error states have same result
        case LOGIN_PW_WRONG:                        // error counter ++
        case LOGOUT_PW_WRONG:                        
        case ORDER_WRONG:                           
        case ORDER_PW_WRONG:                        
          g_error_count++;
        case LOGIN_SUCCESSFUL:                      // client detected
          myAuftragsverwaltung.NewClientDetected();
          break;
        case LOGOUT_SUCCESSFUL:                     // do nothing?
          break;
        case ORDER_SUCCESSFUL:                      // initiate order
          
          AnswerOrder= myAuftragsverwaltung.NewOrderRegistered(parser_return_string, numberoforders, RemainingTime_Sek); 
          Serial.println("Auftragsverwaltung responds with: " + AnswerOrder);
          break;
        case BROADCAST:                             // dunno
          break;
        case CLIENT_CONNECT:                        // do nothing, pw not set yet
          break;
        case CLIENT_DISCONNECT:                     // *CLOS* detected -> client left network
          g_error_count = 0;                        // reset error count (should be in Auftragsverwaltung)
          break;
        default:
          break;              
        }
        
      }

      myWiFiService.Send( parser_return_string );                       // send answer back to client      
      
      if( g_error_count > MAX_ERROR_COUNT_SESSION)                      // check whether error count exceeds limit
      {
        // send message back before kicking client out?
        // kick out function    
      }
    }
}


