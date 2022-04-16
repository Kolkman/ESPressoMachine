//
// ESPressIoT Controller for Espresso Machines
// (c) 2021 Olaf Kolkman
// Replaces senspr_max31855 (c) 2016-2021 by Roman Schmitz

// MAX31855 Sensor Arduino-MAX31855 Library

#ifndef SIMULATION_MODE
#include <Wire.h>
#include <SPI.h>
#include "sensor_max31855.h"

TempSensor::TempSensor() : Adafruit_MAX31855(SENSOR_MAX_CLK, SENSOR_MAX_CS, SENSOR_MAX_DO){

lastT = 0.0;
SumT = 0.0;
lastI = 0.0;
SumI = 0.0;

lastErr = 0.0;
CntT = 0;
CntI = 0;
lastSensTime=0;


}



void TempSensor::setupSensor()
{

  while (!Serial)
    delay(1); // wait for Serial on Leonardo/Zero, etc

  Serial.println("MAX31855 test");
  // wait for MAX chip to stabilize
  delay(500);
  Serial.print("Initializing sensor...");
  if (!begin())
  {
    Serial.println("ERROR.");
    while (1)
      delay(10);
  }
  Serial.println("DONE.");
}

void TempSensor::updateTempSensor(double sensorSampleInterval)
{

  time_now = millis();

  if ((max(time_now, lastSensTime) - min(time_now, lastSensTime)) >= sensorSampleInterval)
  {
    double i = readInternal();
    double c = readCelsius();
    if (isnan(c) || isnan(i))
    {
      Serial.println("Could not get read temperature, Something wrong with thermocouple!");
    }
    else
    {
      double curT = c;
      double curI = i;
      // very simple selection of noise hits/invalid values
      // the weed-out vallue is rather high, to low will cause runnaway
      // heating cycles.
      if (abs(curT - lastT) < 15 || lastT < 1)
      {
        SumT += curT;
        lastT = curT;
        CntT++;
      }
      if (abs(curI - lastI) < 15 || lastI < 1)
      {
        SumI += curI;
        lastI = curI;
        CntI++;
      }

      lastSensTime = millis();
    }
  }
}


float TempSensor::getTemp(float temp)
{
  float retVal = temp; //default to return
  if (CntT >= 1)
  {
    retVal = (SumT / CntT);
    SumT = 0.;
    CntT = 0;
  }

  return retVal;
}


/* - - -  Not Needed - - - - -
float TempSensor::getItemp()
{
  float retVal = 0;
  if (CntI >= 1)
  {
    retVal = (SumI / CntI);
    SumI = 0.;
    CntI = 0;
  }

  return retVal;
}
- - - - - - - - - - */



#endif
