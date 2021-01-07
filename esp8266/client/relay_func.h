#include "config_base.h"

#ifndef RELAY_FUNC_H
	#define RELAY_FUNC_H
    #include <arduino.h>
	void relay_setup();
	int relay_statu_get(int pin);
	boolean relay_statu_isOn(int pin);
	void relay_set_on(int pin);
	void relay_set_off(int pin);
	void relay_set_toggle(int pin);
#endif