//Includes
#include <Arduino.h>
#include "WiFiService.h"
#include <SoftwareSerial.h>

//DEBUGGING
#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.print(F("DEBUG: ")); Serial.println (x,HEX);
#else
#define DEBUG_PRINT(x)
#endif

//#define DEBUG_MODE
#ifdef DEBUG_MODE
#define DEBUG_MODE(x)  Serial.print(F("DEBUG_MODE: ")); Serial.println (x);
#else
#define DEBUG_MODE(x)
#endif

// Create SoftwareSerial object
SoftwareSerial mySerial(10, 11); // RX, TX

//Constructor
WiFiService::WiFiService()
{
  //Parameters:
	ptrSendtoSerialMonitor = SENDTOSWSERIAL;
	ptrStartChar[0] = STARTZEICHEN_1;
	ptrStartChar[1] = STARTZEICHEN_1;
	ptrStartChar[2] = STARTZEICHEN_2;
	ptrEndChar[0] = ENDZEICHEN_1;
	ptrEndChar[1] = ENDZEICHEN_1;
	ptrEndChar[2] = ENDZEICHEN_2;

  //Inital Conditions for State Transitions
  SawEndChar = false;
  SawStartChar = false;
  Approval = false;
  GoToPrepare = false;
  LoopSinceComplete = 0;

  //Inital Values of Variables
  WiFiMode = false;
  SerialChar = "";
  CurrentString = "";
  StartStopCharType = 0;
  StringCounter = 3;
  RxString[0].reserve(57);
  RxString[1].reserve(1);
  RxString[2].reserve(1);

}


void WiFiService::Init()
{
  mySerial.begin(9600);   // Set data rate for the SW serial port
  while (!mySerial) {;}   // wait for SW serial port to connect.

}


void WiFiService::Run(bool bo)
{
	//Set Approval to True
	WiFiMode = bo;
	if (WiFiMode) {Approval = WiFiMode;}


	//Mode "Idle"
	if (!Approval)
	{
		DEBUG_MODE("Idle")
		Idle();
	}

	//Mode "Wait_for_Start_Character"
	if (!SawStartChar && Approval)        
	{
		DEBUG_MODE("Wait for Start Character")  
		Wait_for_Start_Character();   
	}  

	//Mode "BuildString"
	if (SawStartChar)  
	{
		DEBUG_MODE("Build String") 
		BuildString();              
	} 

	if (GoToPrepare)
	{
		DEBUG_MODE("Prepare_for_next_String")
		Prepare_for_next_String();
	}
  
    //Mode "String Complete"
	if (SawEndChar)  
	{
		DEBUG_MODE("String Complete")
		StringComplete();
	} 
  if (mySerial.overflow()) 
  {
    Serial.println(F("SoftwareSerial overflow!")); 
  }
  HWtoSWSerial();
}

void WiFiService::Send(String str)
{

		mySerial.print(str);
}

//Modes
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void WiFiService::Idle()
{
	;
}

// Checks Byte by Byte the SW Serial Buffer for the Start Character. 
// When detected, StringInProgress = true and rxString = "["
void WiFiService::Wait_for_Start_Character()
{
		while (mySerial.available() && !SawStartChar)
		{
			SerialChar = mySerial.read();
			if (IsStartChar(SerialChar))
			{
				CurrentString = SerialChar;
				SawStartChar = true;
			}

			if (!IsStartChar(SerialChar))
			{
				if (ptrSendtoSerialMonitor)
				{
					Serial.write(SerialChar); // Serial.write(mySerial.read());
				}
			}		
		}

}


// Adds Bytes of the SW Serial Buffer to the global variable  rxString 
// until End Character is detected
void WiFiService::BuildString()
{
          while (mySerial.available() && !SawEndChar)
          {
			  SerialChar = (char)mySerial.read();

			  if (!SawEndChar)
			  {
					CurrentString += SerialChar;
			  }			  

			  if (IsEndChar(SerialChar))
			  {
				  SawEndChar = true;
			  }
          }
}

void WiFiService::StringComplete()
{
	int temp;
	String strtemp;
	
		if (StringCounter < 3)
		{
			StringCounter++;
		}
		else
		{
			StringCounter = 1;
		}

		strtemp = RxString[StringCounter - 1].length();
		temp = strtemp.toInt();
		RxString[StringCounter - 1].remove(0, temp);
		RxString[StringCounter - 1] += CurrentString;

		GoToPrepare = true;
		
}

void WiFiService::Prepare_for_next_String()
{
	int temp;
	String strtemp;

	strtemp = CurrentString.length();
	temp = strtemp.toInt();
	CurrentString.remove(0, temp);
	
	SawEndChar = false;
	SawStartChar = false;
	GoToPrepare = false;
	Approval = false;

	StartStopCharType = 0;
}


// HWtoSWSerial is always active 
void WiFiService::HWtoSWSerial()
{
    while (Serial.available())   
	{
		mySerial.write(Serial.read());
	} 

}

//Get Functions
//********************************************************************************************************

String WiFiService::Read()
{
	int tmp;
	tmp = StringCounter;

	if (StringCounter > 1)
	{
		StringCounter--;
	}
	else
	{
		StringCounter = 3;
	}

	return RxString[tmp - 1];
}

String WiFiService::GetString(int n)
{
	return RxString[n-1];
}

//Basic Check Functions
//********************************************************************************************************

bool WiFiService::String_Is_Complete()
{
  return SawEndChar;
}



bool WiFiService::IsStartChar(char c)
{
	for (int i = 0; i < 3; i++)
	{
		if (c == ptrStartChar[i])
		{
			StartStopCharType = i+1;
			return true;			
		}
	}
	return false;
}

bool WiFiService::IsEndChar(char c)
{
	for (int i = 0; i < 3; i++)
	{
		if (c == ptrEndChar[i] && StartStopCharType == i+1)
		{
			return true;
		}
	}
	return false;
}

//Debug Code delete before commit!
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
char WiFiService::Get_bool(bool Variable)
{
    if (Variable == true) 
    {
      return 'T';
    }
    else
    {
      return 'F';
    }
}

void WiFiService::Debug_ShowAll()
{

}

