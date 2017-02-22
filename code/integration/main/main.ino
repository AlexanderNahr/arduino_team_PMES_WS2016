/****************************************************************************************************************//**
   \file    Main.in0
   \brief   main file for code execution
   \author  Alexander Nahrwold
   \date    Feb 20, 2017
*******************************************************************************************************************/


/******************************************************************************************************************/
// include files
//M. Hamidi: This is a 'Main'-Like Ino File. Just to understand how to integrate the class in the 'real' main ino file in our repository.
//Important: DON'T include SoftwareSerial.h it is already included by WiFiService.h

#include <Arduino.h>
#include "WiFiService.h"
#include "driver_timer.h"

// Create WiFiService object
WiFiService myWiFiService;

void setup()
{
  Serial.begin(9600);     // Set data rate for the HW serial port
  while (!Serial) {;}     // wait for HW serial port to connect.

  if( setupTimer() == true )
  {
    Serial.print( "TimerSetupFinished\r\n" ); 
  }
  else Serial.print( "TimerSetupFAILED\r\n" ); 
}



void loop() {

  myWiFiService.Init(false);

  timerRuntime();
  
  if (myWiFiService.String_Is_Complete())
  {
    Serial.println("");
    Serial.print("String detected: ");
    Serial.println(myWiFiService.Get_String());
    myWiFiService.ResetString();
    
    //DEBUGGING CODE
    if (debug_WiFiService) {Serial.println("");Serial.println("MainLoop - if String_Is_Complete");delay(1000);}
  }

}


