/****************************************************************************************************************//**
   \file    Auftragsverwaltung.cpp
   \brief   Controls the orders received from the App
   \author  Izabella Sandor
   \date    Feb 28, 2017
*******************************************************************************************************************/


/******************************************************************************************************************/
// include files

#include <Arduino.h>
#include "driver_timer.h"   
#include "Auftragsverwaltung.h"

//Constructor
Auftragsverwaltung::Auftragsverwaltung()
{
  NewClient = true;
  MaxNoOfOrders = 5;
}


/****************************************************************************/
//Function NewClientDetected:
//->sets/resets some variables (like an init)
//->starts activity timer if ordering is theoretically allowed (=factory on idle) 
void Auftragsverwaltung::NewClientDetected()
{
      NewClient = true;
      counter= 0;
      ClientTotalOrders = 0;
      
      //if ordering possible-> start tracing activity of client (in case of no activity, client will be disconnected)
      if (numberoforders==0)
      {
        OrderProcessTime_Sek=0;                         //if there are no orders left in the queue, reset OrderProcessTime
        //activate timer
        timer.enable(myActivityTimerId);
        timer.restartTimer(myActivityTimerId);
        DoDisable_myActivityTimer = false;
        //end if timer activation
      }
}
/**********************************************************************************************************************/
//Function NewOrderRegistered:
//
String Auftragsverwaltung::NewOrderRegistered(String StringFromParser,int Orders, int Time)
{
        String myAnswerString="";
        
        //if max allowed orders from client reached: inform client
        if (ClientTotalOrders>=MaxNoOfOrders)
        {
          
          //Serial.println("ORDERING NOT ALLOWED: Client has reached the allowed limit (5 orders/client");
          myAnswerString = "ORDER_RS%MAX_REACHED%" + String(Orders) + "%" + String(Time);
          myAnswerString = "[" + myAnswerString + "]\n";
          
        }
        else
        {
          //one order received from client: restart myactivitytimer
          timer.enable(myActivityTimerId);
          timer.restartTimer(myActivityTimerId);
          DoDisable_myActivityTimer = false;
          //end of timer activation
          
          //increment counter 
          ClientTotalOrders++;

          //new client, but still not terminated orders in the factory: inform client, that ordering not possible
          if ((numberoforders!=0)&&(NewClient))
          {
            //Serial.println("ORDERING NOT ALLOWED! Factory still has orders from last client");
            //Serial.println();
            myAnswerString = "ORDER_RS%EXT_ORDER%" + String(Orders) + "%" + String(Time);
            myAnswerString = "[" + myAnswerString + "]\n";
            
          }
          //old client 
          else
          {
            timer.enable(myOrderTimerId);                              //enable the formerly disabled timer    
            if ((NewClient)||(numberoforders==0))
            {
              timer.restartTimer(myOrderTimerId);                        //restart timer (reference time is NOW); do it only in case of the first order from a client; or if all orders are already terminated 
              StartTime_MillSek = millis();
            }
            
            DoDisable_myOrderTimer = false;                             //timer should NOT be deactivated in main
            numberoforders=numberoforders+1;                                                 
            OrderProcessTime_Sek = OrderProcessTime_Sek + TimeForOneOrder;   //increase the time the factory needs to process the orders 
            NewClient = false;                                          //label this client, as: known!              
          }
        }
        if (myAnswerString=="") 
        {
          myAnswerString = StringFromParser;
        }
        return myAnswerString;
}
/****************************************************************************************************************************************************************************************************************************/









