----------------------------------------------------------------------------------------------------------------------------------

Temperature control system. (For solar hot air heater, can also be use for other applications)

----------------------------------------------------------------------------------------------------------------------------------

Require electronic
<br>-x1 Arduino
<br>-x2 db18b20 thermometers or other equivalence
<br>-4.7k resistor (for thermometer, using parasite power)
<br>-x1 led
<br>-x3 buttons

Require libs
<br>Adafruit_SSD1306 (for oled display)
<br>DS18B20

Desc
<br>Takes 2 thermometers readings to compare, output to switch on relay/applications(fans etc...). Comparsion variables are saved using EEPROM.

----------------------------------------------------------------------------------------------------------------------------------

Control 
<br>(Stop output if)
<br>-temp (inside) greater than temp (MAX)
<br>-temp (inside) and temp (offset) greater than temp (outside)

(Start output if)
<br>-temp (outside) greater than temp (inside) and temp (Diff)

----------------------------------------------------------------------------------------------------------------------------------

Settings (Adjustable)
<br>-hold button one for 1-5sec for edit option to display on oled.

<br>-hold button two to increment MAX
<br>-hold button two to decrement MAX

<br>-hold button one, two to increment Dif
<br>-hold button one, two to decrement Dif

<br>-hold button one, two, three to increment Off
<br>-hold button two, three to decrement Off

<br>-hold button one to exit and save.

----------------------------------------------------------------------------------------------------------------------------------

Abbreviation for Oled display
<br>-In = inside
<br>-Out = outside
<br>-MAX = max temp
<br>-Dif = temp (diff) is the difference from the temp (outside) to the temp (inside) before starting output. (energy saver and stop constant switching)
<br>-Off = temp (offset) is the offset (energy saver and stop constant switching)

----------------------------------------------------------------------------------------------------------------------------------
