# AdriLighting Domotic Server
### Description 
> Local smart home - esp8266 - ws2812b, SK6812, Relay.<br>
> Smart home essentiellement créer pour l'éclairage led d'une maison.
### Fonctionnement
> Un module ou plusieurs module server, et un ou plusieurs module client.<br>
> Les modules seront tous connecter au même routeur WIFI et communiqueront ensemble via UDP.<br>
> Les modules server servirons d’interface(application Android, web-server, écran TFT) de liaison entre les modules client.

<hr>
<pre>
Client.
    Envoie un message par udpMulticast tous les 3sec pour signaler leur état.
    Envoie un message par udpMulticast a récéption d'une requête udpMulticast, http, socket.
</pre>

<hr>
<pre>
Server.
    Réception udpMulticast
        met a jour les données dynamique, sauvegarde le module dans un fichier SPIFF
    Réception udp
        - a
</pre>

<br>
<hr>
<br>

## [/esp8266/server/](https://github.com/AdriLighting/ADS/tree/main/esp8266/server)
### Description 
> updated server librairie, work in progress...<br>
> udp server piloted via android application

### Librairies 
- dowload to "\Users\You\Documents\Arduino\libraries"
#### Librairies ADS
* ![DA_COMMON](https://github.com/AdriLighting/DA_COMMON)
#### Librairies additionnelles
* ![adri_espwebserver](https://github.com/AdriLighting/adri_espwebserver)
* ![adri_httparseurl](https://github.com/AdriLighting/adri_httparseurl)
* ![adri_wifiConnect](https://github.com/AdriLighting/adri_wifiConnect)
* ![adri_tools](https://github.com/AdriLighting/adri_tools)
* ![adri_ntpTime](https://github.com/AdriLighting/adri_ntpTime)
* ![adri_timer](https://github.com/AdriLighting/adri_timer)
#### librairies additionnelles
* by Arduinojson
  * ![arduinojson](https://github.com/bblanchon/ArduinoJson)
#### Application Android
* MIT APP Inventor
  * ![aia](https://github.com/AdriLighting/ADS/blob/main/android/Dadri_Server_4.aia)
  * ![apk](https://github.com/AdriLighting/ADS/blob/main/android/Dadri_Server_4.apk)
  
</br>
<hr>

## [/esp8266/client_2021/](https://github.com/AdriLighting/ADS/tree/main/esp8266/client_2021)
### Description 
> updated client librairie, work in progress....<br>
> udp client for driving led-strip or relay.<br>
> can be piloted by own server or by commmun server.
### Librairies 
- dowload to "\Users\You\Documents\Arduino\libraries"
  
</br>
<hr>

## [/esp8266/server_oled /](https://github.com/AdriLighting/ADS/tree/main/esp8266/server_oled)
### Description 
> udp server piloted via push-button and oled-display(128x64)
### Librairies 
- dowload to "\Users\You\Documents\Arduino\libraries"
#### Librairies ADS
* ![DA_COMMON](https://github.com/AdriLighting/DA_COMMON)
#### Librairies additionnelles
* ![adri_oled_menu_128x64](https://github.com/AdriLighting/adri_oled_menu_128x64)
* ![adri_calendar_event](https://github.com/AdriLighting/adri_calendar_event)
#### librairies additionnelles
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
  
</br>
<hr>

## [/esp8266/client/](https://github.com/AdriLighting/ADS/tree/main/esp8266/client)
### Description 
> udp client for driving led-strip or relay.<br>
> can be piloted by own server or by commmun server.
### Librairies 
- dowload to "\Users\You\Documents\Arduino\libraries"
#### Librairies ADS
* ![DA_COMMON](https://github.com/AdriLighting/DA_COMMON)
#### librairies additionnelles
* by kriegsman
  * ![FastLED](https://github.com/FastLED/FastLED)
* by Adafruit
  * ![Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
* by me-no-dev
  * ![ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)  
* by PaulStoffregen
  * ![TimeLib](https://github.com/PaulStoffregen/Time)  
