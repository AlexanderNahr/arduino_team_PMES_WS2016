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
void loop_back_mode();
void bidirectional_mode();

/******************************************************************************************************************/
// constants
#define LOOPBACK      0
#define BIDIRECTIONAL 1

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

  g_mode_of_operation = LOOPBACK; // BIDIRECTIONAL; // set mode of operation
}

void loop() 
{
  switch( g_mode_of_operation )
  {
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
   \brief receive message from Wifly module and send it right back
   \author Alexander
   \date Jan 18, 2017
/******************************************************************************************************************/
void loop_back_mode()
{
  char char_to_send_back = 'A';
  char string_to_send[ 100 ] = "MyAnswer";
  int iRead[ 10 ];

  for (int i=0; i<10; i++)
  {
      iRead[i] = Serial.read();
  }
  
  // read from SW Serial (Wifly Board) and write to HW Serial (Arduino Serial Monitor)
  if (mySerial.available()) 
  {
    char_to_send_back = Serial.write(mySerial.read());
  }
  
  // read from HW serial (Arduino Serial Monitor) and write to SW serial (Wifly Board)
  if (iRead[0] != -1) //--> PROBLEM HERE, need to flush queue here
  {
    // debug code for Serial Monitor
    Serial.print( "Following String will be send to Wifly Modul" );
    Serial.println( string_to_send );
    
        for (int i=0; i<10 ; i++)
          {
              Serial.print("Arrayplatz ");
              Serial.print(i);
              Serial.print(": ");
              Serial.println(iRead[i]);
          }
    
    mySerial.write( string_to_send );  // send this to Wifly modul, should appear in Telnet Com Window
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

