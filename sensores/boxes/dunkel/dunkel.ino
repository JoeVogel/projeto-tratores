#include <Arduino.h>
#include "Encoder.h"
#include "Fluxometer.h"
#include "Thermocouple.h"
#include "Pitot.h"
#include "HX711.h"

/*
*	dunkel.ino
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

#define SEND_TIME 5

const String MAC					= "D2";

//Fluxometers
const int fuelInPort				= 2;
const int fuelOutPort				= 3;

//Encoders
const int encoderPort 				= 19;

//Thermocouples (analog ports)
const int fuelInTemperaturePort		= 4;
const int fuelOutTemperaturePort	= 5;
const int engineAirInPort			= 6;
const int exhaustGasesPort			= 7;
const int waterCoolingPort			= 8;

//Air flow (analog port)
const int engineAirFlowPort			= 9;

//Time handlers
unsigned long timeOnStart			= 0;
unsigned long timeToSend			= 0;
unsigned long lastTimeSended		= 0;
unsigned long currentTime			= 0;
unsigned long diffTime				= 0;
int loopCounter						= 0;

//Encoder handlers
unsigned long previousEncoderValue	= 0;
unsigned long lastEncoderValue		= 0; 

//Fluxometers handlers
unsigned long previousFuelInValue	= 0;
unsigned long lastFuelInValue		= 0;
unsigned long previousFuelOutValue	= 0;
unsigned long lastFuelOutValue		= 0;

char incomingFlag; 											//Flag received from cordinator
boolean canSend 					= false;				//Responsable for the working status
unsigned long packageCounter		= 0; 

Encoder encoder(encoderPort);								//Encoder 5ª wheel

Fluxometer		fluxometerFuelIn(fuelInPort);				//Fuel flow meter in the engine inlet
Fluxometer		fluxometerFuelOut(fuelOutPort);				//Flowmeter of the fuel in the engine return

Thermocouple	temperatureFuelIn(fuelInTemperaturePort);	//Fuel temperature meter in the engine inlet
Thermocouple	temperatureFuelOut(fuelOutTemperaturePort);	//Temperature meter of the fuel return in the engine 
Thermocouple	temperatureEngineAirIn(engineAirInPort);	//Temperature meter of air entering the engine
Thermocouple	temperatureExhaustGases(exhaustGasesPort);	//Temperature meter of exhaust gases
Thermocouple	temperatureWaterCooling(waterCoolingPort);	//Temperature meter of cooling water

Pitot			engineAirFlow(engineAirFlowPort);	

HX711 			scale(7, 8, 64);							

/*
*	setup()
*
*	Args:
*
*	Actions:
*
*		Initialize serial, setup interruptions and the scala 
*/
void setup()
{
	Serial.begin(9600);
	configureInterruptions();
	scale.set_scale(2280.f);
  	scale.tare(); 
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

	if(canSend && myTimer())
	{

		sendPackage();
		lastTimeSended = timeToSend;

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
	attachInterrupt(0, &encoderPulseCounter, FALLING);
	attachInterrupt(1, &fluxometerFuelInCounter, FALLING);
	attachInterrupt(2, &fluxometerFuelOutCounter, FALLING);
}

/*
*	verifyStartCommand()
*
*	Args:
*
*	Actions:
*
*		If it receives a command from cordinator, it verifies the received flag, and then, 
*		If the flag is 1, the variables are reset, and the return is incomingFlag
*		If the flag is 2, it only return the incomingFlag
*		else, it only returns 0
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
			packageCounter 			= 0;
			previousEncoderValue 	= 0;
			previousFuelInValue 	= 0;
			previousFuelOutValue	= 0;
			encoder.initPulseCounter();
			fluxometerFuelIn.initPulseCounter();
			fluxometerFuelOut.initPulseCounter();	
			
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
*	sendPackage()
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

	lastEncoderValue = encoder.getPulseCounter();
	int spinEncoder = (lastEncoderValue - previousEncoderValue);
	previousEncoderValue = lastEncoderValue; 

	lastFuelInValue = fluxometerFuelIn.getPulseCounter();
	int fuelIn = (lastFuelInValue - previousFuelInValue);
	previousFuelInValue = lastFuelInValue;

	lastFuelOutValue = fluxometerFuelOut.getPulseCounter();
	int fuelOut = (lastFuelOutValue - previousFuelOutValue);
	previousFuelOutValue = lastFuelOutValue;

	int scaleValue = (int)scale.get_units();

	package.concat(MAC);
	package.concat(";");
	package.concat(String(timeToSend, HEX));
	package.concat(";");
	package.concat(String(packageCounter, HEX));
	package.concat(";");
	package.concat(String(spinEncoder, HEX));
	package.concat(";");
	package.concat(String(fuelIn, HEX));
	package.concat(";");
	package.concat(String(fuelOut, HEX));
	package.concat(";");
	package.concat(String(temperatureFuelIn.read(), HEX));
	package.concat(";");
	package.concat(String(temperatureFuelOut.read(), HEX));
	package.concat(";");
	package.concat(String(temperatureEngineAirIn.read(), HEX));
	package.concat(";");
	package.concat(String(temperatureExhaustGases.read(), HEX));
	package.concat(";");
	package.concat(String(temperatureWaterCooling.read(), HEX));
	package.concat(";");
	package.concat(String(engineAirFlow.read(), HEX));
	package.concat(";");
	package.concat(String(scaleValue, HEX));

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
float calculesSpin(unsigned long numberOfPulses, int pulsesToSpin)
{
	return (numberOfPulses/pulsesToSpin);
}

/*===	Encoders interrupt handlers ===========================================*/

/*
*	encoderPulseCounter()
*
*	Args:
*
*	Actions:
*
*		Invoque the increasePulseCounter method from encoder
*/
void encoderPulseCounter()
{
 	encoder.increasePulseCounter(); 
}

/*============================================================================*/

/*===	Fluxometers interrupt handlers =======================================*/

/*
*	fluxometerInCounter()
*
*	Args:
*
*	Actions:
*
*		Invoque the increasePulseCounter method from fluxometer
*/
void fluxometerFuelInCounter()
{
 	fluxometerFuelIn.increasePulseCounter(); 
}

/*
*	fluxometerOutCounter()
*
*	Args:
*
*	Actions:
*
*		Invoque the increasePulseCounter method from fluxometer
*/
void fluxometerFuelOutCounter()
{
 	fluxometerFuelOut.increasePulseCounter(); 
}

/*==========================================================================*/
