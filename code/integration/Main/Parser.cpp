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
  Password = "ilias2017";
  //.reserve(150);
  String Answer;
  StringToFactory = "";

}


/****************************************************************************/
//Function Loginmanagement:
//->checks if the password is correct and returns the string to be sent to the App
//->updates the Returnvalue

String Parser::Loginmanagement(byte& Orders, int& Time)
{
  Serial.println(g_received_string); // alex_debug
  String StringToBeReturned;
  String CommonAnswer = "SIGN_IN_RS%" + String(Orders) + "%" + String(Time) + "%";
  int pos = g_received_string.indexOf("%");
  String ReceivedPassword = g_received_string.substring(pos+1);//, g_received_string.length()-1);
 // Serial.println("Loginmanagement pw: " + ReceivedPassword); // alex_debug
  
  if (ReceivedPassword==Password)
  {
    StringToBeReturned = CommonAnswer + "SUCCESSFUL";
    Returnvalue = LOGIN_SUCCESSFUL;
  }
  else
  {
    Serial.print(F("pwWrong"));  
    StringToBeReturned = CommonAnswer + "PW_WRONG";
    Returnvalue = LOGIN_PW_WRONG;
  }
  return StringToBeReturned;

}

/**********************************************************************************/
//Function Logoutmanagement:
//->checks if the password is correct and returns the string to be sent to the App
//->updates the Returnvalue

String Parser::Logoutmanagement(byte& Orders, int& Time)
{
  String StringToBeReturned= "SIGN_OUT";
  //int pos = g_received_string.indexOf("%");
  //String ReceivedPassword = g_received_string.substring(pos+1);
  //if (ReceivedPassword==Password)
  //{
  //  Returnvalue = LOGOUT_SUCCESSFUL;
 // }
 // else
 // {
  //  Returnvalue = LOGOUT_PW_WRONG;
 // }
 Returnvalue = LOGOUT_SUCCESSFUL;
  return StringToBeReturned;

}

/***********************************************************************************/
//Function Ordermanagement:
//->checks if password and order-string are ok
//->returns the string to be sent to the App
//->updates the Returnvalue

String Parser::Ordermanagement(byte& Orders, int& Time)
{
  Serial.print(F("str:"));
  Serial.println(g_received_string); // alex_debug
  String StringToBeReturned;
  String CommonAnswer_Part1 = "ORDER_RS%";
  String CommonAnswer_Part2 =  String(Orders) + "%" + String(Time);
  int pos1 = g_received_string.indexOf("%");
  String SecondPart = g_received_string.substring(pos1+1);
  int pos2 = SecondPart.indexOf("%");
  String ReceivedPassword = SecondPart.substring(0,pos2);
  String ThirdPart = SecondPart.substring(pos2+1);

  if (ReceivedPassword==Password)
  {
    StringToFactory=ThirdPart;
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

states Parser::RunParser(byte& Orders, int& RemainingTime)
{
  
  Serial.print(F("RunParser string: ")); // alex_debug
  Serial.println(g_received_string); // alex_debug
  
  
  if (g_received_string.charAt(0)=='*')
  {
    g_received_string = g_received_string.substring(1,g_received_string.length()-1); 
    if (g_received_string == "OPEN")
    {
      Answer = "OPEN";
      Returnvalue = CLIENT_CONNECT; 
    }
    else if (g_received_string == "CLOS")
    {
      Answer = "CLOS";
      Returnvalue = CLIENT_DISCONNECT;
    }
    else
    {
      Answer = "SIGN_IN_RS%" + String(Orders) + "%" + String(RemainingTime)+ "%ERROR";
      Returnvalue = ERROR_STATE;
      
    }
  }
  else 
  {
    Serial.print(F("Parser else: ")); // alex_debug
    Serial.println(g_received_string); // alex_debug
    Serial.print(F("substring output: "));
    Serial.println(g_received_string.length()-1);
    //g_received_string = g_received_string;
    Serial.print(F("parser memory full? "));
    Serial.println( g_received_string );
    g_received_string = g_received_string.substring(1,g_received_string.length()-1); 
    Serial.print(F("Parser after else: ")); // alex_debug
    Serial.println(g_received_string); // alex_debug
    int DelimiterPosition = g_received_string.indexOf("%");
    String FirstWord = g_received_string.substring(0,DelimiterPosition); 
    Serial.print(F("Parser first word: ")); // alex_debug
    Serial.println(FirstWord); // alex_debug
    if(FirstWord=="SIGN_IN")
    {
      Serial.println(F("Anmeldung"));
      Serial.println(g_received_string); // alex_debug
      Answer=Loginmanagement(numberoforders,RemainingTime_Sek);
    }
    else if (FirstWord=="ORDER")
    {
      Serial.print(F("Parser order detected: ")); // alex_debug
      Serial.println(Answer); // alex_debug
      Answer = Ordermanagement(numberoforders,RemainingTime_Sek);
    }
    else if (FirstWord=="SIGN_OUT")
    {
      Answer = Logoutmanagement(numberoforders,RemainingTime_Sek); 
    }
    else if (FirstWord == "BROADCAST")
    {
      Answer = Broadcastmanagement();
    }
    else
    {
      Returnvalue = ERROR_STATE;
      Answer = "SIGN_IN_RS%" + String(Orders) + "%" + String(RemainingTime)+ "%ERROR";
    }
    Answer = "[" + Answer + "]\n";      // set termination character
  }
  Serial.print(F("Parser answer: ")); // alex_debug
  Serial.println(Answer); // alex_debug
  return Returnvalue;
}
/**************************************************************************************/
//Function CheckString:
//-> returns true, if string is ok
//-> checking rule 1: 7 different ;-delimited strings received?
//-> checking rule 2: has every string of these 7 the expected length?

bool Parser::CheckString(String& OrderString)
{
  int pos,Geom_Int;
  int result = 1;
  String Geom, Version,Arrangement;
  bool Status;

  Serial.print(F("Check string orderstring: ") ); 
  Serial.println(OrderString);
  //check first string: Geometry
  pos = OrderString.indexOf(";");
  Geom = OrderString.substring(0,pos);
  Serial.print(F("Check string geom: " )); 
  Serial.println(Geom);
  if ((Geom.length()<1)or(Geom.length()>2)){result =result *0; }
  Serial.print(F("Check string first part OK? ") ); 
  Serial.println(result);
  OrderString.remove(0, pos+1);
  Serial.print(F("Check string orderstring sub: ") ); 
  Serial.println(OrderString);
  Geom_Int=Geom.toInt();
  Serial.print(F("Check string geom int: ") ); 
  Serial.println(Geom_Int);
  
  //check second string: Version
  pos = OrderString.indexOf(";");
  Version = OrderString.substring(0,pos);
  OrderString.remove(0, pos+1);
  if (Version.length()!=1) {result =result *0;}                 // this is where it bails out -> result is 0 because OrderString is empty
  Serial.print(F("Check string version OK? ") ); 
  Serial.println(result);
  //check all six Arrangement strings
  for (int i=1; i<(Geom_Int+1); i++)
  {
    if (pos==-1) {result=result*0;}
    pos = OrderString.indexOf(";");
    Arrangement = OrderString.substring(0,pos);
    if (Arrangement.length()!=5) {result = result * 0;}
    OrderString.remove(0, pos+1);
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
String Parser::ToFactory()
{
  return StringToFactory;
}








