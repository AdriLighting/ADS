#ifndef OLED_DISPLAY_MENU_H
   	#define OLED_DISPLAY_MENU_H
	#include <Arduino.h>
	void oled_init();
	void oled_menu_setup();
	boolean oled_menu_create_items_device();
	extern String oled_menu_selectedDevice;
	extern String oled_menu_selectedGrp;
	extern String oled_menu_selectedRoom;
	extern boolean om_toggle_select;

	// extern String om_act_light;
	// extern String om_act_color;
	// extern String om_act_device;
	extern char PROGMEM om_act_light   [] ;
	extern char PROGMEM om_act_color   [] ;
	extern char PROGMEM om_act_device  [] ;
	extern char PROGMEM om_act_rommGrp  [] ;
#endif