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
  ReceivedString.reserve(50);

}


/****************************************************************************/
//Function Loginmanagement:
//->checks if the password is correct and returns the string to be sent to the App
//->updates the Returnvalue

String Parser::Loginmanagement(char *p, int Orders, int Time)
{

  String StringToBeReturned="";
  //Serial.print("SecPart: ");Serial.println(p);
  String CommonAnswer = "SIGN_IN_RS%" + String(Orders) + "%" + String(Time) + "%";
  char Pass[10];
  Password.toCharArray(Pass,10);
  //Serial.print("DefinedPass: ");Serial.println(Pass);
  if (strcmp(p,Pass)==0)
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

String Parser::Logoutmanagement()
{
  String StringToBeReturned= "SIGN_OUT";
  return StringToBeReturned;
}

/***********************************************************************************/
//Function Ordermanagement:
//->checks if password and order-string are ok
//->returns the string to be sent to the App
//->updates the Returnvalue

String Parser::Ordermanagement(char *p, int Orders, int Time)
{
  String StringToBeReturned="";
  String CommonAnswer_Part1 = "ORDER_RS%";
  String CommonAnswer_Part2 =  String(Orders) + "%" + String(Time);

      char *s;
      s=strstr(p,"%");s++;
     
      //Serial.print("CH ");Serial.println(s);
    
      int k;
      k=s-p-1;
      if (k<0) {k=7;}
      
      char FirstWord[k+1];
      memset(FirstWord, '\0', sizeof(FirstWord));
      strncpy(FirstWord,p,k);
      //Serial.print("FIRSTWORD: ");Serial.println(FirstWord);
      char Pass[10];
      Password.toCharArray(Pass,10);
      
      p = p + k + 1;     
  
  if (strcmp(Pass,FirstWord)==0)
  {
    bool StringOK = CheckString(p);
    if (StringOK)
    {
      StringToBeReturned = CommonAnswer_Part1 + "SUCCESSFUL%" + CommonAnswer_Part2;
      Returnvalue=ORDER_SUCCESSFUL;
      //Serial.print("PARS: "); Serial.println(StringToBeReturned);
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

states Parser::RunParser(char CharArr[],int Orders, int RemainingTime)
{
  //Serial.print("CH_PARSER: ");Serial.println(CharArr);
  //String ReceivedString = "aaa";
  //Serial.print("RunParser string: "); // alex_debug
  //Serial.println(ReceivedString); // alex_debug

  char CharArr_NoDelim[100];
  memset(CharArr_NoDelim, '\0', sizeof(CharArr_NoDelim));
  char *src;
  src=CharArr;src++;
  memcpy(CharArr_NoDelim,src,(strlen(CharArr)-2));
  
  //Serial.print("CH_PARSER_FIRSTCH: ");Serial.println(CharArr[0]);
  if (CharArr[0]=='*')
  {
   // Serial.println("STAR DETECTED");
    
   // Serial.print("CH_PARSER_COPY: ");Serial.println(CharArr_NoDelim);
    //ReceivedString = ReceivedString.substring(1,ReceivedString.length()-1); 
    if (strcmp(CharArr_NoDelim,"OPEN")==0)
    {
      Answer = "OPEN";
      Returnvalue = CLIENT_CONNECT; 
    //  Serial.println("OPEN DETECTED");
    }
    else if (strcmp(CharArr_NoDelim,"CLOS")==0)
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
      char *s;
      s=strstr(CharArr_NoDelim,"%");s++;
      char *s2;
      s2=strstr(s,"%");s2++;
     // Serial.print("CH ");Serial.println(s);
      //Serial.print("THIRD ");Serial.println(s2);
      char *p;
      p = CharArr_NoDelim;
      int k;
      k=s-p-1;
      if (k<0) {k=7;}
      p = p + k + 1;
      char FirstWord[k+1];
      memset(FirstWord, '\0', sizeof(FirstWord));
      strncpy(FirstWord,CharArr_NoDelim,k);
      //Serial.print("FIRSTWORD: ");Serial.println(FirstWord);

      
    //int DelimiterPosition = ReceivedString.indexOf("%");
  //String FirstWord = ReceivedString.substring(0,DelimiterPosition); 
  if(strcmp(FirstWord,"SIGN_IN")==0)
    {
      //Serial.println("Anmeldung");
      //Serial.println(ReceivedString); // alex_debug
      Answer = Loginmanagement(p,numberoforders,RemainingTime_Sek);
      //Serial.print("RETURN LOGIN: "); Serial.println(String(Answer));
    }
    else if (strcmp(FirstWord,"ORDER")==0)
        {
         // Serial.println("Order");
          Answer = Ordermanagement(s,numberoforders,RemainingTime_Sek);
          Answer = "[" + Answer + "]\n";
        }
        else if (strcmp(FirstWord,"SIGN_OUT")==0)
            {
             // Serial.println("Signout");
             Answer = Logoutmanagement(); 
             Answer = "[" + Answer + "]\n";
            }
       else if (strcmp(FirstWord,"BROADCAST")==0)
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

bool Parser::CheckString(char *p)
{
  int k;
  int result = 1;
  bool Status;

      //Serial.print("ORDERMAN REC: ");Serial.println(p);
      char *s;
      //extract Geom
      s=strstr(p,";");s++;
      k=s-p-1;
      if (k<0) {k=7;}
      
      char Geom[k+1];
      memset(Geom, '\0', sizeof(Geom));
      strncpy(Geom,p,k);
      //Serial.print("GEOM: ");Serial.println(Geom);
      p = p + k + 1;
      if ((strlen(Geom)<1)or(strlen(Geom)>2)){result =result *0; }
      //Serial.print("RES: ");Serial.println(result);

      //extract Version:
      s=strstr(p,";");s++;
      k=s-p-1;
      if (k<0) {k=7;}
      
      char Version[k+1];
      memset(Version, '\0', sizeof(Version));
      strncpy(Version,p,k);
      //Serial.print("VERSION: ");Serial.println(Version);
      p = p + k + 1;
      if (strlen(Version)!=1) {result =result *0;}
      //Serial.print("RES: ");Serial.println(result);
      
      for (int i=1; i<7; i++)
  {
      
      s=strstr(p,";");s++;
      k=s-p-1;
      if (k<0) {k=7;}
      
      char Arrangement[k+1];
      memset(Arrangement, '\0', sizeof(Arrangement));
      strncpy(Arrangement,p,k);
      //Serial.print("Arrangement: ");Serial.println(Arrangement);
      p = p + k + 1;
       if (strlen(Arrangement)!=5) {result =result *0;}
       //Serial.print("RES: ");Serial.println(result);
  }
//  String OrderString = "aaa";
//  //check first string: Geometry
//  pos = OrderString.indexOf(";");
//  Geom = OrderString.substring(0,pos);
//  if ((Geom.length()<1)or(Geom.length()>2)){result =result *0; }
//  OrderString = OrderString.substring(pos+1);
//
//  //check second string: Version
//  pos = OrderString.indexOf(";");
//  Version = OrderString.substring(0,pos);
//  OrderString = OrderString.substring(pos+1);
//  if (Version.length()!=1) {result =result *0;}
//
//  //check all six Arrangement strings
//  for (int i=1; i<7; i++)
//  {
//    if (pos==-1) {result=result*0;}
//    pos = OrderString.indexOf(";");
//    Arrangement = OrderString.substring(0,pos);
//    if (Arrangement.length()!=5) {result = result * 0;}
//    OrderString = OrderString.substring(pos+1);
//  }
//
//  //the string is longer than expected
//  if (pos!=-1) {result = result * 0;}
//  
  if (result ==1)
  {
    Status = true;
  }
  else
  {
    Status = false;
  }
  //Serial.print("STATUS: ");Serial.println(Status);
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









