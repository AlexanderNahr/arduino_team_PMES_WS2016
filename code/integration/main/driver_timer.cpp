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
byte numberoforders;                                 //# of incoming orders
//int myActivityTimerId;                              //timer ID
int myOrderTimerId;                                 //timer ID
int RemainingTime_Sek;                              //remaining seconds until factory terminates all registered orders
long StartTime_MillSek;                             //time point: starting the process of the first registered order (the very first one and the first one after pausing)
int OrderProcessTime_Sek;                           //total time the factory needs to terminate all orders (will be increased at each new incoming order)
int TimeForOneOrder;
bool TimerDebug = true;

void timerDebug(bool Setting)
{
  TimerDebug = Setting;
}

void timerRuntime()
{
  //TIMER BLOCK 1
  timer.run();                                                       //start all timers
  if (DoDisable_myOrderTimer == true)                              //stop the neworderstimer immediately; no need for it right now; it will be started only if
  { //an order is received from the app
    timer.disable(myOrderTimerId);
  }
  if (TimerDebug)
  {
    delay(1000);                                                     //count up in a one-second-tick
  }

  if (numberoforders > 0)
  {
    RemainingTime_Sek = OrderProcessTime_Sek - (round((millis() - StartTime_MillSek) / 1000)); //compute remaining time
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
  TimeForOneOrder = 180;                             //final value = 180 (=3 minutes)

  //Try to reserve space for the string array -> FAILED
  //  for (int i=0; i<=4; i++)
  //  {
  //    g_MyOrders[i].reserve(142);
  //  }

  myOrderTimerId = timer.setInterval (180000, FactoryTerminatedOneOrder);    //final value = 180000 (=3 minutes)

  numberoforders = 0;                                                    //reset number of orders
  OrderProcessTime_Sek = 0;                                             //define the time the factory needs to process them (no orders, no time needed)
  return ( true );
}

/****************************************************************************************************************//**
   \brief   ISR: triggers when factory executed an order
   \details interrupt routines
   \author  Iza
   \date    2.2.2017, update:28.02.2017
********************************************************************************************************************/
void FactoryTerminatedOneOrder()                                   //factory executed one order
{
  if (numberoforders > 1)
  {
    //USE FUNCTION HERE TO FORWARD TO FACTORY
    Serial.print("SENT TO FACTORY: "); Serial.println(g_MyOrders[0]);
    //USE FUNCTION HERE TO FORWARD TO FACTORY

    //shift orders in the array one position to the left
    for (int i = 0; i <= 4; i++)
    {
      g_MyOrders[i] = g_MyOrders[i + 1];
    }
    g_MyOrders[4] = "%";
  }


  numberoforders = numberoforders - 1;                          //decrement number of orders
  if (numberoforders == 0)
  {
    g_ForwardImmediately = true;
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
void AddStringToArray(String& NewString)
{
  int LastEmptyPos;
  for (int i = 4; i >= 0; i--)
  {
    if (g_MyOrders[i] == "%")
    {
      LastEmptyPos = i;
      //Serial.print(String(i));
    }
  }
  Serial.println();
  g_MyOrders[LastEmptyPos] = NewString;
  //use next for debugging:

  //  for (int j=0;j<=4;j++)
  //  {
  //    Serial.print(g_MyOrders[j]);
  //    Serial.print("***");
  //  }
  //  Serial.println();
}

