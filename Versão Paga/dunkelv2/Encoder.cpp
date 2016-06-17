#include <Arduino.h>
#include "Encoder.h"

Encoder::Encoder(int encoderPort)
{

    this->setEncoderPort(encoderPort);
	this->initPulseCounter();

  	pinMode(this->encoderPort, INPUT);
  	digitalWrite(this->encoderPort, HIGH);

}

void Encoder::initPulseCounter()
{
	this->pulseCounter = 0;
}

void Encoder::setEncoderPort(int encoderPort)
{
	this->encoderPort = encoderPort;
}

int Encoder::getEncoderPort()
{
	return this->encoderPort;
}

volatile unsigned long  Encoder::getPulseCounter()
{
	return this->pulseCounter;
}

void Encoder::increasePulseCounter()
{
  this->pulseCounter = this->pulseCounter + 1;   
}

unsigned int Encoder::getSpinCounter(int pulsesToSpin)
{
	return (this->pulseCounter/pulsesToSpin);
}
