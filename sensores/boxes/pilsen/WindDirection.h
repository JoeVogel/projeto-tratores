#include <arduino.h>


#ifndef windDirection_h
#define windDirection_h


class WindDirection{
public:
	WindDirection(int port);
	int readSensor();	   

private:
	int port;
  
};
#endif
