/*
  Code based on SoftwareSerialExample (see Arduino->Datei->Beispiele->SoftwareSerial)

  The circuit:
   RX is digital pin 10 (connect to TX of other device)
   TX is digital pin 11 (connect to RX of other device)

*/


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

/******************************************************************************************************************/
// globals
SoftwareSerial mySerial(10, 11); // // Create SoftwareSerial object (RX, TX)
byte g_mode_of_operation = BIDIRECTIONAL;

/******************************************************************************************************************/
// function declaration (description of functions in defintion)


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

  g_mode_of_operation = LAZY_LOOPBACK; //LOOPBACK; // BIDIRECTIONAL; // set mode of operation
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
   \brief receive message (as a string) from Wifly module and send it right back
   \author Alexander
   \date Jan 18, 2017

   Well, this one isn't working as I planned it to. Seems to be in an eternal loop.
/******************************************************************************************************************/
void loop_back_mode()
{
  char char_to_send_back = 'A';
  char string_to_send[100] = "";
  char char_received = -1;
  

  // read from SW Serial (Wifly Board) and write to HW Serial (Arduino Serial Monitor)
  if( mySerial.available() > 0 ) 
  {
    int i = 0;
    // read from Sw serial (Wifly) and print to HW Serial (Arduino serial monitor)
    char_received = mySerial.read();

    // loop through received string until no character in queue or '\0' reached
    while( char_received != -1 && char_received != '\0' )
    {
      
      Serial.println( char_received );            // print to Arduino Serial Monitor
  
      char_received = mySerial.read();            // read 1 char from Wifly queue
      string_to_send[i] = char_received;          // add to string
      if( char_received = '\0' )                  // end of received string reached
      {
        mySerial.println( string_to_send );       // send whole string back to sender (Wifly module)
      }
      
      i++;                                        // increment counter for string
    }
  }

  // read from HW serial (Arduino Serial Monitor) and write to SW serial (Wifly Board)
  /*if (Serial.available() > 0) //--> PROBLEM HERE, need to flush queue here
  {
    // debug code for Serial Monitor
    Serial.print( "Following String will be send to Wifly Modul" );
    //Serial.print( );//string_to_send );
    Serial.println();
    int read = Serial.read();
    mySerial.print( string_to_send );
    //mySerial.print( "print" );
    //mySerial.println(string_to_send); // is NOT send
    mySerial.println();// "println");     // is send twice
    //mySerial.write( "write\n\r" );  // send this to Wifly modul, should appear in Telnet Com Window
  }*/
  //delay(100); // delay hat nichts gebracht..
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

