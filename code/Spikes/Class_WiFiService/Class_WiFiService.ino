//M. Hamidi: This is a 'Main'-Like Ino File. Just to understand how to integrate the class in the 'real' main ino file in our repository.
//Important: DON'T include SoftwareSerial.h it is already included by WiFiService.h

#ifdef DEBUG
 #define DEBUG_PRINT(x)  Serial.println (x);
#else
 #define DEBUG_PRINT(x)
#endif

#ifdef DEBUG_ENTER
 #define DEBUG_ENTER_PRINT(x)  Serial.println (x);
#else
 #define DEBUG_ENTER_PRINT(x)
#endif

#include <Arduino.h>
#include "WiFiService.h"

// Create WiFiService object
WiFiService myWiFiService;

void setup()
{
  Serial.begin(9600);     // Set data rate for the HW serial port
  while (!Serial) {;}     // wait for HW serial port to connect.
}



void loop() {
  Serial.println("before");
  myWiFiService.Init(true);
  Serial.println("after");
  delay(1000);
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


