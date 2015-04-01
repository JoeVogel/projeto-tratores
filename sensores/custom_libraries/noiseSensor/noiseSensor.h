#include <arduino.h>


#ifndef noiseSensor_h
#define noiseSensor_h


class NoiseSensor{
public:
	NoiseSensor(int port);
	float readSensor();	   

private:
	int port;
  
}
#endif