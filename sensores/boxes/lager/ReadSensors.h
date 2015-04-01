#include <Arduino.h>
#include "Wire.h"
#include "Adafruit_BMP085.h"
#include "DHT.h"

#ifndef ReadSensors_h
#define ReadSensors_h


/*
* ReadSensors.h
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
*   along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/


class ReadSensors
{
	public:

		//is the start of all necessary libraries
		void setDHT(int portDHT, String sensorModel);
		
		void setPortNoiseSensor(int portNoiseSensor);
		//is set port of accelerometer x, y and z
		void setPortAccelerometer(int x, int y, int z);
	
		//is the get noise sensor
		float getSensorNoise();

		float getAccelerometerXMax();

		float getAccelerometerYMax();		

		float getAccelerometerZMax();		

	
		//is the get humidity sensor
		float getHumiditySensor();
		//is the get temperature sensor
		float getTemperatureSensor();
		//is the get sensor altitude
		float getAltitudeSensor();
		//is the get sensor pressure
		float getPressureSensor();

		

	
	private:

		DHT dht;

		Adafruit_BMP085 bmp;

		int portNoiseSensor;
		
		float pressure;
		float altitude;
		float temperature;
		float humidity;
		int portX;
		int portY;
		int portZ;



	
};
#endif


