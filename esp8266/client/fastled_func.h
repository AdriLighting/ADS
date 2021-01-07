#include "config_base.h"
#include "device_def.h"
#include "devices.h"

#if CLIENT_SELECT==CLIENT_LED


#ifndef FASTLED_FUNC_H
   #define FASTLED_FUNC_H

	void strip_setup();

	#if LED_LIB==FASTLED
	   	#include <FastLED.h>
		#include "colors.h"
	
		void setColor(CRGB c);
		void led_show();
		void setPixelColor_ByStrip(CRGB* ledarray, int numleds, RgbColor c, bool show);
		void setPixelColor_ByStrip(CRGB* ledarray, int numleds, CHSV c_chsv, bool show);
		void setPixelColor_ByStrip(CRGB* ledarray, uint8_t numleds, CRGB c, bool show);	   
	#endif

	#if LED_LIB==NEOPIX
	   	#include <Adafruit_NeoPixel.h>

		void led_set_red() ;
		void setColor(uint32_t c);
		void setHsvColor(uint8_t h, uint8_t s, uint8_t v);
		void setRgbColor(uint8_t r, uint8_t g, uint8_t b) ;
		void setColor(uint8_t v1, uint8_t v2, uint8_t v3, char mod);
		void led_show();
	#endif

	void led_show_force(int pos);		
	int allStrip_white(uint8_t val);
	int selectedStrip_white(String v, boolean select);
	void setWhite(int pos, uint8_t v, boolean show);

	int allStrip_setBri(uint8_t val);
	void selectedStrip_setColor(String color);
	int allStrip_hue(uint8_t val);
	int allStrip_sat(uint8_t val);
	int allStrip_bri(uint8_t val);

	int selectedStrip_setBri(String v, 	boolean select);		
	int selectedStrip_hue(String v, 	boolean select);
	int selectedStrip_sat(String v, 	boolean select);
	int selectedStrip_bri(String v, 	boolean select);

	void selectedStrip_setColor(String color);
	void setBriFromString(int pos, int val);
	void setColorFromString(int pos, boolean show);
	void setColorFromString(String color, int pos, boolean show);
	
	int selectedStrip_setcolorFromRGB(String v, boolean select);

	extern boolean command_debug;

#endif

		
#endif



