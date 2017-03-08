/****************************************************************************************************************//**
   \file    Auftragsverwaltung.h
   \brief   Controls the orders received from the App
   \author  Izabella Sandor
   \date    Feb 28, 2017
*******************************************************************************************************************/


/******************************************************************************************************************/
// include files
#ifndef Auftragsverwaltung_h
#define Auftragsverwaltung_h
#include <Arduino.h>


/****************************************************************************************************************//**
 * \brief   Parser Class
 * \author  Izabella Sandor
 * \date    Feb 23, 2017
*******************************************************************************************************************/
class Auftragsverwaltung
{
  public:
  //Constructor:
  Auftragsverwaltung();

  //Public Functions and Variables that can be accessed from the Main Loop
  
  bool NewClientDetected(bool& LastClientSignedOut);
  String NewOrderRegistered(String& StringFromParser,byte& Orders, int& Time);
  

  private: 
  //Private Variables that can NOT be accessed from the Main Loop.They can only be accessed by the class itself.
  int MaxNoOfOrders;
  int ClientTotalOrders;
  bool NewClient;
  
   
  
};

#endif
