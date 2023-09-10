#include "Arduino.h"
#include <DS18B20.h>
#include "TemperatureSensor.h"


TemperatureSensor::TemperatureSensor(int index, DS18B20 &ref){
   ref.selectNext();
   ref.getAddress(_Address);

   if(index < Type::Length)
      _Type = static_cast<Type>(index);
   else 
    _Type = Type::Unknown;

    _DS18B20 = &ref;

    Serial.print(F("Temperture Sensor : "));
    Serial.print(TemperatureSensor::TypeToString(_Type));
    Serial.println();
};


float TemperatureSensor::GetTempC()
{
  _DS18B20->select(_Address);

  float temp = _DS18B20->getTempC();

  Serial.print(TemperatureSensor::TypeToString(_Type));
  Serial.print(F(" Temperature: "));
  Serial.print(temp);
  Serial.print(F(" C / "));
  Serial.println();

  return temp;
}