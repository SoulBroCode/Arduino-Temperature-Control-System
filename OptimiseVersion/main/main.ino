#include <DS18B20.h>

#include "TemperatureSensor.h"
#include "Button.h"

//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels 128
#define SCREEN_HEIGHT 64 // OLED display height, in pixels 64


#define OLED_RESET     -1 
#define SCREEN_ADDRESS  0x3C


#ifndef ONEWIRE_SEARCH
#define ONEWIRE_SEARCH 0
#endif


//Saving
const int CHANGEABLE_TEMP_ADDRESS = 0;
float _ChangeableTempData;

const int DIFF_TEMP_ADDRESS = 50;
float _DiffTempData;

const int OFFSET_TEMP_ADDRESS = 100;
float _OffsetTempData;


const int LED_PIN = 10;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const float MAX_TEMP = 80;
const float MIN_TEMP = -20;
float _ChangeableTemp = 30; 
float _DiffTemp = 2;
float _OffsetTemp = 1;
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
  delay(3000);

  //Loading Data
  EEPROM.get(CHANGEABLE_TEMP_ADDRESS, _ChangeableTempData);
  if(!isnan(_ChangeableTempData)){
      _ChangeableTemp = _ChangeableTempData;
  } 

  EEPROM.get(DIFF_TEMP_ADDRESS, _DiffTempData);
  if(!isnan(_DiffTempData)){
      _DiffTemp = _DiffTempData;
  } 

  EEPROM.get(OFFSET_TEMP_ADDRESS, _OffsetTempData);
  if(!isnan(_OffsetTempData)){
      _OffsetTemp = _OffsetTempData;
  } 



  //red led on arudino
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); 
 
  //Red Led on box 
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 120);

 //Sensor
   _TemperatureSensorCount = _DS18B20.getNumberOfDevices();
  _TemperatureSensors =  new TemperatureSensor*[_TemperatureSensorCount];

  for(int i = 0; i < _TemperatureSensorCount; i++)
  {
    _TemperatureSensors[i] = new TemperatureSensor(i, _DS18B20);
  }

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for(;;); // Don't proceed, loop forever
  }

  //Relay Fan pins
  pinMode(FAN_RELAY_PIN, OUTPUT);
}

void loop() {
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
 
  _InsideTemp = _TemperatureSensors[TemperatureSensor::Inside]->GetTempC();
  _OutsideTemp = _TemperatureSensors[TemperatureSensor::Outside]->GetTempC();

  if(_NewFanState == On){
    if(_InsideTemp >= _ChangeableTemp || _InsideTemp + _OffsetTemp > _OutsideTemp){
      _NewFanState = Off;
    }
  }
  else
  {
    if(_OutsideTemp > _InsideTemp + _DiffTemp){
      _NewFanState = On;
    }
  }

  NormalDisplay();

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
    _State = Edit;
    _NewFanState = Off;
    TurningOffFan();
    EditDisplay();   
  }

  delay(5000);
}

void TurningOnFan(){ 
  _FanTimerCounter = 0;
  _FanState = On;
  analogWrite(FAN_RELAY_PIN, 200);
}

void TurningOffFan(){
  _FanTimerCounter = 0;
  _FanState = Off;
  analogWrite(FAN_RELAY_PIN, 0);
}


void NormalDisplay(void) {
  display.clearDisplay();

  display.setTextSize(2);   
  display.setTextColor(SSD1306_WHITE);       
  display.setCursor(0,0); 

  display.print(F("In: "));
  display.print(_InsideTemp);
  display.println();
  display.print(F("Out: "));
  display.print(_OutsideTemp);
  display.println();
  display.print(F("MAX: "));
  display.print(_ChangeableTemp);
  display.println();
  display.print(F("Dif: "));
  display.print(_DiffTemp);
  display.display();
}




void EditLoop(){

  if(_EditBtn.IsOn() && _UpBtn.IsOn() && _DownBtn.IsOn())
  {
    if(_OffsetTemp < MAX_TEMP){
      _OffsetTemp += 0.1;
    }
    EditDisplay();
  }
  else if(_UpBtn.IsOn() && _DownBtn.IsOn())
  {
    if(_OffsetTemp > MIN_TEMP){
      _OffsetTemp -= 0.1;
    }
    EditDisplay();
  }
  else if(_EditBtn.IsOn() && _UpBtn.IsOn())
  {
    if(_DiffTemp < MAX_TEMP){
      _DiffTemp += 0.1;
    }
    EditDisplay();
  }
  else if(_EditBtn.IsOn() && _DownBtn.IsOn())
  {
    if(_DiffTemp > MIN_TEMP){
      _DiffTemp -= 0.1;
    }
    EditDisplay();
  }
  else if(_UpBtn.IsOn())
  {
    if(_ChangeableTemp < MAX_TEMP){
      _ChangeableTemp += 0.5;
    }
    EditDisplay();
  }
  else if(_DownBtn.IsOn())
  {
    if(_ChangeableTemp > MIN_TEMP){
      _ChangeableTemp -= 0.5;
    }
    EditDisplay();
  }
  else if(_EditBtn.IsOn())
  {
    _State = Normal;
    NormalDisplay();
    SavingToMemory();
  }

  if(_FanTimerCounter < FAN_SWITCHABLE_COUNTER){
    _FanTimerCounter += 0.1;
  }
  delay(500);
}

void SavingToMemory()
{
  if (_ChangeableTemp != _ChangeableTempData){
    EEPROM.put(CHANGEABLE_TEMP_ADDRESS, _ChangeableTemp);
    _ChangeableTempData = _ChangeableTemp;
  }
  if (_DiffTemp != _DiffTempData){
    EEPROM.put(DIFF_TEMP_ADDRESS, _DiffTemp);
    _DiffTempData = _DiffTemp;
  }
  if (_OffsetTemp != _OffsetTempData){
    EEPROM.put(OFFSET_TEMP_ADDRESS, _OffsetTemp);
    _OffsetTempData = _OffsetTemp;
  }
}


void EditDisplay(void) {

  display.clearDisplay();

  display.setTextSize(2);   
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner

  display.print(F("Editing..."));
  display.println();
  display.print(F("Off: "));
  display.print(_OffsetTemp);
  display.println();
  display.print(F("MAX: "));
  display.print(_ChangeableTemp);
  display.println();
  display.print(F("Dif: "));
  display.print(_DiffTemp);
  
  display.display();
}