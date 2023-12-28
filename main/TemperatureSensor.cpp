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

    // Serial.print(F("Temperture Sensor : "));
    // Serial.print(TemperatureSensor::TypeToString(_Type));
    // Serial.println();
};


float TemperatureSensor::GetTempC()
{
  _DS18B20->select(_Address);

  float temp = _DS18B20->getTempC();

  // Serial.print(TemperatureSensor::TypeToString(_Type));
  // Serial.print(F(" Temperature: "));
  // Serial.print(temp);
  // Serial.print(F(" C / "));
  // Serial.println();

  return temp;
}

void TemperatureSensor::Print()
{
  _DS18B20->select(_Address);
  
  uint8_t address[8];
  _DS18B20->getAddress(address);
  Serial.print(F("Address:"));
  for (uint8_t i = 0; i < 8; i++) {
    // Serial.print(F(" "));
    // Serial.print(address[i]);
  }
  Serial.println();

  Serial.print(F("Power Mode: "));
  if (_DS18B20->getPowerMode()) {
    // Serial.println(F("External"));
  } else {
    // Serial.println(F("Parasite"));
  }

  // Serial.print(F("Temperature: "));
  // Serial.print(_DS18B20->getTempC());
  // Serial.print(F(" C / "));
  // Serial.print(_DS18B20->getTempF());
  // Serial.println(F(" F"));
  // Serial.println();
}


String TemperatureSensor::TypeToString(Type type)
 {
    switch (type) {
      case Outside:
        return F("Outside");
      case Inside:
        return F("Inside");
      default:
        return F("UNKNOWN");
    }
 }

 
