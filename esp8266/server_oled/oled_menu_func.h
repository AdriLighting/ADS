
#include "config_base.h"


#ifndef OLED_MENU_FUNC_H
	#define OLED_MENU_FUNC_H
	#include <Arduino.h>
	enum oled_settings_mods { 
	    oled_settings_toggle,
	    oled_settings_outputSelect,
	    oled_settings_none
	};
	extern oled_settings_mods oled_setting_mod;
	void oled_button_startup_events();
	void oled_button_process_events();
	void oled_menu_hsvLoop() ;
	void oled_send_udpRequest_dn(String dn, char * req, String val);
	void oled_menu_display_subtitle (String str) ;


	extern boolean udp_hsv_loop;
#endif