
#ifndef METEO_H
	#define METEO_H
	#include "meteo_weatherMapCurrent.h"


	extern OpenWeatherMapCurrentData 	currentWeather;
	void openWather_updateData();
	void openWather_loop();

#endif