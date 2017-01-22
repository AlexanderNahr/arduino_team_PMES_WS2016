#include "SimpleTimer.h"

                                        //TEST COMMENT 
int counter; 
boolean forthefirsttime = true;
boolean locked = false;
SimpleTimer timer;
int mysimpletimerid;
int numberoforders;
int myordertimerid;
int newordertimerid;
void setup()
{
 counter = 1;                  //counting failed connections
 Serial.begin(9600);
 
 //TIMER BLOCK 0
 mysimpletimerid= timer.setTimeout (8000,RetryTimeout);
 myordertimerid = timer.setInterval (15000,FactoryTerminatedOrder); 
 //END OF TIMER BLOCK 0
 
 numberoforders = random(5);
}

 
void loop()
{  
   timer.run();
   if (forthefirsttime == true)
   {
    timer.disable(mysimpletimerid);
   }
   
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
      forthefirsttime = false; 
      mysimpletimerid= timer.setTimeout (8000,RetryTimeout); 
      timer.enable(mysimpletimerid);
      timer.restartTimer(mysimpletimerid);       
          }//end if
     
     if (numberoforders==0)
     {
      numberoforders = -1; //to avoid entering this loop in the next cycle
      timer.disable(myordertimerid);      
      newordertimerid = timer.setTimeout (5000, NewOrderReceived);
      timer.enable(newordertimerid);
      timer.restartTimer(newordertimerid);
      
     }

}

//TIMER BLOCK 3
void RetryTimeout()
{
  locked = false;                             //global boolean variable, that will be used in the other functions to manage channel's status
  Serial.print("Channel is open again (timestamp (msec): ");
  Serial.print (millis());
  Serial.println(")"); 
  forthefirsttime  = true; 
  counter = 1;                
}
void FactoryTerminatedOrder()
{
  numberoforders = numberoforders -1;
  if (numberoforders ==0)
  {
    Serial.println("All orders terminated");
  }
  else
  {
     Serial.print("Factory ready mit one order. Remaining orders= ");
     Serial.println(numberoforders);
  }
}
 void NewOrderReceived()
 {
  numberoforders = random(5);
  Serial.print(numberoforders);
  Serial.println (" new orders received");
  timer.enable(myordertimerid);
  timer.restartTimer(myordertimerid);
 }

//END OF TIMER BLOCK 3
 
