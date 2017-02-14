#ifndef WiFiService_h
#define WiFiService_h
#include <Arduino.h>

//Debugging 
//Implemented some debugging code in several critical points of the code. If debug_WiFiService is set to true then all debugging lines in the code are active, else inactive.
static bool debug_WiFiService =  true; //

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
  bool SawStartString;
  char SerialChar;
  int StringLength;
  String rxString;
  
  //Private Functions that can NOT be accesd from the Main Loop. They can only be accessd by the class itself.
  void Wait_for_Start_Character();
  void BuildString();
  void Bidirectional_Mode();
  void HWtoSWSerial();
  bool Scan_SW_Serial_Buffer_for_Start_String (bool ShowtoSerial, char StartChar);
};

#endif
