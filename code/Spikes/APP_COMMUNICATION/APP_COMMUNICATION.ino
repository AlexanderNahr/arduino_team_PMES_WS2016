/**
 * Mit dieser Minimalapplikation habe ich einen
 * stabilen Loopbackbetrieb mit dem App hinbekommen.
 * 
 * Allerdings nur im Hardwaremodus!
 * Man muss darauf achten, dass die Hardware UART Schnittstelle wärend der Programmierung
 * getrennt ist.
 * 
 * Das vorgehen zum verbinden mit dem App ist im folgenden Beschrieben.
 * 
 * 1. Smartphone mit WIFI-Modul verbinden
 * 2. App komplett neu öffnen.
 * 3. Beim Senden des ersten Strings wird standardmäßig die
 *    die comm-open und comm-remote message an das Smartphone geschickt.
 *    dannach ist das Programm in Loopackmodus
 *    
 * WARUM FUNKTIONIERT DER SW MODUS Nicht?
 */

// Include SoftwareSerial library
#include <SoftwareSerial.h>

// Create SoftwareSerial object
SoftwareSerial SWSerial(10, 11); // RX, TX

void setup()
{
  // Set data rate for hw serial port on Arduino board Pin 0 and Pin 1
  Serial.begin(9600);
  // wait for serial port to connect.
  while (!Serial) {;}

  SWSerial.begin(9600);
  // wait for serial port to connect.
  while (!SWSerial) {;}
}

void loop() 
{
  while (SWSerial.available()) 
  {
    SWSerial.write(SWSerial.read());
  }
}

/**
 * @brief rx-isr of hw uart
 */
void serialEvent() {

  while (Serial.available()) 
  {
    Serial.write(Serial.read());
  }
}


