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
#define ERROR            -1  //!< wifi protocol identiefier not recognized
#define LOGIN_SUCCESSFUL  0  //!< login to wifly module successful (pw corrct)
#define LOGIN_PW_WRONG    1  //!< login to wifly module failed  (pw wront)
#define LOGOUT_SUCCESSFUL 2  //!< logout from wifly module initiated (pw correct)
#define LOGOUT_PW_WRONG   3  //!< logout from wifly modle failed (pw incorrect)
#define ORDER_SUCCESSFUL  4  //!< order received, order string correct
#define ORDER_WRONG       5  //!< order received, order string incorrect
#define ORDER_PW_WRONG    6  //!< order received, pw incorrect
#define BROADCAST         7  //!< broadcast enabled

/******************************************************************************************************************/
// globals


/******************************************************************************************************************/
// function declaration

#endif  // COMMON_H
