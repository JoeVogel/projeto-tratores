#include <NoiseSensor.h>
#include <arduino.h>

/*
*	Contructor
*/
NoiseSensor::NoiseSensor(int port){
	this->port = port;	
}

float NoiseSensor::readSensor(){
	return analogRead(this->port);
}
