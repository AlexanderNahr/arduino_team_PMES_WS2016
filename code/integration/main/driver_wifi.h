/****************************************************************************************************************//**
   \file    driver_wifi_protocol.h
   \brief   Contains definition/declarations for the WIFI driver for Wifly Module (only hardware dependent stuff k)
   \author  Alexander Nahrwold
   \date    20.1.2017
   \todo    
*******************************************************************************************************************/

#ifndef DRIVER_WIFI_H
#define DRIVER_WIFI_H


/******************************************************************************************************************/
// include files
#include "Arduino.h"

/******************************************************************************************************************/
// globals


/******************************************************************************************************************/
// function declaration

/*
typedef struct WifiClientDevice
{
  int IPAddress;        // TODO: fix data type
  int ClientMacAddress      // TODO: fix data type
  int RemainingLeaseTime;     // TODO: fix data type
  char* HostName;       // TODO: fix data type

} WifiClientDevice;

typdef struct DeviceParamters
{
  // What parameters do we want variable, if any at all?
} DeviceParamters;

// TODO: fix return values and parameters
void configureAccessPoint( DeviceParamters parameter_instance );  //!< cofigures device as accesspoint 
void checkCurrentDevice( WifiClientDevice device_instance );    //!< return the current connected device parameters
void checkLastDevices( WifiClientDevice device_instance[] );    //!< returns list of the last connected devices
void rebootWiflyModule();                     //!< reboots module in case of error

*/
void setupWifiModule();     //!< sets up the wifi module as TCP server
#endif  // DRIVER_WIFI_H
