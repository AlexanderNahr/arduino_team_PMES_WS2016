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

byte g_error_count = 0;                   //!< counts erroneous messages received, resets when client disconnects
#define MAX_ERROR_COUNT_SESSION 5         //!< max number of allowed erroneous message per sessions 

String received_string,received_string_Auftrag;
bool LastClientSignedOut=true;
String StringToFab;

/****************************************************************************************************************//*
   \brief     initialization
/******************************************************************************************************************/
void setup()
{
  Serial.begin(9600);           // set data rate for the HW serial port
  while (!Serial){;}            // init HW serial port wait for HW serial port to connect.

  if ( setupTimer() == true )   // init timer module
  {
    Serial.print(F("TimerSetupSuccessful\r\n") );
  }
  else Serial.print(F("TimerSetupFailed\r\n") );

  myWiFiService.Init();         // init Wifi class
  received_string.reserve( 55 );
}


/****************************************************************************************************************//*
   \brief     main loop
/******************************************************************************************************************/
void loop()
{
  timerRuntime();                                                       // example output timer
  myWiFiService.Run(true);                                              // continous wifi check
  //delay( 1000 );

  if ( myWiFiService.String_Is_Complete() )                             // package received
  {
    if (myWiFiService.String_Is_Complete())
    {
      Serial.println("");
      Serial.println(F("String detected!"));
      
      received_string = myWiFiService.Read();                    // get string from Wifly
      
      myParser.ReceivedString = received_string;
      g_states = myParser.RunParser("", numberoforders, RemainingTime_Sek);             // interpret string
      received_string = myParser.Get_String_from_Parser();  // get string for factory

      switch ( g_states )
      {
        case ERROR_STATE:                           // error states have same result
          myWiFiService.Send( received_string );
          break;
        case LOGIN_PW_WRONG:                        // error counter ++
          myWiFiService.Send( received_string );
          break;
        case LOGOUT_SUCCESSFUL:
          LastClientSignedOut=true;
          break;  
        //WE DO NOT HAVE THIS CASE ANYMORE                      
        case ORDER_WRONG:   
          myWiFiService.Send( received_string );                        
          break;
        case ORDER_PW_WRONG:                       
          //g_error_count++;
          myWiFiService.Send( received_string );
          break;
        case LOGIN_SUCCESSFUL:                      // client detected
          myWiFiService.Send( received_string );
          LastClientSignedOut=myAuftragsverwaltung.NewClientDetected(LastClientSignedOut);
          break;
        case ORDER_SUCCESSFUL:
          if (!LastClientSignedOut)
          {
            received_string_Auftrag = myAuftragsverwaltung.NewOrderRegistered(received_string, numberoforders, RemainingTime_Sek);
            if ((received_string_Auftrag.indexOf("EXT_ORDER")==-1) && (received_string_Auftrag.indexOf("ERROR")==-1))
            {
              StringToFab = myParser.ToFactory();
              AddStringToArray(StringToFab);
               
            }
            myWiFiService.Send( received_string_Auftrag );
          }
          
          
          Serial.print(F("Auftragsverwaltung responds with: "));
          Serial.print( received_string_Auftrag );
          break;
        case BROADCAST:                             // dunno
          myWiFiService.Send( received_string );
          break;
        case CLIENT_CONNECT:                        // do nothing, pw not set yet
          break;
        case CLIENT_DISCONNECT:                     // *CLOS* detected -> client left network
          //g_error_count = 0;                        // reset error count (should be in Auftragsverwaltung)
          break;
        default:
          break;              
        }
        
      }

      //myWiFiService.Send( received_string );                       // send answer back to client      
      
      //if( g_error_count > MAX_ERROR_COUNT_SESSION)                      // check whether error count exceeds limit
     // {
        // send message back before kicking client out?
        // kick out function    
     // }
    }
}


