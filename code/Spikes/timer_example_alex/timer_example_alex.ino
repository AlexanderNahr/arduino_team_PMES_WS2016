/********************************************NOTE********************************************************************
 * Statt Pin 13 habe ich pin A2 zur Tonausgabe verwendet.
********************************************************************************************************************/

/****************************************************************************************************************//**
 * \file displayTest_Aufgabe1.8
 * \brief Uebung 1, Aufgabe 8
 * \author Alexander Nahrwold
 * \date 2.11.2016
 /****************************************************************************************************************/
/*
 * Plan:
 * - 1 second interrupt for traffic light
 * - ADC done interrupt
 * - main loop will 
 * -- count seconds and switch lights on/off
 * -- check ADC done flage periodically and store value (not necessarily required)
 * -- after both checks are done display will be printed with local, updated values
 * 
 * Source:
 * - ADC Interrupt: http://www.glennsweeney.com/tutorials/interrupt-driven-analog-conversion-with-an-atmega328p
 * - Arduino timed interrupts: http://playground.arduino.cc/Code/Timer1
 * - Timed Interrupts: http://www.instructables.com/id/Arduino-Timer-Interrupts/step2/Structuring-Timer-Interrupts/
/****************************************************************************************************************/
// Include Files
#include <LiquidCrystal.h>  //!< include LCD functions:
#include "pitches.h"        //!< for 



/****************************************************************************************************************/
// Global Variables

int RS_pin =    3;  //!< LCD RS pin connects to pin3
int E_pin  =    4;  //!< LCD E pin connects to pin4
int DB4_pin =   6;  //!< LCD DB4 pin connects to pin6
int DB5_pin =   7;  //!< LCD DB5 pin connects to pin7
int DB6_pin =   8;  //!< LCD DB6 pin connects to pin8
int DB7_pin =   9;  //!< LCD DB7 pin connects to pin9




LiquidCrystal lcd( RS_pin, E_pin, DB4_pin, DB5_pin, DB6_pin, DB7_pin);  //!< define the LCD screen


int analogValue   = -1;   //!< global variable, stores the result from analog pin

int g_ledPin1     = 10;              //!< red
int g_ledPin2     = 11;              //!< yellow
int g_ledPin3     = 12;              //!< green
int g_speakerPin  = 13;              //!< pin associated with the speaker
int toggle1       =  0;              //!< interrupt toggler
int g_traffic_light_counter = 0;     //!< counter for ISR to control traffic light

/****************************************************************************************************************/
// function declaration
void SwitchTrafficLight( int light_color );
int ButtonCheck( int adc_count, char* button_clicked_buffer, int buffer_size );
void SetupTimer1( void );


/****************************************************************************************************************/
// constants
#define RED_LIGHT     0
#define YELLOW_LIGHT  1
#define GREEN_LIGHT   2

#define RED_LIGHT_TIME      0  //!< start time red light
#define YELLOW_LIGHT_1_TIME 5  //!< start time yellow light
#define GREEN_LIGHT_TIME    8  //!< start time green light
#define YELLOW_LIGHT_2_TIME 9  //!< start time yello light
#define MAX_TIME            9  //!< overflow time


#define RED_FREQUENCY      100  //!< frequency for speaker in Hz
#define GREEN_FREQUENCY    400  //!< frequency for speaker in Hz

#define ONE_SECOND    1000
#define THREE_SECONDS 3*ONE_SECOND
#define FIVE_SECONDS  5*ONE_SECOND

// references ADC counts for button clicks
#define BUTTON_1              1  //!< ADC count button 1/S1 click
#define BUTTON_2            242  //!< ADC count button 2/S2 click
#define BUTTON_3            473  //!< ADC count button 3/S3 click
#define BUTTON_4            679  //!< ADC count button 4/S4 click
#define BUTTON_5            831  //!< ADC count button 5/S5 click
#define NO_BUTTONS         1023  //!< ADC count no buttons clicked
#define BTN_ADC_TOLERANCE    50  //!< tolerance for ADC counts for button click

#define FAILURE              -1
#define SUCCESS               0

