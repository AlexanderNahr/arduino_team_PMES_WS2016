
#include <Arduino.h>
#include "WiFiService.h"
#include <SoftwareSerial.h>


// Create SoftwareSerial object
SoftwareSerial mySerial(10, 11); // RX, TX

//Constructor
WiFiService::WiFiService()
{
  StringComplete = false;
  StringStarted = false;
  rxString.reserve(50);  
  rxString = "Test";
  SerialChar = "";
  StringLength = 0;
  mySerial.begin(9600);   // Set data rate for the SW serial port
  while (!mySerial) {;}   // wait for SW serial port to connect.
}

void WiFiService::Init(bool mode)
{
  //Mode "Wait_for_Start_Character": Does Bidirectional communication until it sees '['-Character, then Mode "BuildString" is entered in teh next Main Loop.
  if (mode == true && !StringComplete)        {Wait_for_Start_Character();  HWtoSWSerial();}  

  //Mode "BuildString": SWtoHW Communiction is inhibited and Characters are stored until ']' is seen and SWtoHW Communication is back active.
  //The stored String can now be provided via Get_String()
  if (mode == true && StringStarted == true)  {BuildString();               HWtoSWSerial();} 
  
  //Mode "Bidirectional". Just bidirectional Mode between SW and HW Serial.
  if (mode == false || StringComplete)        {Bidirectional_Mode();        HWtoSWSerial();}
  
}


// Checks Byte by Byte the SW Serial Buffer for the Start Character. 
// When detected, StringInProgress = true and rxString = "["

void WiFiService::Wait_for_Start_Character()
{
  while (mySerial.available() && !StringStarted) 
  {
    SerialChar = (char)mySerial.peek();
    if (SerialChar == '[')
    {
      StringStarted = true;
      rxString = "";
      StringLength = 0;
      
      //DEBUGGING CODE
      if (debug_WiFiService) {Serial.println("");Serial.println("rxString leer;");delay(1000);}
    
    }
    
    else
    {
      //Serial.print(mySerial.read()); // sends to serial monitor
      Bidirectional_Mode();

    //DEBUGGING CODE
    if (debug_WiFiService) {Serial.println("");Serial.println("Wait_for_Start_Character - el");delay(1000);}
    
    }
  }

}

// Adds Bytes of the SW Serial Buffer to the global variable  rxString 
// until End Character is detected
void WiFiService::BuildString()
{
    while (mySerial.available() && !StringComplete) 
  {
    SerialChar = (char)mySerial.read();
    rxString += SerialChar;
    StringLength++; 
    
    if (SerialChar == ']')
    {
      StringComplete = true;
      
      //DEBUGGING CODE
      if (debug_WiFiService) {Serial.println("");Serial.println("BuildString - if SerialChar == ']'");delay(1000);} 
              
    }
  
  }
}


// Resets String and flags
void WiFiService::ResetString()
{
    rxString = "";
    StringLength = 0;
    StringComplete = false;
    StringStarted = false;
    SerialChar = "";
}

// Adds SWtoHWSerial functionality to make it Bi directional. HWtoSWSerial is always active 
void WiFiService::Bidirectional_Mode()
{
    while (mySerial.available()) {Serial.write(mySerial.read());}  
    
    //DEBUGGING CODE
    if (debug_WiFiService) {Serial.println("");Serial.println("Bidirectional_Mode");delay(1000);}
    
}

// HWtoSWSerial is always active 
void WiFiService::HWtoSWSerial()
{
    while (Serial.available())   {mySerial.write(Serial.read());} 
    
    //DEBUGGING CODE
    if (debug_WiFiService) {Serial.println("");Serial.println("HWtoSWSerial");delay(1000);}
}

bool WiFiService::String_Is_Complete()
{
  return StringComplete;
}

String WiFiService::Get_String()
{
  return rxString;
}

