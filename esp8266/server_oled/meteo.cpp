
#include "config_base.h"
	
#ifdef OW_METEO
	#include "meteo_weatherMapCurrent.h"
	#include "WeatherStationFonts.h"
	#include "WeatherStationImages.h"
	#include "meteo.h"
	#include "tft_display.h"

	String OPEN_WEATHER_MAP_APP_ID = "603ef62761eb23b76ac8c8af7711776f";
	String OPEN_WEATHER_MAP_LOCATION_ID = "2972214";
	String OPEN_WEATHER_MAP_LANGUAGE = "fr";
	const boolean OPEN_WEATHER_IS_METRIC = true;

	const int OPEN_WEATHER_UPDATE_INTERVAL_SECS = 20 * 60; // Update every 20 minutes

	bool openWeather_readyForWeatherUpdate = false;

	long openWeather_timeSinceLastWUpdate = 0;

	OpenWeatherMapCurrentData currentWeather;
	OpenWeatherMapCurrent currentWeatherClient;

void openWather_show() {
	String temp = String(currentWeather.temp, 1) + (OPEN_WEATHER_IS_METRIC ? "°C" : "°F");
	Serial.printf("temp: %s\n", temp.c_str());
	Serial.printf("description: %s\n", currentWeather.description.c_str());
	String iconName = currentWeather.icon;
	Serial.printf("iconMeteoCon: %s\n", iconName.c_str());
	String test = "03n";
	test.replace("n","d");
	Serial.printf("test: %s\n", test.c_str());
	
}

void openWather_updateData() {
	// drawProgress(display, 10, "Updating time...");
	// drawProgress(display, 30, "Updating weather...");
	currentWeatherClient.setMetric(OPEN_WEATHER_IS_METRIC);
	currentWeatherClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
	currentWeatherClient.updateCurrentById(&currentWeather, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);
	// drawProgress(display, 50, "Updating forecasts...");
	// forecastClient.setMetric(OPEN_WEATHER_IS_METRIC);
	// forecastClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
	// uint8_t allowedHours[] = {12};
	// forecastClient.setAllowedHours(allowedHours, sizeof(allowedHours));
	// forecastClient.updateForecastsById(forecasts, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID, MAX_FORECASTS);

	openWeather_readyForWeatherUpdate = false;

}

void openWather_loop(){
	if (millis() - openWeather_timeSinceLastWUpdate > (1000L*OPEN_WEATHER_UPDATE_INTERVAL_SECS)) {
		openWeather_readyForWeatherUpdate = true;
		openWeather_timeSinceLastWUpdate = millis();
	}

	if (openWeather_readyForWeatherUpdate) {
		openWather_updateData();
	}
}

#endif