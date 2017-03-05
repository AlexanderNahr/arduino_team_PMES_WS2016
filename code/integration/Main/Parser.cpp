/****************************************************************************************************************//**
   \file    Parser.cpp
   \brief   Processes the received string from the App and returns an integer value
   \brief   and a string to be sent back to the App as answer
   \author  Izabella Sandor
   \date    Feb 23, 2017
*******************************************************************************************************************/


/******************************************************************************************************************/
// include files

#include <Arduino.h>
#include "Parser.h"
#include "driver_timer.h"   
#include "common.h"
//Constructor
Parser::Parser()
{
  Password = "a";
  ReceivedString.reserve(50);

}


/****************************************************************************/
//Function Loginmanagement:
//->checks if the password is correct and returns the string to be sent to the App
//->updates the Returnvalue

String Parser::Loginmanagement(String ReceivedString_old, int Orders, int Time)
{
  //Serial.println("START LOGIN MANAGEMENT"); // alex_debug
  Serial.println(ReceivedString); // alex_debug
  String StringToBeReturned;
  //Serial.println("Loginmanagement string: " + ReceivedString); // alex_debug
  String CommonAnswer = "SIGN_IN_RS%" + String(Orders) + "%" + String(Time) + "%";
  int pos = ReceivedString.indexOf("%");
  String ReceivedPassword = ReceivedString.substring(pos+1);//, ReceivedString.length()-1);
 // Serial.println("Loginmanagement pw: " + ReceivedPassword); // alex_debug
  
  if (ReceivedPassword==Password)
  {
    StringToBeReturned = CommonAnswer + "SUCCESSFUL";
    Returnvalue = LOGIN_SUCCESSFUL;
  }
  else
  {
    Serial.print("pwWrong");  
    StringToBeReturned = CommonAnswer + "PW_WRONG";
    Returnvalue = LOGIN_PW_WRONG;
  }
  return StringToBeReturned;
}

/**********************************************************************************/
//Function Logoutmanagement:
//->checks if the password is correct and returns the string to be sent to the App
//->updates the Returnvalue

String Parser::Logoutmanagement(String ReceivedString_old, int Orders, int Time)
{
  String StringToBeReturned= "SIGN_OUT";
  int pos = ReceivedString.indexOf("%");
  String ReceivedPassword = ReceivedString.substring(pos+1);
  if (ReceivedPassword==Password)
  {
    Returnvalue = LOGOUT_SUCCESSFUL;
  }
  else
  {
    Returnvalue = LOGOUT_PW_WRONG;
  }
  return StringToBeReturned;
}

/***********************************************************************************/
//Function Ordermanagement:
//->checks if password and order-string are ok
//->returns the string to be sent to the App
//->updates the Returnvalue

String Parser::Ordermanagement(String ReceivedString_old, int Orders, int Time)
{
  //Serial.println("START ORDERMANAGEMENT");  // alex_debug
  Serial.println("str:" + ReceivedString); // alex_debug
  String StringToBeReturned;
  String CommonAnswer_Part1 = "ORDER_RS%";
  String CommonAnswer_Part2 =  String(Orders) + "%" + String(Time);
  int pos1 = ReceivedString.indexOf("%");
  Serial.println("pos1:" + pos1); // alex_debug
  String SecondPart = ReceivedString.substring(pos1+1);
  Serial.println("2nd: " + ReceivedString); // alex_debug
  int pos2 = SecondPart.indexOf("%");
  Serial.println("pos2:" + pos2); // alex_debug
  String ReceivedPassword = SecondPart.substring(0,pos2);
  Serial.println("rpw:" + ReceivedPassword); // alex_debug
  String ThirdPart = SecondPart.substring(pos2+1);
  Serial.println("3rd:" + ThirdPart); // alex_debug
  if (ReceivedPassword==Password)
  {
    bool StringOK = CheckString(ThirdPart);
    if (StringOK)
    {
      StringToBeReturned = CommonAnswer_Part1 + "SUCCESSFUL%" + CommonAnswer_Part2;
      Returnvalue=ORDER_SUCCESSFUL;
    }
    else
    {
      StringToBeReturned = CommonAnswer_Part1 + "ORDER_WRONG%" + CommonAnswer_Part2;
      Returnvalue = ORDER_WRONG;
      counter++;
    }
  }
  else
  {
    StringToBeReturned = CommonAnswer_Part1 + "PW_WRONG%" + CommonAnswer_Part2;
    Returnvalue = ORDER_PW_WRONG;
  }
  return StringToBeReturned;
}

