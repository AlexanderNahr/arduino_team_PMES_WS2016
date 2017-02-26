//M. Hamidi: This is a 'Main'-Like Ino File. Just to understand how to integrate the class in the 'real' main ino file in our repository.
//Important: DON'T include SoftwareSerial.h it is already included by WiFiService.h


//Iza Sandor: the example from Mario was extended with the parser functions
//24.02.2017
//it is an example how to connect to the received string from Wifiservice 
//and how to get the parser return values (string and digit)


#include <Arduino.h>
#include "WiFiService.h"
#include "Parser.h"
#include "driver_timer.h"

// Create WiFiService object
WiFiService myWiFiService;
//Create Parser object
Parser myParser;
void setup()
{
  Serial.begin(9600);     // Set data rate for the HW serial port
  while (!Serial) {;}     // wait for HW serial port to connect.
  myWiFiService.Init();
}



void loop() {

  myWiFiService.Run(true);
  delay(1000);
  if (myWiFiService.String_Is_Complete())
  {
    Serial.println("");
    Serial.print("String detected: ");
    String InputString=myWiFiService.Read();
    Serial.println(InputString);
    
    int ParserReturnInt;
    ParserReturnInt = myParser.RunParser(InputString,0,0);
    Serial.print ("Parser returns the integer = ");
    Serial.println(ParserReturnInt);
    String ParserReturnString = myParser.Get_String_from_Parser();
    Serial.print ("Parser returns the string = ");
    Serial.println(ParserReturnString);
    Serial.print ("Detected false strings until now= ");
    Serial.println(counter);   
  
  }

}