#define DISPLAY_LINE_1        0  //!< display line 1 
#define DISPLAY_LINE_2        1  //!< display line 2 
#define DISPLAY_LINE_3        2  //!< display line 3 
#define DISPLAY_LINE_4        3  //!< display line 4 

#define DISPLAY_BEGIN         0  //!< display begin 
#define DISPLAY_END          19  //!< display end 


/****************************************************************************************************************//*
 * \brief setup function
 /****************************************************************************************************************/
void setup()
{
  // LCD has 4 lines with 20 chars
  lcd.begin(20, 4); 
  lcd.print("system ready");

  // set ledPin as output
  pinMode( g_ledPin1,    OUTPUT );
  pinMode( g_ledPin2,    OUTPUT );
  pinMode( g_ledPin3,    OUTPUT );
  //pinMode( g_speakerPin, OUTPUT );
  
  SwitchTrafficLight( RED_LIGHT );
  tone( A2, RED_FREQUENCY );

  g_traffic_light_counter = 0;
  
  SetupTimer1( );                   // TIMER_EXAMPLE - setup timer here
  
}

// Interrupts
/****************************************************************************************************************//**
 * \brief interupt service routine for timer 1 - TIMER_EXAMPLE
 *        attach timer 1 comparision to interrupt vector table 
 *        -> THIS WILL BE EXECUTED everytime timer1 interrupt is triggered
 * \source http://www.instructables.com/id/Arduino-Timer-Interrupts/step2/Structuring-Timer-Interrupts/
/****************************************************************************************************************/
ISR( TIMER1_COMPA_vect )
{
  g_traffic_light_counter++;
  if( g_traffic_light_counter > MAX_TIME )
  {
    g_traffic_light_counter = 0;
  }
}

/****************************************************************************************************************//*
 * \brief main loop
 /****************************************************************************************************************/
void loop()
{
  delay( 100 );
  
  static char button_print[ 100 ]  = "No Button";
  static char light_print[ 100 ]   = "red";
  static int current_light         = -1;  //!< identifies the current light
   
  // implement your code here:
  lcd.clear( );

  // DISPLAY LINE 1 ///////////////////////////////////////////////////////
  // read and print analog button value
  lcd.print( "Analog 0: " );
  analogValue = analogRead( A0 );
  lcd.print( analogValue );

  // DISPLAY LINE 2 ///////////////////////////////////////////////////////
  // determine which button is clicked
  int clicked_button = ButtonCheck( analogValue, 
                                    button_print, 
                                    sizeof( button_print ) ); 

  // when button 1 is pressed, leave RED_LIGHT phase
  // REMEMBER: IF BUTTONS ARE NOT CONNECTED, IT WILL LOOK LIKE BUTTON 1
  //           IS ACTIVE
  if(    clicked_button == BUTTON_1 
      && current_light  == RED_LIGHT)
  {
    g_traffic_light_counter = YELLOW_LIGHT_1_TIME;
  }

  // print results
  lcd.setCursor( DISPLAY_BEGIN, DISPLAY_LINE_2 );
  lcd.print( button_print );

  // DISPLAY LINE 3 ///////////////////////////////////////////////////////
  switch( g_traffic_light_counter )  // TIMER_EXAMPLE
  {
    case RED_LIGHT_TIME:
      current_light = RED_LIGHT;
      SwitchTrafficLight( current_light );
      strcpy( light_print, "red");
      tone( A2, RED_FREQUENCY );
      break;
    case YELLOW_LIGHT_1_TIME:
      noTone( A2 );
      current_light = YELLOW_LIGHT;
      SwitchTrafficLight( current_light );
      strcpy( light_print, "yellow");
      break;
    case GREEN_LIGHT_TIME:
      current_light = GREEN_LIGHT;
      SwitchTrafficLight( current_light );
      strcpy( light_print, "green");
      tone( A2, GREEN_FREQUENCY);
      break;
    case YELLOW_LIGHT_2_TIME:
      noTone( A2 );
      current_light = YELLOW_LIGHT;
      SwitchTrafficLight( current_light );
      strcpy( light_print, "yellow");
      break;
    default:
      break;
  }    
  
  lcd.setCursor( DISPLAY_BEGIN, DISPLAY_LINE_3 );
  lcd.print( light_print );

  // wait for 100 ms (reduces display flickering)
  delay( 100 );
}

