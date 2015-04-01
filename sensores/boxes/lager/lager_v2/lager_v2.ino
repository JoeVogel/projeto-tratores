#include <Arduino.h>
#include <LiquidCrystal.h>
#include "NoiseSensor.h"
#include "Accelerometer.h"
#include "Adafruit_BMP085.h"
#include "DHT.h"



/*
*	Lager.ino
*
*	Version 1.0.0
*
*	Copyright (c) 2015 Centro Universitário Católica de Santa Catarina
*
*	This file is part of Lager Box.
*
*   Lager Box source code is free software: you can redistribute it and/or modify
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

//A partir daqui eu editei...
#define SEND_TIME 5

//Time handlers
unsigned long timeToSend        = 0;
unsigned long lastTimeSended    = 0;
unsigned long currentTime       = 0;
unsigned long diffTime          = 0;
unsigned long timeOnStart       = 0;
int loopCounter                 = 0;

char incomingFlag;                          //Flag received from cordinator
boolean canSend                 = false;    //Responsable for the working status
unsigned long packageCounter    = 0;
bool resetVariable = true;
//Até aqui. Ass: Joe

int x=9;
int y=10;
int z=11;

//MAC of BOX
String MAC = "L1";

//configure the ports of sensors
int portDHT = 26;
String sensorModel = "DHT22";
int portNoiseSensor = 8;

int valueXMax;
int valueXMin;
int valueYMax;
int valueYMin;        
int valueZMax;
int valueZMin;  
int valueNoise;  

//Configure ports of Libraries
Adafruit_BMP085 bmp;
LiquidCrystal lcd(13,12,11,10,9,8);  // MEGA
DHT dht(26, DHT22);
NoiseSensor noiseSensor(portNoiseSensor);
Accelerometer accelerometer(x, y, z);


void setup() 
{
    Serial.begin(9600);
    lcd.begin(20, 4);  
    writeInLCD("CONFIGURATION OK.", 3);  
    bmp.begin();     
  
}

void loop() 
{    

    verifyStartCommand();   

    if (canSend)
    {
        writeInLCD("WORKING...", 3);

        calculatingTheSpurNoise();   
        calculateAccelerometer();

        if (myTimer())
        {
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


void calculatingTheSpurNoise()
{
    int atualNoise= noiseSensor.readSensor();
  
    if(resetVariable){
        valueNoise = atualNoise;
    }else if(atualNoise > valueNoise){
        valueNoise = atualNoise;
    }
}


void calculateAccelerometer()
{
    int atualX = accelerometer.getAxisValueX();
    int atualY = accelerometer.getAxisValueY();
    int atualZ = accelerometer.getAxisValueZ();
    
    //checks if the variables were reset
    if(resetVariable){
      
      valueXMax = atualX;
      valueXMin = atualX;
      valueYMax = atualY;
      valueYMin = atualY;
      valueZMax = atualZ;
      valueZMin = atualZ;
      resetVariable = false;
      
    }else{
        if(valueXMax < atualX){
          valueXMax = atualX;
        }else if(valueXMin > atualX){
          valueXMin = atualX;
        }

        if(valueYMax < atualY){
            valueYMax = atualY;
        }else if(valueYMin > atualY){
            valueYMin = atualY;
        }

        if(valueZMax < atualZ){
            valueZMax = atualZ;
        }else if(valueZMin > atualZ){
            valueZMin = atualZ;
        }    
    }
    
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

void resetVariables()
{
  
   valueXMax = 0;
   valueXMin = 0;
   valueYMax = 0;
   valueYMin = 0;
   valueZMax = 0;
   valueZMin = 0;
   valueNoise = 0;
   resetVariable = true;
  
}

/*
*   sendPackage()
*
*   Args:
*
*   Actions:
*
*       Cast data to hexadecimal and then send the package
*/

void sendPackage()
{
    String package;

    packageCounter ++;  
    
    Serial.println(fillDataInPackage(package));   
}

/*
*   fillDataInPackage(String package)
*
*   Args:
*
*       package:    receives the package to fill it with data
*
*   Actions:
*
*       Fills the received package with the encoders data and then return it
*/

String fillDataInPackage(String package)
{
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
    package.concat(String(valueNoise, HEX));
    package.concat(";");
    package.concat(String(valueXMax, HEX));
    package.concat(";");
    package.concat(String(valueXMin, HEX));
    package.concat(";");
    package.concat(String(valueYMax, HEX));
    package.concat(";");
    package.concat(String(valueYMin, HEX));
    package.concat(";");
    package.concat(String(valueZMax, HEX));
    package.concat(";");
    package.concat(String(valueZMin, HEX));
    

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
    lcd.print("UFPR LATA / LAGER");  
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
            timeOnStart = (millis()/1000);
            lastTimeSended  = 0;
            packageCounter  = 0;
            resetVariables();  
            
            canSend = true;
        }
        else
        {
            //canSend = false;
            canSend = true;
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
    currentTime = (millis()/1000) - timeOnStart;
    diffTime = currentTime - lastTimeSended;

    if (diffTime >= SEND_TIME)
    {
        timeToSend = currentTime;
        return true;
    }

    return false;
}
