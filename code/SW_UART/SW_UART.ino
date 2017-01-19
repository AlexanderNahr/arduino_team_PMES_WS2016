/*
  Code based on SoftwareSerialExample (see Arduino->Datei->Beispiele->SoftwareSerial)

  The circuit:
   RX is digital pin 10 (connect to TX of other device)
   TX is digital pin 11 (connect to RX of other device)

*/


// Include SoftwareSerial library
#include <SoftwareSerial.h>

// Create SoftwareSerial object
SoftwareSerial mySerial(10, 11); // RX, TX
char loopback_char = 'A';

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
}

void loop() 
{
  
  // read from SW Serial and write to HW Serial
  if (mySerial.available()) 
  {
    Serial.write(Serial.read()); // sends to serial monitor
  }

  // read from HW serial and write to SW serial
  if (Serial.available()) 
  {
    mySerial.write(Serial.read());
  }
}

