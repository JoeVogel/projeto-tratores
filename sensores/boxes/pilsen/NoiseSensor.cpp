#include "NoiseSensor.h"
#include <arduino.h>

/*
*	Contructor
*/
NoiseSensor::NoiseSensor(int port){
	this->port = port;	
}

int NoiseSensor::readSensor(){
	return analogRead(this->port);
}
