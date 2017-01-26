/****************************************************************************************************************//**
   \file    driver_wifi_protocol.h
   \brief   Contains definition/declarations for the WIFI protocol and how to interpret it (hardware independent)
   \author  Alexander Nahrwold
   \date    20.1.2017
*******************************************************************************************************************/

#ifndef DRIVER_WIFI_PROTOCOL_H
#ifndef DRIVER_WIFI_PROTOCOL_H

#define WIFI_TERMINATION_START '['			//!< first char of valid wifi message -> maybe this should go into the UART driver
#define WIFI_TERMINATION_END   ']'			//!< last char of a valid wifi message -> maybe this should go into the UART driver

#define Protokolldefinitionsn aus xlsx file go here

typedef MessageParts
{
	int x;
	int y;
	int color;
	int location;
} MessageParts;

void parseFromUART( char* message_from_uart, MessageParts* message_instance);		//!< translates from UART string to internal command
void parseToUART( MessageParts* message_instance, char* message_to_uart, );			//!< translates from internal commnd to UART string


#endif  // DRIVER_WIFI_PROTOCOL_H