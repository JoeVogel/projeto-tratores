#include "Accelerometer.h"
#include <arduino.h>

/*
*	Contructor
*/
Accelerometer::Accelerometer(int portX, int portY, int portZ){
	this->portX = portX;	
	this->portY = portY;
	this->portZ = portZ;
}

int Accelerometer::getAxisValueX(){
	return analogRead(portX);
}

int Accelerometer::getAxisValueY(){
	return analogRead(portY);
}

int Accelerometer::getAxisValueZ(){
	return analogRead(portZ);
}	  

	
