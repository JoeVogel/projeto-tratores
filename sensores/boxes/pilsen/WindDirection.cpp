#include "WindDirection.h"
#include <arduino.h>

/*
*	Contructor
*/

WindDirection::WindDirection(int port){
	this->port = port;	
}

int WindDirection::readSensor(){
	return analogRead(this->port);
}
