#ifndef Button_h
#define Button_h

#include "Arduino.h"

class Button {
  public:   
    Button(int pin);
    bool IsOn();
    
  private:
    int _Pin;
};
#endif