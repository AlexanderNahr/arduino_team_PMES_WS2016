/****************************************************************************************************************//**
   \file    CodeStyleGuide.c
   \brief   Example code for common code style for Arduino team
   \details detailed description
            more description
   \author  Alexander Nahrwold
   \date    29.11.2016

  Anmerkung:
  Die hier verwendeten Namenskonventionen sind Beispiele wie ich sie verwenden wuerde. Das wichtigste fuer mich ist,
  dass der Code lesbar ist. Wenn das bedeutet ein paar Kommentarzeilen hinzuzuschreiben, ok. Wenn es bedeutet einen
  Variablen namen besonders lang zu machen, auch gut, oder alle Zuweisung auf einen

  Dokumentation:
  Die Attribute \file, \author, \details etc. sind fuer dozygen.
  Doxygen info: http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html
    Damit Doxygen diese Attribute erkennt muss ein /** am anfang jedes Dokumentationsblocks stehen. die langen
  /***..**//** Zeilen sind also notwendig.
  Wollt ihr eine einzelne variable mit Doxygen dokumentieren, geht dies mit //!<
  Nicht alles muss mit doxygen dokumentiert werden. Generell sollten wichtige Informationen zur Schnittstelle
  mit Doxygen beschrieben werden, die eigentliche Implementierung wird dann ueber normale Kommentare gemacht.


  Indentation:
  Die Arduino Umgebung bietet "Auto Format" (unter Tools -> Auto Format, oder ueber Ctrl + T ). Das vereinheitlcht
  die Einrueckungen, Leertaste in if statements/while loops etc. Das sollte helfen das Bild zu veranschulichen

  Funktionen:
  Funktionen sollten nicht zu lang werden, da diese sonst schwieriger zu debuggen sind (50 Zeilen max?)
  Versuche alles ueber Parameter zu uebergeben.
  Bilde structs fuer komplexe, zusammenhaengende Variablen

  Globale variablen:
  Natuerlich gilt es diese zu vermeiden. dies ist oft nicht moeglich. Also macht es ersichtlich, wenn ihr eine
  globale Variable benutzt. Auch ist auf race conditions zu achten.

  Interrupts:
  Besonders bei low level programmierung helfen Kommentare, da variablen namen oft vorgegeben sind. Lieber ein paar
  Zeilen mehr schreiben.

  mehr info zum Style: https://sites.google.com/a/cabrillo.edu/cs-11m/howtos/cppdoc
                       https://www.arduino.cc/en/Reference/StyleGuide
*******************************************************************************************************************/


/******************************************************************************************************************/
// include files
#include "ExampleFileName.h"      //!< explanation here

/******************************************************************************************************************/
// function declaration (description of function in defintion)
void     exampleFunctionName(   boolean example_parameter );  // function names: mixed case = first letter lowercase
uint16_t anotherExampleFuction( uint8_t another_example   );

/******************************************************************************************************************/
// constants
#define FAILURE              -1   //!< define in all caps
#define SUCCESS               0   //!< constant description
#define DURATION_ARRAY_SIZE   10  //!< array size of duration & frequency   

const byte g_RSpin  =         2;  //!< alternatively as a const varable, preceeded by 'g_' for global,
const byte g_Epin   =         3;  //!< LCD E pin connects to pin4
const byte g_DB4pin =         4;  //!< LCD DB4 pin connects to pin6

/******************************************************************************************************************/
// globals
volatile uint32_t g_timerCount   = 0;  //!< non constant global, precceded by 'g_' and then mixed case
volatile uint32_t g_secondCount  = 0;  //!< use specific size for variable, not just 'int'

uint32_t g_exampleArray[ DURATION_ARRAY_SIZE ]   =  { 501, 3 };  //!< duration for tone/LED in seconds
uint32_t g_exampleArray[ DURATION_ARRAY_SIZE ]   =  { 101, 3 };  //!< frequency for tone in Hz

/****************************************************************************************************************//*
   \brief     perform setup here
  /****************************************************************************************************************/
void setup()
{
  // pin direction
  pinMode( 13, OUTPUT );
  pinMode( 12, OUTPUT );

  setupTimer1(); // 1Hz interrupt
  setupTimer2(); // 100Hz interrupt
}

/****************************************************************************************************************//*
   \brief     this is the infinite while loop
  /****************************************************************************************************************/
void loop()
{
  uint32_t time_arduino     = 0;  // internal variables lower case, seperated by _ underscore
  uint32_t time_asm         = 0;
  uint32_t time_low_c       = 0;
  uint8_t  pushed_button    = 0;
  uint32_t x                = 0;  // avoid cryptic names
  uint16_t dbg_var_bfr      = 0;  // avoid cryptic names

  switch ( pushed_button )
  {
    case BUTTON_1:                // description
      break;
    case BUTTON_2:                //
      break;
    case BUTTON_3:                //
      break;
    case BUTTON_4:                //
      break;
    case BUTTON_5:                //
      break;
    default:
      break;
  }
}

/****************************************************************************************************************//*
   \brief     example description
   \details   provide details if necessary
   \param[in] example_parameter this in an example paramter..
   \author    Alexander Nahrwold
   \date      14.12.2016
  /****************************************************************************************************************/
void exampleFunctionName( boolean example_parameter )
{
  while ( example_paramter );         // avoid this
  while ( example_parameter == true ) // better: explicit comparison
  {

  }
}
/****************************************************************************************************************//*
   \brief     example description
   \details   provide details if necessary
   \param[in] example_parameter this in an example paramter..
   \return    0 -> success, not 0 - failure
   \author    Alexander Nahrwold
   \date      14.12.2016
  /****************************************************************************************************************/
uint16_t anotherExampleFuction( uint8_t another_example )
{
  uint16_t return_value     = FAILURE;  // set default values to a failure mode
  uint8_t  internal_vaiable = 0;
  bool     another_on       = 0;

  for ( int i = 0; i < 324; i++ ) // avoid magic numbers e.g. 324
  {
    if ( i = 323 )
    {
      return_value = SUCCESS;
    }
  }
  return (return_value); // return at the end of the function, one exit point is preferred
}

/****************************************************************************************************************//*
   \brief   interrupt service routine for timer 2 compare match (see file toggling_timer.ino)
   \details sends tone in defined Hz to speaker on S-Trike
            Aufgabe 4
   \author    Alexander Nahrwold
   \date      14.12.2016
  /****************************************************************************************************************/
ISR( TIMER2_COMPA_vect )
{
  PINB |= (1 << 4);  // toggle pin 12 = PB4 (see DFRduino Pro Mini schematic), chapter 14
  // NOTE:  writing a logic one to a bit in the PINx Register, will result in a toggle in the corresponding bit
  //        in the Data Register
}



