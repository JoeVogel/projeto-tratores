#include <Arduino.h>
#include "Pitot.h"
Pitot::Pitot(int port)
{
	this->setPort(port);
}

/*
*	setPort(int port)
*
*	Args:
*	
*		int port: analog port used for the pitot
*
*	Actions:
*
*		Set the port variable
*/
void Pitot::setPort(int port)
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
int Pitot::getPort()
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
int Pitot::read()
{
	return analogRead(this->port);
}
