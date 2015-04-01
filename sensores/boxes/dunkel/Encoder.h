#include <Arduino.h>

/*
* Encoder.h
*
* Version 1.0.0
*
* Copyright (c) 2015 Centro Universitário Católica de Santa Catarina
*
* This file is part of Encoder Library.
*
*   Encoder Library is free software: you can redistribute it and/or modify
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
