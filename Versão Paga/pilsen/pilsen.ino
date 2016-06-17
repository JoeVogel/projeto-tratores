#include <Arduino.h>
#include <Wire.h>
#include "NoiseSensor.h"
#include <Adafruit_BMP085.h>
#include "DHT.h"

#define SEND_TIME 1

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

int x=9;
int y=10;
int z=11;

int portGSel                    = 16;

//MAC of BOX
String MAC = "P1";

int portDHT = 14;
String sensorModel = "DHT22";
int portNoiseSensor = 8;

int valueNoise;

Adafruit_BMP085 bmp;
DHT dht(15, DHT22);
NoiseSensor noiseSensor(portNoiseSensor);

void setup()
{
    Serial.begin(9600);

    pinMode(19,OUTPUT); //VCC BMP
    pinMode(17,OUTPUT); //GND BMP
    pinMode(16,OUTPUT); //Porta GSel acelerometro

    digitalWrite(19,HIGH);
    digitalWrite(17,LOW);
    digitalWrite(16,HIGH);

    bmp.begin();

}

void loop()
{

    verifyStartCommand();

    if (canSend)
    {

        if (myTimer())
        {
            sendPackage();
            lastTimeSended = timeToSend;
        }
    }

}

/*
*   sendPackage()
*
*   Args:
*
*   Actions:
*
*       Send the package
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

    package.concat(MAC);
    package.concat(";");
    package.concat(packageCounter);
    package.concat(";");
    package.concat(dht.readHumidity());
    package.concat(";");
    package.concat(dht.readTemperature());
    package.concat(";");
    package.concat(bmp.readAltitude());
    package.concat(";");
    package.concat(bmp.readPressure());//doubleToString(bmp.readPressure(), 3));
    package.concat(";");
    package.concat(noiseSensor.readSensor());
    package.concat(";");
    package.concat(analogRead(9));
    package.concat(";");
    package.concat(analogRead(10));
    package.concat(";");
    package.concat(analogRead(11));

    return package;
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

        if (incomingFlag == '1' || incomingFlag == '2' || incomingFlag == '3')
        {
            timeOnStart = (millis()/1000);
            lastTimeSended  = 0;
            packageCounter  = 0;
            canSend = true;

            String package = "";
            package.concat("MAC");
            package.concat(";");
            package.concat("CONT_PCT");
            package.concat(";");
            package.concat("UMID");
            package.concat(";");
            package.concat("TEMP");
            package.concat(";");
            package.concat("ALT");
            package.concat(";");
            package.concat("PRES");
            package.concat(";");
            package.concat("RUIDO");
            package.concat(";");
            package.concat("ACEL_X");
            package.concat(";");
            package.concat("ACEL_Y");
            package.concat(";");
            package.concat("ACEL_Z");

            Serial.println(package);
            delay(1000);

        }
        else if (incomingFlag == '0')
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
    currentTime = (millis()/1000) - timeOnStart;
    diffTime = currentTime - lastTimeSended;

    if (diffTime >= SEND_TIME)
    {
        timeToSend = currentTime;
        return true;
    }

    return false;
}