#include "TimerOne.h"
 
int counter; 
int k;
boolean forthefirsttime = true;
boolean locked = false;

void setup()
{
  counter = 1;                  //counting failed connections
  Serial.begin(9600);
  //TIMER BLOCK 0 (init)
  Timer1.initialize(8000000);  
  Timer1.attachInterrupt(doSomething);  
  //END OF TIMER BLOCK 0
}

 
void loop()
{
  //TIMER BLOCK 1 
   if (forthefirsttime == true)
   {
    Timer1.stop();                //stop timer !!!                  
   }
   //END OF TIMER BLOCK1

   //YOUR PROGRAMM HERE:
    delay(1000);
    Serial.print("Actual time is ");
    Serial.print(millis());
    Serial.print("[msec] (#failed connections= ");
    Serial.print(counter);
    Serial.println(").");
    counter=counter+1;
    
    if ((counter>10)&&(forthefirsttime==true))                      
    {
      Serial.print("Too many failed connections. Channel will now be locked for 8 seconds (timestamp (msec): ");
      Serial.print (millis());
      Serial.println(")");
      locked = true;                           //global boolean variable, that will be used in the other functions to manage channel's status
      
      //TIMER BLOCK 2
      Timer1.attachInterrupt(doSomething);     //start counting down (8seconds); actually restart operating Timer1
      //END OF TIMER BLOCK 2
      
      forthefirsttime = false;                //set boolean variable, which allows stopping the timer in the next loop cycle! otherwise the timer will be activated again in 8 seconds
    }//end if

}

//TIMER BLOCK 3
void doSomething()
{
  locked = false;                             //global boolean variable, that will be used in the other functions to manage channel's status
  Serial.print("Channel is open again (timestamp (msec): ");
  Serial.print (millis());
  Serial.println(")");
  counter = 1;                                    //restart the whole procedure
  forthefirsttime = true;                          
}
//END OF TIMER BLOCK 3
 
