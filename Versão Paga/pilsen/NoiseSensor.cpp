#include "NoiseSensor.h"
#include <Arduino.h>

/*
*	Contructor
*/
NoiseSensor::NoiseSensor(int port){
	this->port = port;
}

int NoiseSensor::readSensor(){
	return analogRead(this->port);
}
