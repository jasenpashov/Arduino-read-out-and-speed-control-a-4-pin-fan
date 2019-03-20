# Arduino-read-out-and-speed-control-a-4-pin-fan
 Speed control of computer fan 4pin with arduino NodeMCU V3.

The project is a basic idea, what can be done with hand tools and as cheap as possible. This is a single-fan filter, in the stores the prices of these devices are very high. I looked at several projects until I make my idea. I decided to buy a filter, not to make a filter (maybe I will make a filter in the next version). I'm using a P1 HEPA system filter. For the fan I use - Arctic 120mm PWM. I decided to do a control with NodeMCU - Arduino. My idea was to make an hourly schedule, when to blow strong and weak. I also added a temperature sensor, as there were free pins on the board. I am also waiting for a Dust Sensor PPD42NJ to see if there is a result of everything I did. The biggest problem is the management of the PWM FAN. It took me a long time to make it work and and also a long time to make it work hard.
 
Materials used:

Prices are approximate and by memory.
 
Arctic 120mm P12 PWM - ACFAN00119A  - 5 €

3.3V 5V 12V Multi Output Voltage - 0.50€

https://www.aliexpress.com/item/3-3V-5V-12V-Multi-Output-Voltage-Conversion-DC-DC-12V-to-3-3V-5V-12V/32827889813.html?spm=a2g0s.9042311.0.0.66414c4dJxJCH4 
 
NodeMCU V3 Lua  - 3€

https://www.aliexpress.com/item/NodeMCU-V3-Lua-WIFI-Module-Integration-Of-ESP8266-Extra-Memory-32M-Flash-USB-serial-CH340G/32843773344.html?spm=a2g0s.9042311.0.0.27424c4dLNejor
 
DHT22/DHT11 Digital Temperature and Humidity - 2€
 
Rresistor pack - 0.0001€ок
 
12V 2A Power Adapter, for Router (dead router ) - free 

I have not tried working with other fans.

What I understood based on my practices and on of many attempts
- The NodeMCU and the fan must have a common negative pole (-).
- I can not achieve the maximum fan speed.
- The system operates unstable without a 560R resistor. I am not sure whether it is OK with other values.
 
Sources:

https://handyman.dulare.com/client-server-communication-using-esp8266/

https://codebender.cc/sketch:177208#PWM_Fan.ino

http://forum.arduino.cc/index.php?topic=18742.15

http://www.beefrankly.org/blog/2011/12/21/read-out-4-pin-cpu-fan-speed/

https://www.picmicrolab.com/arduino-pwm-fan-controller/

http://www.theorycircuit.com/reading-dc-fan-rpm-arduino/

https://www.avrfreaks.net/forum/control-pwm-pc-12v-fan-speed

https://www.instructables.com/id/OPEN-SOURCE-AIR-PURIFIER/  
