/*#include <Arduino.h>
#include "Wire.h"
#include "Adafruit_BMP085.h"
#include "DHT.h"


* ReadSensors.cpp
*
* Version 1.0.0
*
* Copyright (c) 2015 Centro Universitário Católica de Santa Catarina
*
* This file is part of ReadSensors Library.
*
*   ReadSensors Library is free software: you can redistribute it and/or modify
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
*   

void ReadSensors::setDHT(int port, String sensorModel)
{
	this->dht = new DHT(port, sensorModel);
}

void ReadSensors::setPortAccelerometer(int x, int y, int z){
	this->portX = x;
	this->portY = y;
	this->portZ = z;
}
float ReadSensors::getSensorNoise(){
	
	return analogRead(portNoiseSensor)
}

//is the read sensor humidity
float ReadSensors::getHumiditySensor(){
	
	return dht.readHumidity();
}

float ReadSensors::getTemperatureSensor(){
	
	return dht.readTemperature();
}

//is the read sensor altitude
float ReadSensors::readSensorAltitude(){
	
	return bmp.readSensorAltitude();
}
//is the read sensor pressure
float ReadSensors::readSensorPressure(){  
	  	
  	return  bmp.readPressure();
}

float ReadSensors::getAccelerometerX(){

	return analogRead(portX);
}

float ReadSensors::getAccelerometerY(){

	return analogRead(portY);
}

float ReadSensors::getAccelerometerZ(){

	return analogRead(portZ);
}*/	
