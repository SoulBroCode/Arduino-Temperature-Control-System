Temperature controller for Arduino. Takes 2 temperatures from thermometers and compare them to switch on and off a relay/output. There is a 3rd control temperature "MAX" that allow to you set the max temperature to switch off the relay/output. This library can be use to control relay to switch on and off fans. This allows you to control the max temperature in your room.

This use the Adafruit_SSD1306 library to display the to oled screen. The display temperatures "MAX", "IN" and "OUT". "MAX" is control temperature. "IN" is temperature in the room and "OUT" is the temperature outside. 

This use the EEPROM library to save the "MAX" temperature to the Arduino's chip ROM.
