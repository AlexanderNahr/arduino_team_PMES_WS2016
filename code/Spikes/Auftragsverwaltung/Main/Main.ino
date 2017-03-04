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
#include "SimpleTimer.h"
#include "Auftragsverwaltung.h"

// Create WiFiService object
WiFiService myWiFiService;
//Create Parser object
Parser myParser;
//Create order management object
Auftragsverwaltung myOrder;

int ClientTotalOrders;
int MaxNoOfOrders=5; 
void setup()
{
  Serial.begin(9600);     // Set data rate for the HW serial port
  while (!Serial) {;}     // wait for HW serial port to connect.
  myWiFiService.Init();
  timerDebug(true);
  setupTimer();  
}
void loop() {
  timerRuntime();
  myWiFiService.Run(true);
  delay(1000);
  if (myWiFiService.String_Is_Complete())
  {
    Serial.println("");
    Serial.print("App->Arduino: ");
    //String InputString=myWiFiService.Get_String();
    String InputString=myWiFiService.Read();
    Serial.print(InputString);
      
    states ParserReturnState;
    ParserReturnState = myParser.RunParser(InputString,numberoforders,RemainingTime_Sek);
//    Serial.print ("Parser returns the state = ");
//    Serial.println(ParserReturnState);
    String ParserReturnString = myParser.Get_String_from_Parser();
    Serial.print (" Arduino->App: ");
    Serial.print(ParserReturnString);
    Serial.print (" #false order-strings: ");
    Serial.println(counter);   
    Serial.println();
    
    if (ParserReturnState == 9)                          //OPEN detected
    {
      myOrder.NewClientDetected(); 
    }
    
    if (ParserReturnState == 4)                         //Order string is ok
    {
        String AnswerNewOrderRegistration=myOrder.NewOrderRegistered(numberoforders,RemainingTime_Sek);
        Serial.println(AnswerNewOrderRegistration);       
   }
  }
    
    
}



