#include <Arduino.h>

String package; 

void setup()
{
	Serial.begin(9600);

	for(int i = 0; i < 10; i++)
	{
		package.concat("BLA;");
	}

	Serial.println(package);
}

void loop()
{

}