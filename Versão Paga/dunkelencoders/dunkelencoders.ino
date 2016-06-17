#include <Arduino.h>
#include "Encoder.h"

#define SEND_TIME 1

const int encoderPort1 				= 2;
const int encoderPort2 				= 18;
const int encoderPort3 				= 19;
const int encoderPort4 				= 20;
const int encoderPort5 				= 21;

const String MAC					= "D1";

//Time handlers
unsigned long timeOnStart			= 0;
unsigned long timeToSend			= 0;
unsigned long lastTimeSended		= 0;
unsigned long currentTime			= 0;
unsigned long diffTime				= 0;
int loopCounter						= 0;

//Hendlers of encoders
unsigned long lastEncoder1Value		= 0;
unsigned long previousEncoder1Value	= 0;
unsigned long lastEncoder2Value		= 0;
unsigned long previousEncoder2Value	= 0;
unsigned long lastEncoder3Value		= 0;
unsigned long previousEncoder3Value	= 0;
unsigned long lastEncoder4Value		= 0;
unsigned long previousEncoder4Value	= 0;
unsigned long lastEncoder5Value		= 0;
unsigned long previousEncoder5Value	= 0;

char incomingFlag; 								//Flag received from cordinator
boolean canSend 					= false;	//Responsable for the working status	
unsigned int packageCounter 		= 0; 

Encoder encoder1(encoderPort1);
Encoder encoder2(encoderPort2);
Encoder encoder3(encoderPort3);
Encoder encoder4(encoderPort4);
Encoder encoder5(encoderPort5);

/*
*	setup()
*
*	Args:
*
*	Actions:
*
*		Initialize serial and setup encoder interruptions
*/
void setup()
{
	Serial.begin(9600);

    configureInterruptions();
}

/*
*	loop()
*
*	Args:
*
*	Actions:
*
*		Start only when has received a start command from cordinator
*		When it receives the start command from cordinator, the variables are reset
*		and it starts to send data;
*
*		If it receives the stop flag (2), it stops to send data.   
*/
void loop()
{
	verifyStartCommand(); 

	if (canSend)
	{

		if (myTimer())
		{

			sendPackage();
			lastTimeSended = timeToSend;

		}

	}

}

/*
*	configureInterruptions()
*
*	Args:
*
*	Actions:
*
*		Configure the interruptions
*/
void configureInterruptions()
{
    attachInterrupt(0, &counter1, FALLING);
    attachInterrupt(1, &counter2, FALLING);
    attachInterrupt(2, &counter3, FALLING);
    attachInterrupt(4, &counter4, FALLING);
    attachInterrupt(5, &counter5, FALLING);
}

/*
*	verifyStartCommand()
*
*	Args:
*
*	Actions:
*
*		If it receives a command from cordinator, it verifies the received flag, and then, 
*		if the flag is 1, the variables are reset
*		The canSend receives the incomingFlag value
*/
void verifyStartCommand()
{
  	if (Serial.available() > 0) 
	{
		incomingFlag = (char)Serial.read();

		if (incomingFlag == '1' || incomingFlag == '2' || incomingFlag == '3')
		{
			timeOnStart				= (millis()/1000);
			lastTimeSended			= 0;
			packageCounter			= 0;
			previousEncoder1Value	= 0;
			previousEncoder2Value	= 0;
			previousEncoder3Value	= 0;
			previousEncoder4Value	= 0;
			previousEncoder5Value	= 0;
			encoder1.initPulseCounter();
			encoder2.initPulseCounter();
			encoder3.initPulseCounter();
			encoder4.initPulseCounter();
			encoder5.initPulseCounter();

			Serial.println("MAC;CONT_PCT;TDP;ENC_RTE;ENC_RDD;ENC_RTD;ENC_RDE");
			delay(1000);

			canSend = true;
		}
		else if(incomingFlag == '0')
		{
			canSend = false;
		}
	}
}

/*
*	myTimer()
*
*	Args:
*
*	Actions:
*
*		Based on the millis() of the Arduino, it calculates the elapsed time since the start command received from cordinator
*/
bool myTimer()
{
	currentTime = (millis()/1000) - timeOnStart;
	diffTime = currentTime - lastTimeSended;

	if (diffTime >= SEND_TIME)
	{
		timeToSend = currentTime;
		return true;
	}

	return false;
	
}

/*
*	sendPackage(unsigned long myTimerValue)
*
*	Args:
*
*	Actions:
*
*		Cast data to hexadecimal and then send the package
*/
void sendPackage()
{

	String package;

	packageCounter ++;

	Serial.println(fillDataInPackage(package));

}

/*
*	fillDataInPackage(String package)
*
*	Args:
*
*		package:	receives the package to fill it with data
*
*	Actions:
*
*		Fills the received package with the encoders data and then return it
*/
String fillDataInPackage(String package)
{

	lastEncoder1Value = encoder1.getPulseCounter();
	int enc1 = (lastEncoder1Value - previousEncoder1Value);
	previousEncoder1Value = lastEncoder1Value;

	lastEncoder2Value = encoder2.getPulseCounter();
	int enc2 = (lastEncoder2Value - previousEncoder2Value);
	previousEncoder2Value = lastEncoder2Value;

	lastEncoder3Value = encoder3.getPulseCounter();
	int enc3 = (lastEncoder3Value - previousEncoder3Value);
	previousEncoder3Value = lastEncoder3Value;

	lastEncoder4Value = encoder4.getPulseCounter();
	int enc4 = (lastEncoder4Value - previousEncoder4Value);
	previousEncoder4Value = lastEncoder4Value;

	lastEncoder5Value = encoder5.getPulseCounter();
	int enc5 = (lastEncoder5Value - previousEncoder5Value);
	previousEncoder5Value = lastEncoder5Value;

	package.concat(MAC);
	package.concat(";");
	package.concat(packageCounter);
	package.concat(";");
	package.concat(enc1);
	package.concat(";");
	package.concat(enc2);
	package.concat(";");
	package.concat(enc3);
	package.concat(";");
	package.concat(enc4);
	package.concat(";");
	package.concat(enc5);

	return package;

}

/*===	Encoders interrupt handlers ===========================================*/

/*
*	counter1()
*
*	Args:
*
*	Actions:
*
*		Invoque the increasePulseCounter method from encoder 1
*/
void counter1()
{
 	encoder1.increasePulseCounter(); 
}

/*
*	counter2()
*
*	Args:
*
*	Actions:
*
*		Invoque the increasePulseCounter method from encoder 2
*/
void counter2()
{
 	encoder2.increasePulseCounter(); 
}

/*
*	counter3()
*
*	Args:
*
*	Actions:
*
*		Invoque the increasePulseCounter method from encoder 3
*/
void counter3()
{
 	encoder3.increasePulseCounter(); 
}

/*
*	counter4()
*
*	Args:
*
*	Actions:
*
*		Invoque the increasePulseCounter method from encoder 4
*/
void counter4()
{
 	encoder4.increasePulseCounter(); 
}

/*
*	counter5()
*
*	Args:
*
*	Actions:
*
*		Invoque the increasePulseCounter method from encoder 5
*/
void counter5()
{
 	encoder5.increasePulseCounter(); 
}

/*============================================================================*/
