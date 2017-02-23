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

extern int counter;                                        //it counts the failed connections to the uC
extern boolean DoDisable_mysimpletimer;             //boolean variable - if it is true, the Mysimpletimer will be disabled for the actual running cycle
extern boolean DoDisable_neworderstimer;            //boolean variable - if it is true, the Neworderstimer will be disabled for the actual running cycle                  
extern SimpleTimer timer;                                  
extern int numberoforders;                                 //# of incoming orders
extern int mysimpletimerid;                                //timer ID                              
extern int myordertimerid;                                 //timer ID
extern int neworderstimerid;                               //timer ID
extern int RemainingTime_Sek;                              //remaining seconds until factory terminates all registered orders
extern int MyRandomNumber;                                 //a number between 0 and 10 generated with the internal random generator
extern long StartTime_MillSek;                             //time point: starting the process of the first registered order (the very first one and the first one after pausing)
extern int OrderProcessTime_Sek;                           //total time the factory needs to terminate all orders (will be increased at each new incoming order)

/******************************************************************************************************************/
// function declaration
void timerRuntime();
bool setupTimer();
void RetryTimeout();
void NewOrderReceived();
void FactoryTerminatedOrder();

#endif  // DRIVER_TIMER_H


