#include <Arduino.h>
#ifndef encoder_h
#define encoder_h
 
class Encoder
{
  
  public:
 
    Encoder(int encoderPort); 

    
    int                     getEncoderPort(); 

    void                    initPulseCounter();
    volatile unsigned long  getPulseCounter();
    unsigned int            getSpinCounter(int pulsesToSpin);
    
    void                    increasePulseCounter();
    
  private:
  
    int                     encoderPort;
    volatile unsigned long  pulseCounter;

    void                    setEncoderPort(int encoderPort);

}; 
 
#endif
