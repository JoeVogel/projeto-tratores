#include <LiquidCrystal.h>
#include "Wire.h"
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
*   DuLagernkel Box source code is free software: you can redistribute it and/or modify
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


DHT dht(26, DHT22);

Adafruit_BMP085 bmp;
int portNoiseSensor=8;

int cont=0;
int tempPackage = 1000;

float valueNoise;

float pressure;

float altitude;

float temperature;

float umidade;



void setup() 
{
  Serial.begin(9600);
  bmp.begin();  
  dht.begin();
  
}

void loop() 
{
    lerSensorsNoise();    
    cont++;   

    if(cont >= tempPackage){
      lerBarometro();
      lerSensorHumidity();
      imprimir();      
      imprimirLcd();
      zerarContadores();
      
      
    }
}

void imprimirLcd(void){
  LiquidCrystal lcd(13,12,11,10,9,8);  // MEGA
  lcd.begin(20, 4); 
  
  lcd.setCursor(0,0);
  lcd.print("T:");  
  lcd.print(millis()/1000);
  lcd.setCursor(7,0);           
  lcd.print("dunkel");
  
}

void lerSensorsNoise(){
  float atualNoise= analogRead(portNoiseSensor);
  
  if(atualNoise > valueNoise){
    valueNoise= atualNoise;  
  }
  
}

void lerSensorHumidity(){
  umidade = dht.readHumidity();
  temperature = dht.readTemperature();
  
}

void lerBarometro(){  
  pressure = bmp.readPressure();
  altitude = bmp.readAltitude();
}

void zerarContadores(){
  cont=0;
  valueNoise=0;
  
}

void imprimir(){
  
  Serial.print("Ruido:");
  Serial.println(valueNoise);  
  Serial.print("Pressao = ");  
  Serial.print(pressure);  
  Serial.println(" Pa");
  Serial.print("Altitude = ");
  Serial.print(altitude);  
  Serial.println(" metros");
  Serial.print("Temperatura = ");    
  Serial.print(temperature);
  Serial.println(" *C");  
  Serial.print("umidade");
  Serial.println(umidade);    
  

}
