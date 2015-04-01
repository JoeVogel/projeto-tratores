#include <Arduino.h>

/*
* Fluxometer.h
*
* Version 1.0.0
*
* Copyright (c) 2015 Centro Universitário Católica de Santa Catarina
*
* This file is part of Encoder Library.
*
*   Fluxometer Library is free software: you can redistribute it and/or modify
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

#ifndef fluxometer_h
#define fluxometer_h
 
class Fluxometer
{
  
  public:
  
                        Fluxometer(int fluxometerPort);

    void                setFluxometerPort(int encoderPort);
    int                 getFluxometerPort(); 

    void                initPulseCounter();
    unsigned long       getPulseCounter();
    
    void                increasePulseCounter();
    
  private:
  
    int                 fluxometerPort;
    unsigned long       pulseCounter;

}; 
 
#endif