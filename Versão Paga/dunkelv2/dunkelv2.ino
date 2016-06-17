#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Encoder.h"
#include "HX711.h"
#include "FreqMeasure.h"

//Thermocouples
#define CLK 9

#define DBIT0 6
#define DBIT1 10
#define DBIT2 11
#define DBIT3 12
#define CS 13

#define SEND_TIME 1

const String MAC				= "D2";

//Fluxometers
const int fuelInPort				= 3;
const int fuelOutPort				= 2;

//Time handlers
unsigned long timeOnStart			= 0;
unsigned long timeToSend			= 0;
unsigned long lastTimeSended		= 0;
unsigned long currentTime			= 0;
unsigned long diffTime				= 0;
int loopCounter						= 0;

//Thermocouple handlers
int thermocoupleFuelIn;
int thermocoupleFuelOut;
int thermocoupleEngineOil;
int thermocoupleWatterCooler;

//Frequency handlers
double frequencyReadSum				= 0;
int frequencyReadsCount				= 0;

char incomingFlag; 											//Flag received from cordinator
boolean canSend 					= false;				//Responsable for the working status
unsigned long packageCounter		= 0;
char tempChar;
String dataIn;
String dataTemp;
int functionSelector;

Encoder		fluxometerFuelIn(fuelInPort);				//Fuel flow meter in the engine inlet
Encoder		fluxometerFuelOut(fuelOutPort);				//Flowmeter of the fuel in the engine return

HX711 			scale(7, 8);

void setup()
{
  Serial.begin(9600);
  configureThermocouples();
  configureInterruptions();
  scale.set_scale(2280.f);
  scale.tare();
  FreqMeasure.begin();//port for use in Arduino Mega is digital 49
}

void loop()
{
  verifyStartCommand();

  if (FreqMeasure.available())
  {
    frequencyReadSum += FreqMeasure.read();
    frequencyReadsCount ++;
  }

  if (canSend && myTimer())
  {

    sendPackage();
    lastTimeSended		= timeToSend;
    frequencyReadsCount	= 0;
    frequencyReadSum	= 0;

  }
}

void configureThermocouples() {

  pinMode(CLK, OUTPUT);
  pinMode(DBIT0, INPUT);
  pinMode(DBIT1, INPUT);
  pinMode(DBIT2, INPUT);
  pinMode(DBIT3, INPUT);
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  digitalWrite(CLK, LOW);

}

void configureInterruptions()
{
  attachInterrupt(1, &fluxometerFuelInCounter, FALLING);
  attachInterrupt(0, &fluxometerFuelOutCounter, FALLING);
}

void verifyStartCommand()
{
  if (Serial.available() > 0)
  {
    incomingFlag = (char)Serial.read();

    if (incomingFlag == '1' || incomingFlag == '2' || incomingFlag == '3')
    {
      timeOnStart				= (millis() / 1000);
      lastTimeSended			= 0;
      packageCounter 			= 0;
      frequencyReadSum		= 0;
      frequencyReadsCount		= 0;
      fluxometerFuelIn.initPulseCounter();
      fluxometerFuelOut.initPulseCounter();

      if (incomingFlag == '1')
      {
        scale.set_gain(64);
        functionSelector = 1;
      }
      else if (incomingFlag == '2')
      {
        scale.set_gain(64);
        functionSelector = 2;
      }
      else if (incomingFlag == '3')
      {
        scale.set_gain(32);
        functionSelector = 3;
      }

      Serial.println("MAC;CONT_PCT;FLUX_ENT;FLUX_SAIDA;TEMP_COMB_ENT;TEMP_COMB_SAIDA;TEMP_MOTOR;TEMP_LIQ_ARREF;CEL_CARGA;RADAR");
      delay(1000);

      canSend = true;
    }
    else if (incomingFlag == '0')
    {
      canSend = false;
    }
  }
}

bool myTimer()
{
  currentTime = (millis() / 1000) - timeOnStart;
  diffTime = currentTime - lastTimeSended;

  if (diffTime >= SEND_TIME)
  {
    timeToSend = currentTime;
    return true;
  }

  return false;
}

void sendPackage()
{
  String package;

  packageCounter ++;

  Serial.println(fillDataInPackage(package));
}

String fillDataInPackage(String package)
{
  float scaleValue = calculateScale(scale.get_units());;

  int frequency = ((int)(FreqMeasure.countToFrequency(frequencyReadSum / frequencyReadsCount))) * 0.0267;

  readThermocouples();

  package.concat(MAC);
  package.concat(";");
  package.concat(packageCounter);
  package.concat(";");
  package.concat(fluxometerFuelIn.getPulseCounter());
  package.concat(";");
  package.concat(fluxometerFuelOut.getPulseCounter());
  package.concat(";");
  package.concat(thermocoupleFuelIn);
  package.concat(";");
  package.concat(thermocoupleFuelOut);
  package.concat(";");
  package.concat(thermocoupleEngineOil);
  package.concat(";");
  package.concat(thermocoupleWatterCooler);
  package.concat(";");
  package.concat(scaleValue);
  package.concat(";");
  package.concat(frequency);

  return package;

}

void fluxometerFuelInCounter()
{
  fluxometerFuelIn.increasePulseCounter();
}

void fluxometerFuelOutCounter()
{
  fluxometerFuelOut.increasePulseCounter();
}

void readThermocouples() {

  thermocoupleFuelIn			= 0;
  thermocoupleFuelOut			= 0;
  thermocoupleEngineOil		= 0;
  thermocoupleWatterCooler	= 0;

  digitalWrite(CS, LOW);
  delay(2);
  digitalWrite(CS, HIGH);
  delay(220);
  digitalWrite(CS, LOW);
  digitalWrite(CLK, HIGH);
  delay(1);
  digitalWrite(CLK, LOW);
  for (int i = 14; i >= 0; i--) {
    digitalWrite(CLK, HIGH);
    thermocoupleFuelIn += digitalRead(DBIT0) << i;
    thermocoupleFuelOut += digitalRead(DBIT1) << i;
    thermocoupleEngineOil += digitalRead(DBIT2) << i;
    thermocoupleWatterCooler += digitalRead(DBIT3) << i;
    digitalWrite(CLK, LOW);
  }

}

float calculateScale(float scaleValue) {

  if (functionSelector == 1)
  {
    return (20.899 * scaleValue) + 8.14; //scaleValue/0.097;
  }
  else if (functionSelector == 2)
  {
    return (20.764 * scaleValue) + 57.4; //scaleValue/0.0479;
  }
  else if (functionSelector == 3)
  {
    return (30.5 * scaleValue);
  }

  return 0;

}
