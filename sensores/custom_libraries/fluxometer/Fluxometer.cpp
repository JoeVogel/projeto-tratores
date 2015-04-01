#include <Arduino.h>
#include "Fluxometer.h"

/*
*	Fluxometer.cpp
*
*	Version 1.0.0
*
*	Copyright (c) 2015 Centro Universitário Católica de Santa Catarina
*
*	This file is part of Encoder Library.
*
*   Fluxometer Library is free software: you can redistribute it and/or modify
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
*	Constructor
*
*	Args:
*
*		fluxometerPort: interrupt port used for the fluxometer
*
*	Actions:
*
*		Start the pulse counter, and set the pin mode.
*/
Fluxometer::Fluxometer(int fluxometerPort)
{
	this->setFluxometerPort(fluxometerPort);
	this->initPulseCounter();

  	pinMode(this->fluxometerPort, INPUT);
  	digitalWrite(this->fluxometerPort, HIGH);
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
void Fluxometer::initPulseCounter()
{
	this->pulseCounter = 0;
}

/*
*	setFluxometerPort(int fluxometerPort)
*
*	Args:
*
*		fluxometerPort: number from fluxometer port 
*
*	Actions:
*
*		Set the fluxometerPort variable
*/
void Fluxometer::setFluxometerPort(int fluxometerPort)
{
	this->fluxometerPort = fluxometerPort;
}

/*
*	getFluxometerPort()
*
*	Args:
*
*	Actions:
*
*		Returns the fluxometerPort number 
*/
int Fluxometer::getFluxometerPort()
{
	return this->fluxometerPort;
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
unsigned long Fluxometer::getPulseCounter()
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
void Fluxometer::increasePulseCounter()
{
  this->pulseCounter++;   
}