/*/****************************************************************************************************************//**
   \file    spike_loopack_mode.c
   \brief   Spike solution for loopback mode between Wifly Module and Arduino
   \details uses telnet on host pc, connecting to IP 1.2.3.4 port 2000 and Arduino Serial Monitor to display messages
            Code based on SoftwareSerialExample (see Arduino->Datei->Beispiele->SoftwareSerial)

            The circuit:
             RX is digital pin 10 (connect to TX of other device)
             TX is digital pin 11 (connect to RX of other device)        
   \author  Alexander Nahrwold
   \date    20.1.2017
*******************************************************************************************************************/



/******************************************************************************************************************/
// Include SoftwareSerial library
#include <SoftwareSerial.h>

/******************************************************************************************************************/
// function declaration
void lazy_loop_back_mode();
void loop_back_mode();
void bidirectional_mode();

/******************************************************************************************************************/
// constants
#define LOOPBACK      0
#define BIDIRECTIONAL 1
#define LAZY_LOOPBACK 2

#define WIFI_TERMINATION_START '['
#define WIFI_TERMINATION_END   ']'


/******************************************************************************************************************/
// globals
SoftwareSerial mySerial(10, 11); // // Create SoftwareSerial object (RX, TX)
byte g_mode_of_operation = BIDIRECTIONAL;


void setup()
{
  // Set data rate for HW Serial port on Arduino board Pin 0 and Pin 1
  Serial.begin(9600);

  while (!Serial)
  {
    ; // wait for serial port to connect.
  }

  // Set data rate for the SW SoftwareSerial port
  mySerial.begin(9600);

  g_mode_of_operation = BIDIRECTIONAL;//LOOPBACK; //LAZY_LOOPBACK; // ; // set mode of operation
}

void loop() 
{
  switch( g_mode_of_operation )
  {
    case LAZY_LOOPBACK:
      lazy_loop_back_mode();    // sends a received character back immediately with carriage return
      break;
    case LOOPBACK:              // send received message back immediately
      loop_back_mode();
    break;
    case BIDIRECTIONAL:         // allow bidirectional, user controlled message flow
    default:
      bidirectional_mode();
    break;
  }

}

/****************************************************************************************************************//*
   \brief receive character from Wifly module and send it right back
   \author Alexander
   \date Jan 19, 2017
   // Problems are:
   // 1. only characters, not packages/words are send back (-> check for \r\n)
   // 2. 255 value (weird y value in ASCII table) shows up when more than 1 char is send from Wifly module

/******************************************************************************************************************/
void lazy_loop_back_mode()
{
  char char_received = -1;
  if( mySerial.available() > 0 ) 
  {
    // read from Sw serial (Wifly) and print to HW Serial (Arduino serial monitor)
    char_received = mySerial.read();
    Serial.println( char_received );

    // send received character right back where it came from..Sw Serial (Wifly)
    mySerial.println( char_received );
    
  }
  
}

/****************************************************************************************************************//*
   \brief receive message (as a string) from Wifly module and send it right back, needs termination values
   \details keep in mind it's a spike, need a function READ SERIAL BUFFER (interrupt driven)
   \author Alexander
   \date Jan 18, 2017
/******************************************************************************************************************/
void loop_back_mode()
{
  char string_to_send[100]  = "";    
  char received_char        = -1;
  bool full_msg_received    = false;
  int i                     = 0;
  

  while( mySerial.available() > 0 && full_msg_received != true)           // check for first character
  {
    received_char = mySerial.read();
    if( received_char == WIFI_TERMINATION_START )
    {
      delay( 1 );                                                         // IMPORTANT: delay here, needs to be adjusted by baudrate 
                                                                          // 9600 baud -> 9600 bytes/second -> 10 byte/ms=10kHz, 
                                                                          // operationg speed of ATmega 328 = 16kHz)
      string_to_send[ i++ ] = received_char;                              // store START char
      while( mySerial.available() > 0 && full_msg_received != true )      // loop through remaining message
      {
        received_char = mySerial.read();
        string_to_send[ i ] = received_char;
        if( received_char == WIFI_TERMINATION_END )                       // check for END char
        {
          mySerial.println( string_to_send );                             // send message back to sender
          full_msg_received = true;
          
          Serial.print( "Full Message received. Length: " );              // some debugging stuff
          Serial.print( i + 1 );
          Serial.print( " Message was: " );
          Serial.println( string_to_send );
          
        }
        delay( 1 );
        i++;
      }
    }
  }
}

/****************************************************************************************************************//*
   \brief receive message from Wifly module, display it in Arduino Serial Monitor, allows to send messages back 
          through Arduino Serial Monitor, works bidirectional
   \author Alexander
   \date Jan 18, 2017
/******************************************************************************************************************/
void bidirectional_mode()
{
  // read from SW Serial (Wifly Board) and write to HW Serial (Arduino Serial Monitor)
  if (mySerial.available()) 
  {
    Serial.write(mySerial.read());
  }

  // read from HW serial (Arduino Serial Monitor) and write to SW serial (Wifly Board)
  if (Serial.available()) 
  {
    mySerial.write(Serial.read());
  }
}

