#include <arduino.h>
#include <LiquidCrystal.h>
#include "Encoder.h"
#include "NoiseSensor.h"
#include "Adafruit_BMP085.h"
#include "DHT.h"
#include "WindDirection.h"

/*
*	Pilsen.ino
*
*	Version 1.0.0
*
*	Copyright (c) 2015 Centro Universitário Católica de Santa Catarina
*
*	This file is part of Pilsen Box.
*
*   Pilsen Box source code is free software: you can redistribute it and/or modify
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
#define SEND_TIME 5

//Time handlers
unsigned long timeToSend             = 0;
unsigned long lastTimeSended         = 0;
unsigned long currentTime            = 0;
unsigned long diffTime               = 0;
int loopCounter                      = 0;

char incomingFlag;                                  //Flag received from cordinator
boolean canSend                      = false;       //Responsable for the working status
unsigned long packageCounter         = 0;
bool resetVariable                   = true;

//Encoders
const int encoderPort               = 19;

//Encoder handlers
unsigned long previousEncoderValue	 = 0;
unsigned long lastEncoderValue		 = 0; 

//MAC of BOX
String MAC                           = "P1";

//configure the ports of sensors
const int portDHT                    = 26;
String sensorModel                   = "DHT22";
const int portNoiseSensor            = 8;

//verificar porta correta  
const int portWindDirection          = 100;
int valueNoise;  

//Create objects
Encoder encoder(encoderPort);
WindDirection windDirection(portWindDirection);
Adafruit_BMP085 bmp;
LiquidCrystal lcd(13,12,11,10,9,8);  
DHT dht(26, DHT22);
NoiseSensor noiseSensor(portNoiseSensor);


/*
*   setup()
*
*   Args:
*
*   Actions:
*
*       Initialize serial, initialize Barometer(BMP), setup interruptions and write status in LCD
*/

void setup()
{

    Serial.begin(9600);
    writeInLCD("CONFIGURATION OK.", 3);  
    bmp.begin();   
    configureInterruptions(); 

}

void loop()
{
  
    verifyStartCommand();   

    if (canSend)
    {
        writeInLCD("WORKING...", 3);  

        calculatingTheSpurNoise();

        if(myTimer())
        {

            writeInLCD("SENDING PACKAGE...", 3);            
            sendPackage(); 
            lastTimeSended = timeToSend;
            resetVariables();
        }
    }
    else
    {
        writeInLCD("STAND BY.", 3);
    }


}

/*
*   configureInterruptions()
*
*   Args:
*
*   Actions:
*
*       Configure the interruptions
*/

void configureInterruptions()
{
    attachInterrupt(0, &encoderPulseCounter, FALLING);	
}

/*
*   calculatingTheSpurNoise()
*
*   Args:
*
*   Actions:
*
*       Checks if the variable is reset and there was an increase of noise
*/

void calculatingTheSpurNoise(){
    int atualNoise= noiseSensor.readSensor();
  
    if(atualNoise > valueNoise){
        valueNoise= atualNoise;  
    }    
}

/*
*   calculatingCompass()
*
*   Args:
*
*   Actions:
*
*       Calculate wind direction in relation to compass
*/

String calculatingCompass(int windDirection){
    float vane =  (3.3 / 1024) * windDirection;

    if( vane<0.25 )
    {
        return "ESE";
    }
    else if( vane>=0.25 && vane<0.28 )
    {
        return "ENE";
    }
    else if( vane>=0.28 && vane<0.35 ) 
    {
        return "E";
    }
    else if( vane>=0.35 && vane<0.5 )
    {
        return "SSE";
    }
    else if( vane>=0.5 && vane<0.65 )
    {
        return "SE";
    }
    else if( vane>=0.65 && vane<0.85 )
    {
        return "SSW";
    }
    else if( vane>=0.85 && vane<1.1 )
    {
        return "S";
    }
    else if( vane>=1.1 && vane<1.38 ) 
    {
        return "NNE";
    }
    else if( vane>=1.38 && vane<1.6 )
    {
        return "NE";
    }
    else if( vane>=1.6 && vane<1.96 )
    {
        return "WSW";
    }
    else if( vane>=1.96 && vane<2.15 ) 
    {
        return "SW";
    }
    else if( vane>=2.15 && vane<2.35 ) 
    {
        return "NNW";
    }
    else if( vane>=2.35 && vane<2.6 )
    {
        return "N";
    }
    else if( vane>=2.6 && vane<2.8 ) 
    {
        return "WNW";
    }
    else if( vane>=2.8 && vane<3.1 ) 
    {
        return "W";
    }
    else if( vane>=3.1 ) 
    {
        return "NW";
    }

    return "";
}

