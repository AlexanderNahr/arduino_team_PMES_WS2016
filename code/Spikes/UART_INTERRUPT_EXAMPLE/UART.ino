// Create SoftwareSerial object
SoftwareSerial SW_Serial(10, 11); // RX, TX

// set up glabal data 
String rxString = "";

// init flags
boolean rxStringCompleteFlag = false;
boolean rxStringOverflowFlag = false;

void UartInit(void)
{
  // Set data rate for hw serial port on Arduino board Pin 0 and Pin 1
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect.
  }
  
  // Set data rate for the sw serial port
  SW_Serial.begin(9600);

  while (!SW_Serial) {
    ; // wait for sw serial port to connect.
  }

  rxString.reserve(200);
}

/**
 * @brief rx-isr of hw uart
 */
void serialEvent()
{
  while (Serial.available()) 
  {
    // catch the incoming char
    char inChar = (char)Serial.read();
    
    // add it to the inputString:
    rxString += inChar;

    if (rxString.length() >= 200)
    {
      rxString = "";
      rxStringOverflowFlag = true;
    }
    
    // set flag if incoming char is the termination sign "]".
    // this signals to the main loop to do somthing with the data
    if (inChar == ']')
    {
      rxStringCompleteFlag = true;
    }
  }
}

