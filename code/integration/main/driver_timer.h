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
#include "common.h"

/******************************************************************************************************************/
// globals

extern int counter;                                        //it counts the failed connections to the uC
//extern boolean DoDisable_myActivityTimer;             //boolean variable - if it is true, the Mysimpletimer will be disabled for the actual running cycle
extern boolean DoDisable_myOrderTimer;                 
extern SimpleTimer timer;                                  
extern byte numberoforders;                                 //# of incoming orders
//extern int myActivityTimerId;                                //timer ID                              
extern int  myOrderTimerId;                                 //timer ID
extern int RemainingTime_Sek;                              //remaining seconds until factory terminates all registered orders
extern long StartTime_MillSek;                             //time point: starting the process of the first registered order (the very first one and the first one after pausing)
extern int OrderProcessTime_Sek;                           //total time the factory needs to terminate all orders (will be increased at each new incoming order)
extern int TimeForOneOrder;                          //IN SECONDS! 


/******************************************************************************************************************/
// function declaration
void timerRuntime();
bool setupTimer();
void ActivityTimeout();
void FactoryTerminatedOneOrder();
void timerDebug(bool& Setting);
void AddStringToArray(String& NewString);
#endif  // DRIVER_TIMER_H


