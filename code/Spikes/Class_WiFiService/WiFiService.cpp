#ifdef DEBUG
 #define DEBUG_PRINT(x)  Serial.println (x);
#else
 #define DEBUG_PRINT(x)
#endif

#define DEBUG_ENTER 1
#ifdef DEBUG_ENTER
 #define DEBUG_ENTER_PRINT(x)  Serial.println (x);
#else
 #define DEBUG_ENTER_PRINT(x)
#endif

#include <Arduino.h>
#include "WiFiService.h"
#include <SoftwareSerial.h>


// Create SoftwareSerial object
SoftwareSerial mySerial(10, 11); // RX, TX

//Constructor
WiFiService::WiFiService()
{
  StringComplete = false;
  SawStartString = false;
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
  if (mode == true && !StringComplete)        {Serial.println("Wait_for_Start_Character"); Wait_for_Start_Character();  HWtoSWSerial(); }  

  //Mode "BuildString": SWtoHW Communiction is inhibited and Characters are stored until ']' is seen and SWtoHW Communication is back active.
  //The stored String can now be provided via Get_String()
  if (mode == true && SawStartString == true)  {Serial.println("BuildString"); BuildString();               HWtoSWSerial();} 
  
  //Mode "Bidirectional". Just bidirectional Mode between SW and HW Serial.
  if (mode == false || StringComplete)        {Serial.println("Bi"); Bidirectional_Mode();        HWtoSWSerial();}
  
}


// Checks Byte by Byte the SW Serial Buffer for the Start Character. 
// When detected, StringInProgress = true and rxString = "["

void WiFiService::Wait_for_Start_Character()
{
    SawStartString = Scan_SW_Serial_Buffer_for_Start_String (true, '[');
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
    SawStartString = false;
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

//Reads Char after Char from the SW_Serial Buffer. When the Start Character is detected it stops and returns TRUE.
//If "ShowtoSerial=true", all other Characters until the start character is seen are passed to the Serial Monitor.
bool WiFiService::Scan_SW_Serial_Buffer_for_Start_String (bool ShowtoSerial, char StartChar)
{
  DEBUG_ENTER_PRINT("ENTER:Scan_SW_Serial_Buffer_for_Start_String")
  while (mySerial.available())
  {
    DEBUG_ENTER_PRINT("while")
    SerialChar = (char)mySerial.read();
    if (SerialChar == StartChar)
    {
      rxString = "";
      StringLength = 0;
      return true;
    }
    else
    {
      Serial.write(mySerial.read());
    }
  }
}

bool WiFiService::String_Is_Complete()
{
  return StringComplete;
}

String WiFiService::Get_String()
{
  return rxString;
}

