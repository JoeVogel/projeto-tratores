#include <Arduino.h>
#include "Encoder.h"

/*
*	Encoder.cpp
*
*	Version 1.0.0
*
*	Copyright (c) 2015 Centro Universitário Católica de Santa Catarina
*
*	This file is part of Encoder Library.
*
*   Encoder Library is free software: you can redistribute it and/or modify
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

/*
*	Encoder(int encoderPort)
*
*	Args:
*	
*		int encoderPort: interrupt port used for the encoder
*
*	Actions:
*
*		Start the Pulse Counter, and set the pin mode
*/
Encoder::Encoder(int encoderPort)
{

    this->setEncoderPort(encoderPort);
	this->initPulseCounter();

  	pinMode(this->encoderPort, INPUT);
  	digitalWrite(this->encoderPort, HIGH);

}

/*
*	initPulseCounter()
*
*	Args:
*
*	Actions:
*
*		Initiate the pulse counter.
*/
void Encoder::initPulseCounter()
{
	this->pulseCounter = 0;
}

/*
*	setEncoderPort(int encoderPort)
*
*	Args:
*
*		encoderPort: number from encoder port 
*
*	Actions:
*
*		Set the encoderPort variable
*/
void Encoder::setEncoderPort(int encoderPort)
{
	this->encoderPort = encoderPort;
}

/*
*	getEncoderPort()
*
*	Args:
*
*	Actions:
*
*		Returns the encoderPort number 
*/
int Encoder::getEncoderPort()
{
	return this->encoderPort;
}

/*
*	getPulseCounter()
*
*	Args:
*
*	Actions:
*
*		Returns the pulseCounter value
*/
unsigned long Encoder::getPulseCounter()
{
	return this->pulseCounter;
}

/*
*	increasePulseCounter()
*
*	Args:
*
*	Actions:
*	
*		Increase a pulse to pulseCounter
*/
void Encoder::increasePulseCounter()
{
  this->pulseCounter++;   
}

/*
*	getSpinCounter()
*
*	Args:
*
*		int pulsesToSpin: number of pulses needed to a full spin

*	Actions:
*
*		Returns the spin counter value
*/
unsigned int Encoder::getSpinCounter(int pulsesToSpin)
{
	return (this->pulseCounter/pulsesToSpin);
}