#include "Button.h"

Button::Button(int pin){
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);

  _Pin = pin;
}

bool Button::IsOn()
{
   return (digitalRead(_Pin) == LOW);
}
