#include <Arduino.h>


#ifndef noiseSensor_h
#define noiseSensor_h


class NoiseSensor{
public:
	NoiseSensor(int port);
	int readSensor();

private:
	int port;

};
#endif
