#include <Arduino.h>

/*
* Time.h
*
* Version 1.0.0
*
* Copyright (c) 2015 Centro Universitário Católica de Santa Catarina
*
* This file is part of Time Library.
*
*   Time Library is free software: you can redistribute it and/or modify
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

#ifndef Time_h
#define Time_h

class Time {

  public: 
    
    int getSecond();

    int getMillis();

    int getMinute();

    int getHour(); 
   
    char* getTime(unsigned long millisLocal);

  private:

    int hour;
    int minute;
    int second;
    int milliss;
    char currentTime[8];

};

#endif
