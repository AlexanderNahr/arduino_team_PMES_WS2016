//M. Hamidi: This is a 'Main'-Like Ino File. Just to understand how to integrate the class in the 'real' main ino file in our repository.
//Important: DON'T include SoftwareSerial.h it is already included by WiFiService.h

#include <Arduino.h>
#include "WiFiService.h"
#include "common.h"

// Create WiFiService object
WiFiService myWiFiService;


void setup()
{
  Serial.begin(9600);     // Set data rate for the HW serial port
  while (!Serial) {;}     // wait for HW serial port to connect.
  myWiFiService.Init();  
  //myWiFiService.Debug_ShowAll();
  received_string.reserve(143);
}



void loop() 
{

  //myWiFiService.Debug_ShowAll();  //-> for Debugging
  myWiFiService.Run(true);
  //delay(1000);
  if (myWiFiService.String_Is_Complete())
  {
    Serial.println("");
    Serial.print("String detected: ");

	myWiFiService.Send(received_string);
	myWiFiService.SendtoExternal(received_string);

	Serial.println("SHOW ALL:");
	myWiFiService.Debug_ShowAll();
  }

}


