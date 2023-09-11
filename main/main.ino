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
  if(_TemperatureSensorCount < TemperatureSensor::Unknown)
  {
    Serial.println();
    Serial.println(F("-------------------"));
    Serial.println(F("Error : Missing Sensor"));

    display.clearDisplay();

    display.setTextSize(2);   
    display.setTextColor(SSD1306_WHITE);       
    display.setCursor(0,0); 

    display.print(F("Error"));
    display.println();
    display.print(F("Missing"));
    display.println();
    display.print(F("Sensor"));
    display.display();

    delay(5000);

    return;
  } 

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









