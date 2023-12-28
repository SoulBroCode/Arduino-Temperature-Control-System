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
};


float TemperatureSensor::GetTempC()
{
  _DS18B20->select(_Address);

  float temp = _DS18B20->getTempC();
  
  return temp;
}

void TemperatureSensor::Print()
{
  _DS18B20->select(_Address);
  
  uint8_t address[8];
  _DS18B20->getAddress(address);
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

 
