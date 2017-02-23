/****************************************************************************************************************//**
   \file    DRIVER_TIMER_H.h
   \brief   Contains definition/declarations for the timer
   \author  Alexander Nahrwold
   \date    20.1.2017
   \todo    
*******************************************************************************************************************/

#ifndef DRIVER_TIMER_H
#define DRIVER_TIMER_H

/******************************************************************************************************************/
// include files
#include "Arduino.h"
#include "SimpleTimer.h"  //!< source @Iza?

/******************************************************************************************************************/
// globals
extern int g_counter_timer;                //!< counts the failed connections to the uC
extern boolean g_doDisableMySimpletimer;   //!< boolean variable - if it is true, the Mysimpletimer will be disabled for the actual running cycle
extern boolean g_doDisableNewOrdersTimer;  //!< boolean variable - if it is true, the Neworderstimer will be disabled for the actual running cycle                  


                               
extern int g_numberOfOrders;               //!< # of incoming orders
extern int g_mySimpleTimerId;              //!< timer ID                              
extern int g_myOrderTimerId;               //!< timer ID
extern int g_newOrdersTimerId;             //!< timer ID

extern SimpleTimer g_timer;                //!< instance of a timer

/******************************************************************************************************************/
// function declaration
bool setupTimer();
void retryTimeout();
void factoryTerminatedOrder();
void newOrderReceived();

#endif  // DRIVER_TIMER_H


