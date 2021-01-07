#include "config_base.h"
#ifndef OUTPUT_FUNC_H 
	#define OUTPUT_FUNC_H
    #include <arduino.h>

	int output_set_selected(String v);


	boolean output_isRelay(int pos);
	boolean output_isStrip(int pos);
	boolean output_isRGBWStrip(int pos);
	boolean output_isRGBStrip(int pos);

	int output_set_selected(String v);
	int output_selected_toggleOnOff(String v, boolean select);
	int output_selected_onOff(String v, boolean select);
	int output_all_toggleOnOff();
	int output_all_on();
	int output_all_off();
	int output_all_onOff();
	int output_all_onOff(boolean relay);
	void output_save_load(String s);
	int output_all_bri();
	int output_dDefault();
	void output_preset_load(boolean relay);
	
#endif

