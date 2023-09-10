#ifndef TemperatureSensor_h
#define TemperatureSensor_h

#include "Arduino.h"
#include <DS18B20.h>

#ifndef ONEWIRE_SEARCH
#define ONEWIRE_SEARCH 0
#endif


class TemperatureSensor {
  public:   
    enum Type {
      Outside,    // 0
      Inside,
      Unknown,
      Length
    };

    TemperatureSensor(int index, DS18B20 &ref);

    static String TypeToString(Type type);
  private:
     DS18B20* _DS18B20;

     Type _Type;
     uint8_t _Address[8];
};
#endif