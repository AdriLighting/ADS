#include "config_base.h"


#ifdef DHT_ENABLE
	// SONDE TEMPERATURE
	#include <Adafruit_Sensor.h>
	#include <DHT.h>
	#include <DHT_U.h>

	#include "meteo_dht.h"

	DHT_Unified dht(D1, DHT22);

	void dht_init(){
		dht.begin();

		Serial.println(F("DHTxx Unified Sensor Example"));
		// Print temperature sensor details.
		sensor_t sensor;
		dht.temperature().getSensor(&sensor);
		Serial.println(F("------------------------------------"));
		Serial.println(F("Temperature Sensor"));
		Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
		Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
		Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
		Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
		Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
		Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
		Serial.println(F("------------------------------------"));
		// Print humidity sensor details.
		dht.humidity().getSensor(&sensor);
		Serial.println(F("Humidity Sensor"));
		Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
		Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
		Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
		Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
		Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
		Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
		Serial.println(F("------------------------------------"));			
	}	
	void dht_loop(){
		sensors_event_t event;
		dht.temperature().getEvent(&event);
		if (isnan(event.temperature)) {
			Serial.println(F("Error reading temperature!"));
		}
		else {
			Serial.print(F("Temperature: "));
			Serial.print(event.temperature);
			Serial.println(F("°C"));
		}
		// Get humidity event and print its value.
		dht.humidity().getEvent(&event);
		if (isnan(event.relative_humidity)) {
			Serial.println(F("Error reading humidity!"));
		}
		else {
			Serial.print(F("Humidity: "));
			Serial.print(event.relative_humidity);
			Serial.println(F("%"));
		}		
	}	
	float dht_get_temperature() {
		sensors_event_t event;
		dht.temperature().getEvent(&event);
		return event.temperature;		
	}
	float dht_get_humidity() {
		sensors_event_t event;
		dht.humidity().getEvent(&event);
		return event.relative_humidity;		
	}		
#endif