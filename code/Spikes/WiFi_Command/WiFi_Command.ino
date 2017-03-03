//M. Hamidi: This is a 'Main'-Like Ino File. Just to understand how to integrate the class in the 'real' main ino file in our repository.
//Important: DON'T include SoftwareSerial.h it is already included by WiFiService.h

#include <Arduino.h>
#include "WiFiService.h"


// Create WiFiService object
WiFiService myWiFiService;

void setup()
{
  Serial.begin(9600);     // Set data rate for the HW serial port
  while (!Serial) {;}     // wait for HW serial port to connect.
  myWiFiService.Init();  
  //myWiFiService.Debug_ShowAll();
  delay(2000);
}



void loop() 
{

  //myWiFiService.Debug_ShowAll();  //-> for Debugging
  myWiFiService.Run(true);
  delay(1000);
  myWiFiService.CMD_Enter();
  delay(500);
  myWiFiService.Run(true);
  delay(3000);
  myWiFiService.CMD_Exit();
  delay(500);
  myWiFiService.Run(true);
  

}


