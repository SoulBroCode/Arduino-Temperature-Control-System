#include <DS18B20.h>

#include "TemperatureSensor.h"
#include "Button.h"

const float MAX_TEMP = 80;
const float MIN_TEMP = -20;
float _ChangeableTemp = 30; 
float _InsideTemp = 0;
float _OutsideTemp = 0;

uint8_t _TemperatureSensorCount = 0;
DS18B20 _DS18B20(8);
TemperatureSensor** _TemperatureSensors;


//Loop State
enum State{
  Normal,
  Edit
};

State _State(Normal);

Button _UpBtn(2);
Button _DownBtn(4);
Button _EditBtn(7);

//Fan Relay Condition
const int FAN_RELAY_PIN = 11;
enum FanState{
  On,
  Off
};

FanState _FanState(Off);
FanState _NewFanState(Off);
float FAN_SWITCHABLE_COUNTER = 3; //15 seconds
float _FanTimerCounter = 3;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.print(F("------- Starting --------"));
  Serial.println();

  //Sensor
   _TemperatureSensorCount = _DS18B20.getNumberOfDevices();
  _TemperatureSensors =  new TemperatureSensor*[_TemperatureSensorCount];

  Serial.print(F("Devices : ")); 
  Serial.print(_TemperatureSensorCount);  
  Serial.println();

  for(int i = 0; i < _TemperatureSensorCount; i++)
  {
    _TemperatureSensors[i] = new TemperatureSensor(i, _DS18B20);
  }
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
  
 
  _InsideTemp = _TemperatureSensors[TemperatureSensor::Inside]->GetTempC();
  _OutsideTemp = _TemperatureSensors[TemperatureSensor::Outside]->GetTempC();

  if(_InsideTemp < _ChangeableTemp)
  {
    if(_OutsideTemp > _InsideTemp){
      _NewFanState = On;
      //Serial.print(F("Turning ON fans"));
    }
    else{
      _NewFanState = Off;
      //Serial.print(F("Turning OFF fans"));
    }
  }
  else{
    _NewFanState = Off;
    //Serial.print(F("Turning OFF fans"));
  }

  if(_FanState != _NewFanState && _FanTimerCounter >= FAN_SWITCHABLE_COUNTER){
    if(_NewFanState == On){
      TurningOnFan();
    }
    else{
      TurningOffFan();
    }
    _FanState = _NewFanState;
  }
  else if(_FanTimerCounter < FAN_SWITCHABLE_COUNTER){
    _FanTimerCounter += 1;
  }

  if(_EditBtn.IsOn()){
    Serial.println();
    Serial.println(F("Enable Editing"));
    _State = Edit;
    TurningOffFan();
  }

  delay(5000);
}


void TurningOnFan(){ 
  Serial.print(F("Turning ON fans"));
  _FanTimerCounter = 0;
  _FanState = On;
  analogWrite(FAN_RELAY_PIN, 200);
}

void TurningOffFan(){
  Serial.print(F("Turning OFF fans"));
  _FanTimerCounter = 0;
  _FanState = Off;
  analogWrite(FAN_RELAY_PIN, 0);
}

void EditLoop(){

  if(_UpBtn.IsOn())
    {
      if(_ChangeableTemp < MAX_TEMP)
      {
        _ChangeableTemp += 0.5;
      }
      Serial.print(F("Changeable Temp : "));
      Serial.println(_ChangeableTemp);
    }
    if(_DownBtn.IsOn())
    {
      if(_ChangeableTemp > MIN_TEMP)
      {
        _ChangeableTemp -= 0.5;
      }
      Serial.print(F("Changeable Temp : "));
      Serial.println(_ChangeableTemp);
    }


    if(_EditBtn.IsOn())
    {
      Serial.println();
      Serial.println(F("Finish Editing"));
      _State = Normal;
    }

    if(_FanTimerCounter < FAN_SWITCHABLE_COUNTER)
    {
      _FanTimerCounter += 0.1;
    }
    delay(500);
}










