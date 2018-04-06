/* 
 AutomationShield.cpp
  Arduino library for teaching control engineering.
  Authors: Tibor Konkoly, Gabor Penzinger, [...], and Gergely Takacs
  2017-2018.
  Released into the public domain.
  Last change by Tibor Konkoly on 06.04.2018 at 21:25 .
*/


#include "AutomationShield.h"
#include "Arduino.h"

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min; // linear mapping, same as Arduino map()
}

// AutomationShield

AutomationShield::AutomationShield(){
  
}

// Opto

Opto::Opto(){
  
}

void Opto::calibration(){                           // a function determines the maximal and minimal value of the LDR, which is crutial for accurate measurements
  Serial.println("Calibration is running...");
  delay(3000);                                      // we start with a 3s delay, which is needed for the value of LDR to get steady
  
  _minVal = analogRead(OPTO_YPIN);             // determining the minimal value

  analogWrite(OPTO_UPIN,255);                        // determining the maximal value
  delay(3000);
  _maxVal = analogRead(OPTO_YPIN);

  analogWrite(OPTO_UPIN,0);                          // this line switches off the LEDs

  indicator = true;

  // summary
  Serial.println("_______________________________________");
  Serial.println("__________CALIBRATION RESULTS__________");
  Serial.print("Minimal value of the LDR is: ");
  Serial.println(_minVal);
  Serial.print("Maximal value of the LDR is: ");
  Serial.println(_maxVal);
  Serial.println("_______________________________________"); 
} // end of calibration



void Opto::begin(void){                  // begin function initializes the pins used by the hardware. Optoshield uses three pins, pin A1 is used by the LDR,                                          //pin A0 is connected to the runner of the potentiometer and digital pin 3 is for setting the intensity of the leds' light                                            
  pinMode(OPTO_YPIN, INPUT);             // analog pin A2 is connected to the auxiliary LDR
  pinMode(OPTO_UPIN, OUTPUT);
  pinMode(OPTO_RPIN, INPUT); 
  pinMode(OPTO_YAUX, INPUT);
}


void Opto::actuatorWrite(float value){          // actuatorWrite() sets the LEDs' brightness, using a floating point number from 0 (min) to 100 (max)
  _convertedValue = mapFloat(value,0.00,100.00,0.00,255.00);
  analogWrite(OPTO_UPIN,_convertedValue); 
}


float Opto::sensorRead(){
   _sensorRead = analogRead(OPTO_YPIN);
   
   if(indicator){                                                   // with an if statement I can check, if the calibration function was called (recommended)
   _sensorValue = mapFloat(_sensorRead, _minVal, _maxVal, 0.00, 100.00); 
   }
   else{
   _sensorValue = mapFloat(_sensorRead, 515.00, 1000.00, 0.00, 100.00); // when the calibration was not called we use static values, which were the most common values during the tests
   }
  return _sensorValue;
}


float Opto::sensorReadVoltage(){   // sensorReadVoltage returns the Voltage on the LDR in the tube
  float k = (5.00 / 1023.00);     // constant that allows us to convert the analog values from the sensor into voltage 
  _valueRead = analogRead(OPTO_YPIN);
  _sensorVoltage = _valueRead * k;
  return _sensorVoltage;  
}


float Opto::sensorAuxRead(){      // sensorAuxRead returns the Voltage on the auxiliary LDR as a floating point number
 float k = (5.00 / 1023.00);      // constant that allows us to convert the analog values from the sensor into voltage 
  _auxRead = analogRead(OPTO_YAUX);
  _auxVoltage = _auxRead * k;
  return _auxVoltage;  
}



float Opto::referenceRead(){           // referenceRead function returns the reference value of the potentiometer, which was set by the user, in percents
   _referenceRead = analogRead(OPTO_RPIN);
   _referenceValue = mapFloat(_referenceRead,0.00,1023.00,0.00,100.00);
  return _referenceValue;
}