/****************************************************************************************************************//**
 * \brief switches traffic light
 * \param light_color color of light you want to switch
 /****************************************************************************************************************/
void SwitchTrafficLight( int light_color )
{
  switch( light_color )
  {
    case RED_LIGHT:
      digitalWrite(g_ledPin1, HIGH);
      digitalWrite(g_ledPin2, LOW);
      digitalWrite(g_ledPin3, LOW);  
    break;
    case YELLOW_LIGHT:
      digitalWrite(g_ledPin1, LOW);
      digitalWrite(g_ledPin2, HIGH);
      digitalWrite(g_ledPin3, LOW);  
    break;
    case GREEN_LIGHT:
      digitalWrite(g_ledPin1, LOW);
      digitalWrite(g_ledPin2, LOW);
      digitalWrite(g_ledPin3, HIGH);  
    break;
    default:
    break;
  }  
}

/****************************************************************************************************************//*
 * \brief checks which button is clicked on S-Trike experimentation board
 * \param adc_count ADC value returned by analog read function
 * \param button_clicked output: char array with string description of button
 * \return return the reference value for button click
/****************************************************************************************************************/
int ButtonCheck( int adc_count, char* button_clicked_buffer, int buffer_size )
{
  int return_value = FAILURE;

  #define MIN_STRING_SIZE 100

  strncpy( button_clicked_buffer, "Not Set.", buffer_size );

  if( buffer_size >= MIN_STRING_SIZE )
  { 
    if( adc_count >= 0 )
    {
      /*
       * assume adc_count=250
       * adc_count_plus = 300
       * adc_count_mius = 200
       * BUTTON_2=242
      */
      int adc_count_plus = adc_count + BTN_ADC_TOLERANCE;
      int adc_count_minus = adc_count - BTN_ADC_TOLERANCE;

      if( adc_count_plus > BUTTON_1 && adc_count_minus < BUTTON_1)
      {
        strncpy( button_clicked_buffer, "Button: 1", buffer_size );
        return_value = BUTTON_1;
      }
      else if( adc_count_plus > BUTTON_2 && adc_count_minus < BUTTON_2)
      {
        strncpy( button_clicked_buffer, "Button: 2", buffer_size );
        return_value = BUTTON_2;      
      }
      else if( adc_count_plus > BUTTON_3 && adc_count_minus < BUTTON_3)
      {
        strncpy( button_clicked_buffer, "Button: 3", buffer_size );
        return_value = BUTTON_3;
      }
      else if( adc_count_plus > BUTTON_4 && adc_count_minus < BUTTON_4)
      {
        strncpy( button_clicked_buffer, "Button: 4", buffer_size );
        return_value = BUTTON_4;
      }
      else if( adc_count_plus > BUTTON_5 && adc_count_minus < BUTTON_5)
      {
        strncpy( button_clicked_buffer, "Button: 5", buffer_size);
        return_value = BUTTON_5;
      }
      else
      {
        strncpy( button_clicked_buffer, "No Button.", buffer_size );
        return_value = NO_BUTTONS;
      }
    }
  }
  return( return_value );
}

/****************************************************************************************************************//**
 * \brief setup timer 1 for Compare Match (CTC) with 1 sec resolution  - TIMER_EXAMPLE
 * \details modified from \source to accomodate 256 prescaler and 8MHz clk
 * \source http://www.instructables.com/id/Arduino-Timer-Interrupts/step2/Structuring-Timer-Interrupts/
/****************************************************************************************************************/
void SetupTimer1( void )
{
  
  cli();  // stop interrupts

  // set timer1 interrupt at 1Hz
  TCCR1A = 0;  // set entire TCCR1A register to 0
  TCCR1B = 0;  // same for TCCR1B
  TCNT1  = 0;  // initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 31249;  // = (8*10^6) / (1*256) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);

  // Set CS10 and CS12 bits for 256 prescaler
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();  // allow interrupts
}

