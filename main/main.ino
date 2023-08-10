#include "Button.h"

Button _UpBtn(2);
Button _DownBtn(4);
Button _EditBtn(7);

enum FanState{
  On,
  Off
};

FanState _FanState(Off);
FanState _NewFanState(Off);

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.print(F("------- Starting --------"));
  Serial.println();
}

void loop() {
  delay(3000);

  if(_State == Normal)
  {
    NormalLoop();
  }
  else
  {
    EditLoop();
  }
}

void NormalLoop(){
  Serial.println();
  Serial.println(F("-------------------"));
} 

void EditLoop(){

  if(_UpBtn.IsOn())
    {
      Serial.print(F("Press Up"));
    }
    if(_DownBtn.IsOn())
    {
      Serial.print(F("Press Down"));
    }

    if(_EditBtn.IsOn())
    {
      Serial.println();
      Serial.println(F("Finish Editing"));
      _State = Normal;
    }

    delay(500);
}









