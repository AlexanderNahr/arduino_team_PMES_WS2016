/****************************************************************************************************************//**
   \file    timer.cpp
   \brief   Contains definition/declarations for the timer
   \author  Alexander Nahrwold
   \date    2.2.2017
********************************************************************************************************************/

/******************************************************************************************************************/
// include files
#include "driver_timer.h"

/******************************************************************************************************************/
// globals
int g_counter_timer               = 0;
boolean g_doDisableMySimpletimer  = true;
boolean g_doDisableNewOrdersTimer = true;



int g_numberOfOrders              = 0;
int g_mySimpleTimerId             = 0;
int g_myOrderTimerId              = 0;
int g_newOrdersTimerId            = 0;

SimpleTimer g_timer;

/****************************************************************************************************************//**
   \brief   ISR for retry counter
   \details TIMER BLOCK 3 - interrupt routines
   \author  Iza
   \date    2.2.2017
********************************************************************************************************************/
bool setupTimer()
{
  g_counter_timer = 1;                                                        // counting failed connections

  //TIMER BLOCK 0
  g_mySimpleTimerId   = g_timer.setInterval (8000, retryTimeout);             // 8 seconds timeout for ARDUINO_ACK_RETRY_TIMEOUT
  g_myOrderTimerId    = g_timer.setInterval (15000, factoryTerminatedOrder);  // 15 seconds for each order to be completed by the factory (in reality 3 minutes)
  g_newOrdersTimerId  = g_timer.setInterval (5000, newOrderReceived);         // after the factory executed all orders, 5 seconds later new orders "are received"
  //END OF TIMER BLOCK 0

  g_numberOfOrders = 2;                                                       // suppose that there are already some orders registered (starting state)
}
/****************************************************************************************************************//**
   \brief   ISR for retry counter
   \details TIMER BLOCK 3 - interrupt routines
   \author  Iza
   \date    2.2.2017
********************************************************************************************************************/
void retryTimeout()
{
  Serial.print("Channel is open again (timestamp (msec): ");
  Serial.print (millis());
  Serial.println(")");
  g_counter_timer = 1;                 // failed connections counter is reset
  g_doDisableMySimpletimer  = true;    // let the program disable the timer in the next running cycle (otherwise it would trigger again automatically in 8 seconds)
}

/****************************************************************************************************************//**
   \brief   ISR: retry counter
   \details TIMER BLOCK 3 - interrupt routines
   \author  Iza
   \date    2.2.2017
********************************************************************************************************************/
void factoryTerminatedOrder()                                   //factory executed one order
{
  g_numberOfOrders = g_numberOfOrders - 1;                          //decrement number of orders
  if (g_numberOfOrders == 0)
  {
    Serial.print("All orders terminated (time: ");
    Serial.print(millis());
    Serial.println(")");
  }
  else
  {
    Serial.print("Factory ready with one order. Remaining orders= ");
    Serial.println(g_numberOfOrders);
  }
}

/****************************************************************************************************************//**
   \brief   ISR: new orders will be "generated" (as if the uC would receive a/some new order(s) ). THIS IS A DEMO!
   \details TIMER BLOCK 3 - interrupt routines
   \author  Iza
   \date    2.2.2017
********************************************************************************************************************/
void newOrderReceived()
{
  g_numberOfOrders = 2;                             // new orders "received"
  Serial.print(g_numberOfOrders);
  Serial.print (" new orders received (time: ");
  Serial.print(millis());
  Serial.println(")");

  g_timer.enable(g_myOrderTimerId);                 // enable the myordertimer
  g_timer.restartTimer(g_myOrderTimerId);           // restart it  (reference time is NOW)

  g_doDisableNewOrdersTimer = true;               // let the program disable the timer in the next running cycle
  // (otherwise it would trigger again automatically in 5 seconds)
}

//END OF TIMER BLOCK 3
