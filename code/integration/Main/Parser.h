/****************************************************************************************************************//**
   \file    Parser.h
   \brief   Processes the received string from the App and returns an integer value
   \brief   and a string to be sent back to the App as answer
   \author  Izabella Sandor
   \date    Feb 23, 2017
*******************************************************************************************************************/


/******************************************************************************************************************/
// include files
#ifndef Parser_h
#define Parser_h
#include <Arduino.h>
#include "common.h"


/****************************************************************************************************************//**
 * \brief   Parser Class
 * \author  Izabella Sandor
 * \date    Feb 23, 2017
*******************************************************************************************************************/
class Parser
{
  public:
  
  //Constructor:
  Parser();

  //Public Functions and Variables that can be accessed from the Main Loop
  String Get_String_from_Parser();
  states RunParser(String ReceivedString_old,int Orders, int RemainingTime);
  String ReceivedString;
  

  private: 
  //Private Variables that can NOT be accessed from the Main Loop.They can only be accessed by the class itself.
  String Answer;
  String Password;
  states Returnvalue; 
 
  
  //Private Functions that can NOT be accessed from the Main Loop. They can only be accessed by the class itself.
  String Loginmanagement(String ReceivedString_old, int Orders, int Time);
  String Logoutmanagement(String ReceivedString_old, int Orders, int Time);
  String Ordermanagement(String ReceivedString_old, int Orders, int Time);
  String Broadcastmanagement();
  bool CheckString(String OrderString);
};

#endif