/*
*   resetVariables()
*
*   Args:
*
*   Actions:
*
*      If the method is executed it resets the variables with some types of values.
*      
*
*/


void resetVariables(){
  
   valueNoise = 0;
   resetVariable = true;
  
}

/*===	Encoders interrupt handlers ===========================================*/

/*
*	encoderPulseCounter()
*
*	Args:
*
*	Actions:
*
*		Invoque the increasePulseCounter method from encoder
*/
void encoderPulseCounter()
{
    encoder.increasePulseCounter(); 
}

/*
*	sendPackage()
*
*	Args:
*
*	Actions:
*
*		Cast data to hexadecimal and then send the package
*/

void sendPackage()
{
    String package;

    packageCounter ++;
    
    Serial.println(fillDataInPackage(package));  
    
}

/*
*	fillDataInPackage(String package)
*
*	Args:
*
*		package:	receives the package to fill it with data
*
*	Actions:
*
*		Fills the received package with the encoders data and then return it
*/
String fillDataInPackage(String package)
{

    lastEncoderValue = encoder.getPulseCounter();
    int spinEncoder = (lastEncoderValue - previousEncoderValue);
    previousEncoderValue = lastEncoderValue; 
    //int windDirectionActual = windDirection.readSensor();	

    int altitude  = (int)bmp.readAltitude();
    int temperature = (int)(dht.readTemperature() * 10);
    int humidity = (int)(dht.readHumidity() * 10);

    package.concat(MAC);
    package.concat(";");
    package.concat(String(timeToSend, HEX));
    package.concat(";");
    package.concat(String(packageCounter, HEX));
    package.concat(";");
    package.concat(String(humidity, HEX));
    package.concat(";");
    package.concat(String(temperature, HEX));
    package.concat(";");
    package.concat(String(altitude, HEX));
    package.concat(";");
    package.concat(String(bmp.readPressure(), HEX));
    package.concat(";");
    //verificar que retorna string
    package.concat(String(calculatingCompass(windDirection.readSensor())));
    package.concat(";");
    package.concat(String(spinEncoder ,HEX));
	
    return package;
}

/*
*   writeInLCD()
*
*   Args:
*
*   Actions:
*
*       Writes on the LCD status box
*/

void writeInLCD(String message, int initialPosition)
{
    lcd.begin(20, 4);
    lcd.setCursor(5,0);
    lcd.print("UFPR LATA / PILSEN");  
    lcd.setCursor(initialPosition,3);
    lcd.print(message);  
}

/*
*   verifyStartCommand()
*
*   Args:
*
*   Actions:
*
*       If it receives a command from cordinator, it verifies the received flag, and then, 
*       If the flag is 1, the variables are reset, and the return is incomingFlag
*       If the flag is 2, it only return the incomingFlag
*       else, it only returns 0
*/
void verifyStartCommand()
{
    if (Serial.available() > 0) 
    {   
        incomingFlag = (char)Serial.read();

        if (incomingFlag == '1')
        {
            lastTimeSended  = 0;
            packageCounter  = 0; 
            resetVariables();
            encoder.initPulseCounter();             
            canSend = true;
        }
        else
        {
            canSend = false;
        }
    }
}

/*
*   myTimer()
*
*   Args:
*
*   Actions:
*
*       Based on the millis() of the Arduino, it calculates the elapsed time since the start command received from cordinator
*/
bool myTimer()
{
    currentTime = (millis()/1000);
    diffTime = currentTime - lastTimeSended;

    if (diffTime >= SEND_TIME)
    {
        timeToSend = currentTime;
        return true;
    }

    return false;
}
