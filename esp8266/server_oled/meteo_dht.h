
#ifndef METEO_DHT_H
	#define METEO_DHT_H
	// #include <Adafruit_Sensor.h>
	#include <DHT.h>
	// #include <DHT_U.h>
	// extern DHT_Unified dht; 
	// #include <SimpleDHT.h>
	// SimpleDHT22 dht22;	
	void 	dht_init();
	void 	dht_loop();
	float 	dht_get_temperature();
	float 	dht_get_humidity();
#endif