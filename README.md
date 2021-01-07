# ADS
## Description 
> Local smart home - esp8266 - ws2812b, SK6812, Relay.<br>
> Smart home essentiellement créer pour l'éclairage led d'une maison.
### Le principe :
> Un module ou plusieurs module server, et un ou plusieurs module client.<br>
> Les modules seront tous connecter au même routeur WIFI et communiqueront ensemble via UDP.<br>
> Les modules server servirons d’interface(application Android, web-server, écran TFT) de liaison entre les modules client.

<br>
# Partie server
## /esp8266/server/
### Description 
> udp server piloted via android application
### Librairies 
- dowload to "\Users\You\Documents\Arduino\libraries"
### Librairies ADS
* ![DA_COMMON](https://github.com/AdriLighting/DA_COMMON)
### Librairies additionnelles by AdriLighting
* ![adri_espwebserver](https://github.com/AdriLighting/adri_espwebserver)
* ![adri_httparseurl](https://github.com/AdriLighting/adri_httparseurl)
* ![adri_wifiConnect](https://github.com/AdriLighting/adri_wifiConnect)
* ![adri_tools](https://github.com/AdriLighting/adri_tools)
* ![adri_ntpTime](https://github.com/AdriLighting/adri_ntpTime)
* ![adri_timer](https://github.com/AdriLighting/adri_timer)
### librairies additionnelles
* by Arduinojson
  * ![arduinojson](https://github.com/bblanchon/ArduinoJson)
  
<br>  

## /esp8266/server_oled /
### Description 
> udp server piloted via push-button and oled-display(128x64)
### Librairies ADS
* ![DA_COMMON](https://github.com/AdriLighting/DA_COMMON)
### Librairies additionnelles by AdriLighting
* ![adri_oled_menu_128x64](https://github.com/AdriLighting/adri_oled_menu_128x64)
* ![adri_calendar_event](https://github.com/AdriLighting/adri_calendar_event)
### librairies additionnelles
* by Arduinojson
  * ![arduinojson](https://github.com/bblanchon/ArduinoJson)
* by Adafruit 
  * ![ST7735](https://github.com/adafruit/Adafruit-ST7735-Library)
  * ![Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)  
  * ![DHT](https://github.com/adafruit/DHT-sensor-library)  
 * by squix78    
  * ![json-streaming-parser](https://github.com/squix78/json-streaming-parser)
 * by PaulStoffregen
  * ![TimeLib](https://github.com/PaulStoffregen/Time) 
