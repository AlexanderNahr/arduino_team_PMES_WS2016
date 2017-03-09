//Includes
#include <Arduino.h>
#include "WiFiService.h"
#include <SoftwareSerial.h>

//#define DEBUG_MODE
#ifdef DEBUG_MODE
#define DEBUG_MODE(x)  Serial.print("DEBUG_MODE: "); Serial.println (x);
#else
#define DEBUG_MODE(x)
#endif

//String received_string;

// Create SoftwareSerial object
SoftwareSerial serialWiFi(10, 11); // RX, TX
SoftwareSerial serialExtern(8, 9); // RX, TX

//Constructor
WiFiService::WiFiService()
{
  //Parameters:/
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
}


void WiFiService::Init()
{
  serialWiFi.begin(9600);   // Set data rate for the SW serial port
  while (!serialWiFi) { ; }   // wait for SW serial port to connect.

  serialExtern.begin(9600);   // Set data rate for the SW serial port
  while (!serialExtern) { ; }   // wait for SW serial port to connect.

}


void WiFiService::Run(bool bo)
{
	serialWiFi.listen();
	//Set Approval to True
	WiFiMode = bo;
	if (WiFiMode) {Approval = WiFiMode;}


	//Mode "Idle"
	if (!Approval)
	{
		DEBUG_MODE("Idle")
		Idle();
	}

	if (GoToPrepare)
	{
		DEBUG_MODE("Prepare_for_next_String")
			Prepare_for_next_String();
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
  
    //Mode "String Complete"
	if (SawEndChar)  
	{
		DEBUG_MODE("String Complete")
		StringComplete();
	} 

  HWtoSWSerial();
}

void WiFiService::Send(String& str)
{

		serialWiFi.print(str);
}

void WiFiService::SendtoExternal(String& str)
{
	serialExtern.listen();
	serialExtern.print(str);
	serialWiFi.listen();
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
		while (serialWiFi.available() && !SawStartChar)
		{
			SerialChar = serialWiFi.read();
			if (IsStartChar(SerialChar))
			{
				CurrentString = SerialChar;
				SawStartChar = true;
			}

			if (!IsStartChar(SerialChar))
			{
				if (ptrSendtoSerialMonitor)
				{
					Serial.write(SerialChar); // Serial.write(serialWiFi.read());
				}
			}		
		}

}


// Adds Bytes of the SW Serial Buffer to the global variable  rxString 
// until End Character is detected
void WiFiService::BuildString()
{
          while (serialWiFi.available() && !SawEndChar)
          {
			  SerialChar = (char)serialWiFi.read();

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

		strtemp = received_string.length();
		temp = strtemp.toInt();
		received_string.remove(0, temp);
		received_string += CurrentString;
	
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
		serialWiFi.write(Serial.read());
	} 

}


//Basic Check Functions
//********************************************************************************************************

bool WiFiService::String_Is_Complete()
{
  return SawEndChar;
}



bool WiFiService::IsStartChar(char c)
{
	for (byte i = 0; i < 3; i++)
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
	for (byte i = 0; i < 3; i++)
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
  char tempchar;
  
  Serial.write("SawEndChar: ");
  tempchar = Get_bool(SawEndChar);
  Serial.write(tempchar);
  Serial.println("");

  Serial.write("SawStartChar: ");
  tempchar = Get_bool(SawStartChar);
  Serial.write(tempchar);
  Serial.println("");

  Serial.write("ptrSendtoSerialMonitor: ");
  tempchar = Get_bool(ptrSendtoSerialMonitor);
  Serial.write(tempchar);
  Serial.println("");

  Serial.write("ptrStartChar[1]: ");
  Serial.write(ptrStartChar[1]);
  Serial.println("");

  Serial.write("ptrStartChar[2]: ");
  Serial.write(ptrStartChar[2]);
  Serial.println("");
  
  Serial.write("ptrEndChar[1]: ");
  Serial.write(ptrEndChar[1]);
  Serial.println("");

  Serial.write("ptrEndChar[2]: ");
  Serial.write(ptrEndChar[2]);
  Serial.println("");

  Serial.write("CurrentString: ");
  Serial.print(CurrentString);
  Serial.println("");

}

