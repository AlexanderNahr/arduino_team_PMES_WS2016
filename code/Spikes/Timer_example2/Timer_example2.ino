#include "SimpleTimer.h"

                                        
int counter;                                        //it counts the failed connections to the uC
boolean DoDisable_mysimpletimer = true;             //boolean variable - if it is true, the Mysimpletimer will be disabled for the actual running cycle
boolean DoDisable_neworderstimer = true;            //boolean variable - if it is true, the Neworderstimer will be disabled for the actual running cycle                  
SimpleTimer timer;                                  
int numberoforders;                                 //# of incoming orders
int mysimpletimerid;                                //timer ID                              
int myordertimerid;                                 //timer ID
int neworderstimerid;                               //timer ID

void setup()
{
 counter = 1;                                       //counting failed connections
 Serial.begin(9600);
 
 //TIMER BLOCK 0
 mysimpletimerid= timer.setInterval (8000,RetryTimeout);                //8 seconds timeout for ARDUINO_ACK_RETRY_TIMEOUT 
 myordertimerid = timer.setInterval (15000,FactoryTerminatedOrder);     //15 seconds for each order to be completed by the factory (in reality 3 minutes) 
 neworderstimerid = timer.setInterval (5000, NewOrderReceived);         //after the factory executed all orders, 5 seconds later new orders "are received"
 //END OF TIMER BLOCK 0
 
 numberoforders = 2;                                                    //suppose that there are already some orders registered (starting state)
}

 
void loop()
{  
  //TIMER BLOCK 1 
   timer.run();                                                       //start all timers
   if (DoDisable_mysimpletimer == true)                               //stop the mysimpletimer immediately; no need for it right now; it will be started only if there are
   {                                                                  //at least 10 failed connection trials (final number of allowed failed connections not decided yet)
    timer.disable(mysimpletimerid);
   }
   if (DoDisable_neworderstimer ==true)                               //stop the neworderstimer immediately; no need for it right now; it will be started only if the
   {                                                                  //factory is ready with all the orders
    timer.disable(neworderstimerid);
   }
  //END OF TIMER BLOCK 1
  
   //YOUR PROGRAMM HERE:
    delay(1000);                                                     //count up in a one-second-tick
    Serial.print("Actual time is ");
    Serial.print(millis());
    Serial.print("[msec] (#failed connections= ");
    Serial.print(counter);
    Serial.println(").");
    counter=counter+1;                                              //count up (as if it would be registered a failed connection every second)

    
    if ((counter>10)&&(DoDisable_mysimpletimer==true))              //if threshold is reached for the first time                         
    {
      Serial.print("Too many failed connections. Channel will now be locked for 8 seconds (timestamp (msec): ");
      Serial.print (millis());
      Serial.println(")");                         

     //TIMER BLOCK 2
      timer.enable(mysimpletimerid);                              //enable the formerly disabled timer    
      timer.restartTimer(mysimpletimerid);                        //restart timer (reference time is NOW)    
      DoDisable_mysimpletimer = false;                            //don't let the program disable the timer in the next running cycle (with the if statement at the code's beginning)
     //END OF TIMER BLOCK 2
     
     }//end if
     
     if (numberoforders==0)                                       //if no more orders left in the factory queue
     {
      numberoforders = -1;                                        //avoid entering this loop in the next cycle

      //TIMER BLOCK 2
      timer.disable(myordertimerid);                              //stop the interval timer       
      timer.enable(neworderstimerid);                             //enable the neworderstimer
      timer.restartTimer(neworderstimerid);                       //restart the neworderstimer (reference time is NOW); this timer will "generate" in 5 seconds some new orders
      DoDisable_neworderstimer = false;                           //don't let the program disable the timer in the next running cycle (with the if statement at the code's beginning)
      //END OF TIMER BLOCK 2
     }

}

//TIMER BLOCK 3 - interrupt routines
void RetryTimeout()
{
  Serial.print("Channel is open again (timestamp (msec): ");
  Serial.print (millis());
  Serial.println(")"); 
  counter = 1;                                                   //failed connections counter is reset 
  DoDisable_mysimpletimer  = true;                               //let the program disable the timer in the next running cycle (otherwise it would trigger again automatically in 8 seconds) 
                
}

void FactoryTerminatedOrder()                                   //factory executed one order
{
  numberoforders = numberoforders -1;                           //decrement number of orders
  if (numberoforders ==0)
  {
    Serial.print("All orders terminated (time: ");
    Serial.print(millis());
    Serial.println(")");
  }
  else
  {
     Serial.print("Factory ready with one order. Remaining orders= ");
     Serial.println(numberoforders);
  }
}
 void NewOrderReceived()                              //interrupt routine : new orders will be "generated" (as if the uC would receive a/some new order(s) )  
 {
  numberoforders = 2;                                 //new orders "received"
  Serial.print(numberoforders);
  Serial.print (" new orders received (time: ");
  Serial.print(millis());
  Serial.println(")");
  
  timer.enable(myordertimerid);                               //enable the myordertimer 
  timer.restartTimer(myordertimerid);                         //restart it  (reference time is NOW)
   
  DoDisable_neworderstimer = true;                            //let the program disable the timer in the next running cycle (otherwise it would trigger again automatically in 5 seconds) 
 }
 //END OF TIMER BLOCK 3

//END OF TIMER BLOCK 3
 
