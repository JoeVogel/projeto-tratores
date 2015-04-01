#include <Arduino.h>

/*
* Pitot.h
*
* Version 1.0.0
*
* Copyright (c) 2015 Centro Universitário Católica de Santa Catarina
*
* This file is part of Encoder Library.
*
*   Pitot Library is free software: you can redistribute it and/or modify
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

#ifndef pitot_h
#define pitot_h
 
class Pitot
{
  
  public:
  
  			Pitot(int port);
    void	setPort(int port);
    int		getPort(); 

    int		read();
    
  private:
  
    int		port;
}; 
 
#endif