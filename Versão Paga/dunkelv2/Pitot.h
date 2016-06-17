#include <Arduino.h>
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
