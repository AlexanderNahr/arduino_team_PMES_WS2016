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
#include "string.h"

//Constructor
Parser::Parser()
{
  Password = "ilias2017";
}


/****************************************************************************/
//Function Loginmanagement:
//->checks if the password is correct and returns the string to be sent to the App
//->updates the Returnvalue

String Parser::Loginmanagement(String ReceivedString, int Orders, int Time)
{
  String StringToBeReturned;
  String CommonAnswer = "SIGN_IN_RS%" + String(Orders) + "%" + String(Time) + "%";
  int pos = ReceivedString.indexOf("%");
  String ReceivedPassword = ReceivedString.substring(pos+1);
  if (ReceivedPassword==Password)
  {
    StringToBeReturned = CommonAnswer + "SUCCESSFUL";
    Returnvalue = LOGIN_SUCCESSFUL;
  }
  else
  {
    StringToBeReturned = CommonAnswer + "PW_WRONG";
    Returnvalue = LOGIN_PW_WRONG;
  }
  return StringToBeReturned;
}

/**********************************************************************************/
//Function Logoutmanagement:
//->checks if the password is correct and returns the string to be sent to the App
//->updates the Returnvalue

String Parser::Logoutmanagement(String ReceivedString, int Orders, int Time)
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

String Parser::Ordermanagement(String ReceivedString, int Orders, int Time)
{
  String StringToBeReturned;
  String CommonAnswer_Part1 = "ORDER_RS%";
  String CommonAnswer_Part2 =  String(Orders) + "%" + String(Time);
  int pos1 = ReceivedString.indexOf("%");
  String SecondPart = ReceivedString.substring(pos1+1);
  int pos2 = SecondPart.indexOf("%");
  String ReceivedPassword = SecondPart.substring(0,pos2);
  String ThirdPart = SecondPart.substring(pos2+1);
  Serial.print("THIRDPART = ");
  Serial.println(ThirdPart);
  if (ReceivedPassword==Password)
  {
    StringToBeReturned = CommonAnswer_Part1 + "SUCCESSFUL%" + CommonAnswer_Part2;
    //Returnvalue = 4;
    bool StringOK = CheckString(ThirdPart);
    if (StringOK==true)
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

states Parser::RunParser(String ReceivedString,int Orders, int RemainingTime)
{
  String ReceivedString_NoDelim = ReceivedString.substring(1,ReceivedString.length()-1);
  //decide if first character is a * or [
  if (ReceivedString.charAt(0)=="*") //the message is a connection message
  {
    if (ReceivedString_NoDelim == "OPEN")
    {
      Answer = "OPEN"; //actually no response string requested
      Returnvalue = CLIENT_CONNECT;
    }
    else if (ReceivedString_NoDelim=="CLOS")
    {
      Answer = "CLOS"; //actually no response string requested
      Returnvalue = CLIENT_DISCONNECT;
    }
    else //this case theoretically cannot even happen, but for all cases: 
    {
      Answer = "ERROR";
      Returnvalue = ERROR_STATE;
    }
  }
  else //the message is a command message (detected with the [ and ] delimiters
  {
    int DelimiterPosition = ReceivedString_NoDelim.indexOf("%");
    String FirstWord = ReceivedString_NoDelim.substring(0,DelimiterPosition);
    if(FirstWord=="SIGN_IN")
    {
      Answer = Loginmanagement(ReceivedString_NoDelim,0,0);
      Answer = "[" + Answer + "]\n";
    }
    else if (FirstWord=="ORDER")
    {
        Answer = Ordermanagement(ReceivedString_NoDelim,0,0);
        Serial.println("Answer is=");
        Serial.println(Answer);
        Answer = "[" + Answer + "]\n";
    }
    else if (FirstWord=="SIGN_OUT")
    {
         Answer = Logoutmanagement(ReceivedString_NoDelim,0,0); 
         Answer = "[" + Answer + "]\n";
    }
    else if (FirstWord == "BROADCAST")
    {
         Answer = Broadcastmanagement();
         Answer = "[" + Answer + "]\n";
    }
            else
            {
              Returnvalue = ERROR_STATE;
              Answer = "ERROR";
              Answer = "[" + Answer + "]\n";
            }   
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
  String Geom;
  String Version;
  String Arrangement;
  String HelpString;
  String OrderString2="";
  bool Status;
  Serial.print("checkstring received= ");
  Serial.println(OrderString);
  OrderString2=OrderString;
  Serial.print("OrderString2 =" );
  Serial.println(OrderString2);
  
  //check first string: Geometry
  pos = OrderString2.indexOf(";");
  Geom = OrderString2.substring(0,pos);
  Serial.println("Geom: " + Geom);
  if ((Geom.length()<1)or(Geom.length()>2)){result =result *0; }
  OrderString2 = OrderString2.substring(pos+1);
  
  Serial.print("new orderstring= ");
  Serial.println(OrderString2);

  //check second string: Version
  pos = OrderString2.indexOf(";");
  Version = OrderString2.substring(0,pos);
  Serial.println(Version);
  OrderString2 = OrderString2.substring(pos+1);
  Serial.print("new orderstring = ");
    Serial.println(OrderString2);
  if (Version.length()!=1) {result =result *0;}

  //check all six Arrangement strings
  for (int i=1; i<7; i++)
  {
    if (pos==-1) {result=result*0;} //if end of string too early reached (string does not contain all required elements)
    pos = OrderString2.indexOf(";");
    Arrangement = OrderString2.substring(0,pos);
    Serial.println("Arr= "+Arrangement);
    if (Arrangement.length()!=5) {result = result * 0;}
    OrderString2 = OrderString2.substring(pos+1); 
    Serial.print("new orderstring = ");
    Serial.println(OrderString2);
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









