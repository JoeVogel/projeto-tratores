#include <LiquidCrystal.h>
#include <Arduino.h>
#include "Encoder.h"

/*
*	dunkelencoders.ino
*
*	Version 1.0.0
*
*	Copyright (c) 2015 Centro Universitário Católica de Santa Catarina
*
*	This file is part of Lager Box.
*
*   Dunkel Box source code is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   Foobar is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
#define SEND_TIME 1

const int encoderPort1 				= 2;
const int encoderPort2 				= 3;
const int encoderPort3 				= 18;
const int encoderPort4 				= 19;
const int encoderPort5 				= 20;
const int encoderPort6 				= 21;

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
unsigned long lastEncoder6Value		= 0;
unsigned long previousEncoder6Value	= 0;

char incomingFlag; 								//Flag received from cordinator
boolean canSend 					= false;	//Responsable for the working status	
unsigned int packageCounter 		= 0; 

Encoder encoder1(encoderPort1);
Encoder encoder2(encoderPort2);
Encoder encoder3(encoderPort3);
Encoder encoder4(encoderPort4);
Encoder encoder5(encoderPort5);
Encoder encoder6(encoderPort6);

LiquidCrystal lcd(13,12,11,10,9,8); 


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

	writeInLCD("CONFIGURATING...", 3);

    configureInterruptions();

    writeInLCD("CONFIGURATION OK.", 3);
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

		writeInLCD("WORKING...", 3);

		if (myTimer())
		{

			sendPackage();
			lastTimeSended = timeToSend;

		}

	}
	else
	{
		writeInLCD("STAND BY.", 3);
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
    attachInterrupt(3, &counter4, FALLING);
    attachInterrupt(4, &counter5, FALLING);
    attachInterrupt(5, &counter6, FALLING);
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

		if (incomingFlag == '1')
		{
			timeOnStart				= (millis()/1000);
			lastTimeSended			= 0;
			packageCounter			= 0;
			previousEncoder1Value	= 0;
			previousEncoder2Value	= 0;
			previousEncoder3Value	= 0;
			previousEncoder4Value	= 0;
			previousEncoder5Value	= 0;
			previousEncoder6Value	= 0;
			encoder1.initPulseCounter();
			encoder2.initPulseCounter();
			encoder3.initPulseCounter();
			encoder4.initPulseCounter();
			encoder5.initPulseCounter();
			encoder6.initPulseCounter();

			canSend = true;
		}
		else
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

	writeInLCD("SENDING DATA.", 3);

	packageCounter ++;

	Serial.println(fillDataInPackage(package));

	writeInLCD("DATA SENT.", 3);

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
	int spinEnc1 = (lastEncoder1Value - previousEncoder1Value);
	previousEncoder1Value = lastEncoder1Value;

	lastEncoder2Value = encoder2.getPulseCounter();
	int spinEnc2 = (lastEncoder2Value - previousEncoder2Value);
	previousEncoder2Value = lastEncoder2Value;

	lastEncoder3Value = encoder3.getPulseCounter();
	int spinEnc3 = (lastEncoder3Value - previousEncoder3Value);
	previousEncoder3Value = lastEncoder3Value;

	lastEncoder4Value = encoder4.getPulseCounter();
	int spinEnc4 = (lastEncoder4Value - previousEncoder4Value);
	previousEncoder4Value = lastEncoder4Value;

	lastEncoder5Value = encoder5.getPulseCounter();
	int spinEnc5 = (lastEncoder5Value - previousEncoder5Value);
	previousEncoder5Value = lastEncoder5Value;

	lastEncoder6Value = encoder6.getPulseCounter();
	int spinEnc6 = (lastEncoder6Value - previousEncoder6Value);
	previousEncoder6Value = lastEncoder6Value;

	package.concat(MAC);
	package.concat(";");
	package.concat(String(timeToSend, HEX));
	package.concat(";");
	package.concat(String(packageCounter, HEX));
	package.concat(";");
	package.concat(String(spinEnc1, HEX));
	package.concat(";");
	package.concat(String(spinEnc2, HEX));
	package.concat(";");
	package.concat(String(spinEnc3, HEX));
	package.concat(";");
	package.concat(String(spinEnc4, HEX));
	package.concat(";");
	package.concat(String(spinEnc5, HEX));
	package.concat(";");
	package.concat(String(spinEnc6, HEX));

	return package;

}

/*
*	calculesSpin(unsigned long numberOfPulses, int pulsesToSpin)
*
*	Args:
*
*		unsigned long numberOfPulses: 	number of pulses returned from the encoder
*		int pulsesToSpin: 				number of pulses needed to complete a spin
*
*	Actions:
*
*		Calculate the pulses needed to a Spin
*/
int calculesSpin(unsigned long numberOfPulses, int pulsesToSpin)
{
	return (numberOfPulses/pulsesToSpin);
}

/*
*	writeInLCD(String message, int initialPosition)
*
*	Args:
*
*		message: 			message to write in the lcd 
*		initialPosition:	point where the first letter will be written
*
*	Actions:
*
*		This method will write the received message in the lcd  
*/
void writeInLCD(String message, int initialPosition)
{
	lcd.begin(20, 4); 

	lcd.setCursor(1,0);
  	lcd.print("UFPR LATA - DUNKEL");  
	lcd.setCursor(initialPosition,3);
  	lcd.print(message);  
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

/*
*	counter6()
*
*	Args:
*
*	Actions:
*
*		Invoque the increasePulseCounter method from encoder 6
*/
void counter6()
{
 	encoder6.increasePulseCounter(); 
}

/*============================================================================*/