/****************************************************************************************************************//**
   \file    driver_timer.cpp
   \brief   Contains definition/declarations for the timer
   \author  Alexander Nahrwold
   \date    2.2.2017
********************************************************************************************************************/
#include "driver_timer.h"


SimpleTimer timer; 
int counter;                                        
//boolean DoDisable_myActivityTimer = true;             //boolean variable - if it is true, the Mysimpletimer will be disabled for the actual running cycle       
boolean DoDisable_myOrderTimer = true;                                 
int numberoforders;                                 //# of incoming orders
//int myActivityTimerId;                              //timer ID                              
int myOrderTimerId;                                 //timer ID
int RemainingTime_Sek;                              //remaining seconds until factory terminates all registered orders
long StartTime_MillSek;                             //time point: starting the process of the first registered order (the very first one and the first one after pausing)
int OrderProcessTime_Sek;                           //total time the factory needs to terminate all orders (will be increased at each new incoming order)
int TimeForOneOrder;                        
bool TimerDebug=true; 
String MyOrders[5]={"%","%","%","%","%"};

void timerDebug(bool Setting)
{
  TimerDebug = Setting;
}

void timerRuntime()
{
  //TIMER BLOCK 1 
   timer.run();                                                       //start all timers
//   if (DoDisable_myActivityTimer == true)                               //stop the myactivitytimer immediately; no need for it right now; it will be started in the "Auftragsverwaltung" module
//   {                                                                  
//    timer.disable(myActivityTimerId);
//   }
   if (DoDisable_myOrderTimer ==true)                               //stop the neworderstimer immediately; no need for it right now; it will be started only if 
   {                                                                  //an order is received from the app
    timer.disable(myOrderTimerId);
   }
   if (TimerDebug)
   {
    delay(1000);                                                     //count up in a one-second-tick
   }
    
      if (numberoforders>0)
      {
        RemainingTime_Sek = OrderProcessTime_Sek-(round((millis()-StartTime_MillSek)/1000));      //compute remaining time
        if (TimerDebug)
        {
          Serial.print(RemainingTime_Sek);
          Serial.println(numberoforders);
        }      
      }
      else 
      {
        if (TimerDebug)
        {
                        //factory is on idle right now
        }
      }

}
/****************************************************************************************************************//**
   \brief   setup timer
   \author  Iza
   \date    2.2.2017 ; update: 28.02.2017
********************************************************************************************************************/
bool setupTimer()
{
  bool ret_val = false;
  counter = 0;                                       //counting false order-strings
  TimeForOneOrder = 30;                             //final value = 360 (=3 minutes)
  
  //myActivityTimerId= timer.setInterval (10000,ActivityTimeout);                //timer for control of maximal allowed inactivity time of a user; after time elapsed, user will be deconnected
                                                                            //current setting: 10 seconds   
  myOrderTimerId = timer.setInterval (30000,FactoryTerminatedOneOrder);     //final value = 360000 (=3 minutes)

  numberoforders = 0;                                                    //reset number of orders
  OrderProcessTime_Sek = 0;                                             //define the time the factory needs to process them (no orders, no time needed)
  return( true );
}

/****************************************************************************************************************//**
   \brief   ISR for activity timer 
   \details interrupt routines
   \author  Iza
   \date    2.2.2017, update: 28.02.2017
********************************************************************************************************************/
//void ActivityTimeout()
//{
//  if (TimerDebug)
//  {
//  }
//  
// //code for: 
// //disconnect user
// //INSERT CODE HERE!!!!
//  
//  DoDisable_myActivityTimer  = true;                               //let the program disable the timer in the next running cycle (otherwise it would trigger again automatically in 10 seconds) 
//                
//}

/****************************************************************************************************************//**
   \brief   ISR: triggers when factory executed an order
   \details interrupt routines
   \author  Iza
   \date    2.2.2017, update:28.02.2017
********************************************************************************************************************/
void FactoryTerminatedOneOrder()                                   //factory executed one order
{
  //USE FUNCTION HERE TO FORWARD TO FACTORY
  Serial.print("SENT TO FACTORY: ");Serial.println(MyOrders[0]);
  //USE FUNCTION HERE TO FORWARD TO FACTORY
  
  //shift orders in the array one position to the left 
  for (int i=0;i<=4;i++)
  {
    MyOrders[i]=MyOrders[i+1];
  }
  MyOrders[4]= "%";
  
  numberoforders = numberoforders -1;                           //decrement number of orders
  if (numberoforders ==0)
  {
    if (TimerDebug)
    {
      
      Serial.print(millis());
      Serial.println(")");
    }
    DoDisable_myOrderTimer = true;                              //let the main programm deactivate this timer; it will be activated again, if new orders are coming - controlled in the "Auftragsverwaltung" module
    OrderProcessTime_Sek = 0; 
  }
  else
  {
    if (TimerDebug)
    {
       
       Serial.println(numberoforders);
    }

  }
}
//********************************************************************************************************
//Function AddStringToArray
//inserts the last incoming valid order to the order array;
//from this order array the factory will take one (the oldest one) after the current order processing is over
//NewString: last incoming accepted order
void AddStringToArray(String NewString)
{
  int LastEmptyPos;
  for (int i=4; i>=0; i--)
  {
    if (MyOrders[i]=="%")
    {
      LastEmptyPos = i;
      //Serial.print(String(i));
    }
  }
  Serial.println();
  MyOrders[LastEmptyPos]=NewString;
  //use next for debugging:
  
//  for (int j=0;j<=4;j++)
//  {
//    Serial.print(MyOrders[j]);
//    Serial.print("***");
//  }
//  Serial.println();
}

