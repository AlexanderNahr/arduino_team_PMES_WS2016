#ifndef WiFiService_h
#define WiFiService_h
#include <Arduino.h>

#define STARTZEICHEN_1 '['
#define STARTZEICHEN_2 '*'
#define ENDZEICHEN_1 ']'
#define ENDZEICHEN_2 '*'
#define SENDTOSWSERIAL true

class WiFiService
{
  public:
  //Constructor:
  WiFiService();

  //Public Functions
  void Init();
  void Run(bool bo);
  void Send(String str);
  void SendtoExternal(String str);
  bool String_Is_Complete();
  String GetString(byte n);
  String Read();

  // Debugging Functions
  void Debug_ShowAll();
  
	private: 
	//Members
	//****************************************************
	
	//Variables for State Transitions
	bool SawEndChar;
	bool SawStartChar;
	bool GoToPrepare;
	bool Approval;


	//passed Parameters
	bool WiFiMode;

	//Variables
	char SerialChar;
	String RxString[3];
	String CurrentString;
	int StartStopCharType;
	int StringCounter;
	int LoopSinceComplete;
	bool String_available;

	//Parameters (#define):
	bool ptrSendtoSerialMonitor;
	char ptrStartChar[3];
	char ptrEndChar[3];
  
	//Methods
	//****************************************************

	//Modes
	void Wait_for_Start_Character();
	void BuildString();
	void StringComplete();
	void Prepare_for_next_String();
	void Idle();

	//Internal
	void HWtoSWSerial();
	char Get_bool(bool Variable);
  
	//Checks
	bool IsStartChar(char c);
	bool IsEndChar(char c);
  
};

#endif
