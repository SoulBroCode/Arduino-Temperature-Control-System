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

const int LED_PIN = 10;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
  delay(3000);

  Serial.begin(9600);
  Serial.println();
  Serial.print(F("------- Starting --------"));
  Serial.println();

  //Loading Data
  EEPROM.get(CHANGEABLE_TEMP_ADDRESS, _ChangeableTempData);
  if(!isnan(_ChangeableTempData)){
      _ChangeableTemp = _ChangeableTempData;
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

  Serial.print(F("Devices : ")); 
  Serial.print(_TemperatureSensorCount);  
  Serial.println();

  for(int i = 0; i < _TemperatureSensorCount; i++)
  {
    _TemperatureSensors[i] = new TemperatureSensor(i, _DS18B20);
  }

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
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


//----------------------------
  //Potentiometer
  // int readValue = analogRead(POT_PIN);

  // Serial.print(F("readValuee : "));  
  // Serial.println(readValue);

  // int mapValue = map(readValue,0, 1023, 0, 255);
  // Serial.print(F("mapValue : "));  
  // Serial.println(mapValue);
  
  // analogWrite(MOSTFET_PIN, mapValue); //102

  // delay(500);
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
    Serial.println();
    Serial.println(F("Enable Editing"));
    _State = Edit;
    TurningOffFan();
    EditDisplay();   
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
  display.display();
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

    EditDisplay();

    if(_EditBtn.IsOn())
    {
      Serial.println();
      Serial.println(F("Finish Editing"));
      _State = Normal;
      NormalDisplay();
      SavingToMemory();
    }

    if(_FanTimerCounter < FAN_SWITCHABLE_COUNTER)
    {
      _FanTimerCounter += 0.1;
    }
    delay(500);
}

void SavingToMemory()
{
  if (_ChangeableTemp != _ChangeableTempData)
  {
    Serial.println(F("Saving to memory"));
    EEPROM.put(CHANGEABLE_TEMP_ADDRESS, _ChangeableTemp);
  }
}


void EditDisplay(void) {

  display.clearDisplay();

  display.setTextSize(2);   
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner

  display.print(F("Editing..."));
  display.println(F(" "));
  display.print(F("MAX: "));
  display.print(_ChangeableTemp);
  display.display();
}