/*****************************************************************************************/
//Function Broadcastmanagement:
//->returns the value to be sent to the App
//->updates the Returnvalue

String Parser::Broadcastmanagement()
{
  String BroadcastAnswer = "BROADCAST_RS%SMART_MINIFAB";
  Returnvalue = BROADCAST;
  return BroadcastAnswer;
}

/****************************************************************************************/
//Function RunParser:
//->returns the integer value (Returnvalue), which controls the main state machine
//->updates the string to be sent to the App

states Parser::RunParser(String ReceivedString_old,int Orders, int RemainingTime)
{
  
  Serial.print("RunParser string: "); // alex_debug
  Serial.println(ReceivedString); // alex_debug
  
  
  if (ReceivedString.charAt(0)=='*')
  {
    ReceivedString = ReceivedString.substring(1,ReceivedString.length()-1); 
    if (ReceivedString == "OPEN")
    {
      Answer = "OPEN";
      Returnvalue = CLIENT_CONNECT; 
    }
    else if (ReceivedString == "CLOS")
    {
      Answer = "CLOS";
      Returnvalue = CLIENT_DISCONNECT;
    }
    else
    {
      Answer = "ERROR";
      Returnvalue = ERROR_STATE;
      
    }
  }
  else 
  {
    ReceivedString = ReceivedString.substring(1,ReceivedString.length()-1); 
    int DelimiterPosition = ReceivedString.indexOf("%");
  String FirstWord = ReceivedString.substring(0,DelimiterPosition); 
  if(FirstWord=="SIGN_IN")
    {
      Serial.println("Anmeldung");
      Serial.println(ReceivedString); // alex_debug
      Answer = Loginmanagement(ReceivedString,numberoforders,RemainingTime_Sek);
    }
    else if (FirstWord=="ORDER")
    {
      Answer = Ordermanagement(ReceivedString,numberoforders,RemainingTime_Sek);
      //Returnvalue=ORDER_SUCCESSFUL; (nur für Iza aktivieren!)
    }
    else if (FirstWord=="SIGN_OUT")
    {
      Answer = Logoutmanagement(ReceivedString,numberoforders,RemainingTime_Sek); 
    }
    else if (FirstWord == "BROADCAST")
    {
      Answer = Broadcastmanagement();
    }
    else
    {
      Returnvalue = ERROR_STATE;
      Answer = "ERROR";
    }
    Answer = "[" + Answer + "]\n";      // set termination character
  }
  return Returnvalue;
}
/**************************************************************************************/
//Function CheckString:
//-> returns true, if string is ok
//-> checking rule 1: 7 different ;-delimited strings received?
//-> checking rule 2: has every string of these 7 the expected length?

bool Parser::CheckString(String OrderString)
{
  int pos;
  int result = 1;
  String Geom, Version,Arrangement;
  bool Status;
  
  //check first string: Geometry
  pos = OrderString.indexOf(";");
  Geom = OrderString.substring(0,pos);
  if ((Geom.length()<1)or(Geom.length()>2)){result =result *0; }
  OrderString = OrderString.substring(pos+1);

  //check second string: Version
  pos = OrderString.indexOf(";");
  Version = OrderString.substring(0,pos);
  OrderString = OrderString.substring(pos+1);
  if (Version.length()!=1) {result =result *0;}

  //check all six Arrangement strings
  for (int i=1; i<7; i++)
  {
    if (pos==-1) {result=result*0;}
    pos = OrderString.indexOf(";");
    Arrangement = OrderString.substring(0,pos);
    if (Arrangement.length()!=5) {result = result * 0;}
    OrderString = OrderString.substring(pos+1);
  }

  //the string is longer than expected
  if (pos!=-1) {result = result * 0;}
  
  if (result ==1)
  {
    Status = true;
  }
  else
  {
    Status = false;
  }
  return Status;
}
/****************************************************************************************/
//Function Get_String_from_Parser:
//->returns the string to be sent to the App

String Parser::Get_String_from_Parser()
{
  return Answer;
}
/***************************************************************************************/









