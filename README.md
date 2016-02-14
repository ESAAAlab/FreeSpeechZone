# Free Speech Zone*
Wifi-Connected Thermal Printer, censoring everything people say.


#### Introduction
After finding a Bixolon STP-103 Thermal Printer in a trash, adding an ESP8266 module and using node and a MQTT server, this project allows people to send their messages to the printer via [this page](http://www.blaste.net/expression).
All the words in a message have a 50% chance of being censored.

#### Why ?
Free speech is on the way out & anonymity doesn't existe anymore. However, people are still creating rumors, telling lies or they simply need a space to vent, express their thought or make dirty jokes without risks.
This printer just prints whatever is posted on a web page, no connection informations are stored and no user account is needed. However, some words will be blacked out, randomly.
What is the purpose of censorship if everything might be offensive ?

#### Installation
The printer should be installed in a walkway or any other space with lots of people walking by. A poster, found in the docs folder, should be printed and glued next to the printer. The roll of printouts should be left hanging, if possible over a railing to an inferior floor and should never be torn out.

#### Prerequisite
- A thermal printer : [The STP-103](https://www.intelligentwt.com/v/vspfiles/files/manuals/user-manuals/stp103-usermanual.pdf) seems to use a serial protocol not too far from the [A2 printers you can find on Adafruit](https://www.adafruit.com/products/597).


- ESP8266 module : I used an ESP-12 on a breakout board, it should work with other modules with minimum fiddling.
![ESP12](http://blog.hekkers.net/wp-content/uploads/2015/03/ESP-8266-12.jpg)

- Voltage regulation : You can find +5v and GND on the voltage regulators inside the printer. I used a LD1117v33 according to this schematic.

![VReg](http://people.ece.cornell.edu/land/courses/ece4760/FinalProjects/s2009/gt62_mrs67/gt62_mrs67/voltage_reg.jpg)

- Level shifter : The ESP8266 works with 3.3v, the printer with 5v. Bidirectionnal communication is not strictly necessary for this project, so I just used this circuit on the ESP8266 SoftwareSerial tx line and left the rx unconnected.

![level shifter](http://i.stack.imgur.com/TS4Td.png)

- MQTT broker : Since I didn't want to host my own server, I opted for [CloudMQTT's free plan](https://www.cloudmqtt.com/) which allows up to 10 connections. You can however use any broker you want.

- node server : If you need to get your page hosted on a node server, I can recommend [Gandi's SimpleHosting](https://www.gandi.net/hebergement/simple?language=nodejs&db=pgsql).
