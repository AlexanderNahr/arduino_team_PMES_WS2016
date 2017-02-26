/****************************************************************************************************************//**
   \file    WiFiService.h
   \brief   Define control/configuration of Wifly module, read/writes from/to strings
   \author  Mario Hamidi
   \date    Feb 20, 2017
*******************************************************************************************************************/


/******************************************************************************************************************/
// include files
#ifndef WiFiService_h
#define WiFiService_h
#include <Arduino.h>

static bool debug_WiFiService =  false;  //!< debug = true -> print debug statements

/****************************************************************************************************************//**
 * \brief   Class for control/configuration of Wifly module, read/writes from/to strings
 * \author  Mario Hamidi
 * \date    Feb 20, 2017
*******************************************************************************************************************/
class WiFiService
{
  public:
  //Variables:

  //Constructor:
  WiFiService();

  //Public Functions and Variables that can be accesd from the Main Loop
  void Init(bool mode);
  void ResetString();
  bool String_Is_Complete();
  String Get_String();

  private: 
  //Private Variables that can NOT be accesd from the Main Loop.They can only be accessd by the class itself.
  bool StringComplete;
  bool StringStarted;
  char SerialChar;
  int StringLength;
  String rxString;
  
  //Private Functions that can NOT be accesd from the Main Loop. They can only be accessd by the class itself.
  void Wait_for_Start_Character();
  void BuildString();
  void Bidirectional_Mode();
  void HWtoSWSerial();
};

#endif
