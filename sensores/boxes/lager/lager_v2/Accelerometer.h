#include <arduino.h>

#ifndef Accelerometer_h
#define Accelerometer_h

class Accelerometer{
public:
	Accelerometer(int portX, int portY, int portZ);
	int getAxisValueX();	   
	int getAxisValueY();
	int getAxisValueZ();

private:
	int portX; 
	int portY;
	int portZ;  
};
#endif
