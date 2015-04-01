#include "Arduino.h"
#include "Time.h"

/*
*	Time.cpp
*
*	Version 1.0.0
*
*	Copyright (c) 2015 Centro Universitário Católica de Santa Catarina
*
*	This file is part of Time Library.
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

//Analisar junto com o Manfred como utilizar hora

char* Time::getTime(unsigned long millisLocal){

	String timeLocal="";
	//this->milliss =millisLocal%1000;
 	this->second =(millisLocal/1000)%60;
 	this->minute =((millisLocal/1000)/60)%60;
 	this->hour = ((millisLocal/1000)/60)/60;
        sprintf(currentTime,"%c; :%c; :%c", this->hour, this->minute, this->second); 
 
 	return currentTime;
 }


