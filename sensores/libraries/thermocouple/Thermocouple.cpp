#include <Arduino.h>
#include "Thermocouple.h"

/*
*	Thermocouple.cpp
*
*	Version 1.0.0
*
*	Copyright (c) 2015 Centro Universitário Católica de Santa Catarina
*
*	This file is part of Encoder Library.
*
*   Thermocouple Library is free software: you can redistribute it and/or modify
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
*		int port: interrupt port used for the thermocouple
*
*	Actions:
*
*		Set the port variable
*/
Thermocouple::Thermocouple(int port)
{
	this->setPort(port);
}

/*
*	setPort(int port)
*
*	Args:
*	
*		int port: interrupt port used for the thermocouple
*
*	Actions:
*
*		Set the port variable
*/
void Thermocouple::setPort(int port)
{
	this->port = port;
}

/*
*	getPort()
*
*	Args:
*	
*	Actions:
*
*		Get the port variable value
*/
int Thermocouple::getPort()
{
	return this->port;
}

/*
*	read()
*
*	Args:
*	
*	Actions:
*
*		Read the analog port
*/
int Thermocouple::read()
{
	return analogRead(this->port);
}