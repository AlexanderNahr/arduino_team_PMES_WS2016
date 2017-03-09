/****************************************************************************************************************//**
   \file    common.h
   \brief   contains common declarations throughout this project
   \author  Alexander Nahrwold
   \date    24.2.2017
   \todo    
*******************************************************************************************************************/

#ifndef COMMON_H
#define COMMON_H


/******************************************************************************************************************/
// include files
#include "Arduino.h"

/******************************************************************************************************************/
// definitions 

// STATEMACHINE
typedef enum states
{
	ERROR_STATE       = -1,  //!< wifi protocol identifier not recognized
	LOGIN_SUCCESSFUL  =  0,  //!< login to wifly module successful (pw corrct)
	LOGIN_PW_WRONG    =  1,  //!< login to wifly module failed  (pw wront)
	LOGOUT_SUCCESSFUL =  2,  //!< logout from wifly module initiated (pw correct)
	LOGOUT_PW_WRONG   =  3,  //!< logout from wifly modle failed (pw incorrect)
	ORDER_SUCCESSFUL  =  4,  //!< order received, order string correct
	ORDER_WRONG       =  5,  //!< order received, order string incorrect
	ORDER_PW_WRONG    =  6,  //!< order received, pw incorrect
	BROADCAST         =  7,  //!< broadcast enabled
	CLIENT_CONNECT	  =  9,  //!< *OPEN* received from wify module (connect)
	CLIENT_DISCONNECT = 10   //!< *CLOS* received from wify module (disconnect)	
} STATES;

/******************************************************************************************************************/
// globals
extern String g_received_string;                      //!< receive buffer uart
//extern String g_MyOrders[5];                          //!< storage for incoming orders to be passed to factory

/******************************************************************************************************************/
// function declaration

#endif  // COMMON_H
