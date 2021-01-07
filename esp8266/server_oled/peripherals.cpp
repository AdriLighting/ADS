/*
 * nuts_screws.cpp
 *
 *  Created on: 27 févr. 2018
 *      Author: Nri
 *
 *	Process hardware dependents things like push buttons, leds etc
 */

#include "peripherals.h"

#include <Arduino.h>
#include "tools.h"

//#define BUTTONS_DEBUG
//#define LEDS_DEBUG

boolean peripherals_trace = false;

int button_nbr=0;
button * buttons[BUTTON_MAX];
int button_create(int pin, boolean pullup) {
	if (button_nbr<BUTTON_MAX) {
		#ifdef BUTTONS_DEBUG
				fsprintf("creating button nbr:%d pin:%d, pullup:%d\n", button_nbr, pin, pullup);
		#endif
		buttons[button_nbr]=new button(button_nbr, pin, pullup);
		button_nbr++;
		return button_nbr-1;
	}
	return -1;
}

void	peripherals_setup() {
		for (int i=0; i<button_nbr;	i++) buttons[i]->setup();
}

void	peripherals_loop() {
		for (int i=0; i<button_nbr;	i++) buttons[i]->loop();
}

void	peripherals_end() {
		for (int i=0; i<button_nbr;	i++) buttons[i]->end();
}

#ifdef BUTTONS_DEBUG
	String status(int status) {
		switch (status) {
		case 0: return "Inactive     ";
		case 1: return "Raised       ";
		case 2: return "After click  ";
		case 3: return "After long   ";
		case 4: return "Wait release ";
		case 5: return "End and clear";
		}
	}
#endif

	#define MAX_SHORT 	250
	#define MAX_LONG	2000

		String dir_string(boolean pullup) {if (pullup) return "pullup"; else return "pulldown";}

	button::button(int id, int pin, boolean pullup) {
		_id				= id;
		_pin_number		= pin;
		_pullup			= pullup;
		_press			= on();
		_click_number	= 0;
		_status 		= end_and_clear;
		_last_change	= millis();
		_short_press	= 0;
		_long_press		= false;
		if (_pin_number>=0) pinMode(_pin_number, INPUT_PULLUP);
		#ifdef BUTTONS_DEBUG
			if (_pin_number>=0)  { fsprintf("Connecting button %d (%s)\n", id, hw_config().c_str());}
			else {fsprintf("Button %d (%s) not connected\n", id, hw_config().c_str());}
		#endif
	}

	void button::setup() {
		if (_pin_number>=0) pinMode(_pin_number, INPUT_PULLUP);
	}

	void button::end() {}

	void button::loop() {
		boolean b = on();
		switch (_status) {
				/* when inactive, we wait for the button activation */
                case inactive: //0
                    if (b) {
                        _last_change    = millis();
                        _status         = raised;
                        _click_number   = 0;
                        change_state();
                    }
                    break;

                /* when button is raised, we expect either a falling edge -short click- or a time out -long press- */
                case raised: //1
                    if (!b) {
                        _last_change    = millis();
                        _click_number++;
                        _status         = after_click;
                        change_state();
                    }
                    else if ((millis()-_last_change)>MAX_LONG) {
                        _long_press     = true;
                        _status         = after_long;
                        change_state();
                    }
                    break;

                /* after a click, we can have another click -double- or a time out -single- */
                case after_click: //2
                    if (b) {
                        _last_change    = millis();
                        _status         = raised;
                        change_state();
                    }
                    else if ((millis()-_last_change)>MAX_SHORT) {
                        _short_press    = _click_number;
                        _status         = inactive;
                        change_state();
                    }
                    break;

                /* after a long press, we wait for the button release */
                case after_long: //3
                    if (!b) {
                        _last_change    = millis();
                        _status         = inactive;
                        _long_press     = false;
                        change_state();
                    }
                break;

                case wait_release: //4
                    if (!b) {
                        _last_change    = millis();
                        _status         = inactive;
                        change_state();
                        _long_press=true;
                    }
                    break;

                case end_and_clear: //5
                    if (!b) {
                        _last_change    = millis();
                        _status         = inactive;
                        _short_press    = 0;
                        _long_press     = false;
                        _press          = false;
                        change_state();
                    }
                    break;
		}
		_press = b;
	}

	int button::click() {
		int b = _short_press;
		if (peripherals_trace) if (b!=0) fsprintf("button %d %d short press\n",_id,b);
		_short_press 	= 0;
		return b;
	}

	boolean button::long_press() {
		boolean b = _long_press;
		if (peripherals_trace) if (b!=0) fsprintf("button %d long press\n",_id);
		_long_press 	= false;
		return b;
	}

    void button::change_state() {
        boolean b = on();
        #ifdef BUTTONS_DEBUG
            fsprintf("button change button:%d status:%s click:%d long:%d\n", b , status(_status).c_str(), _short_press, _long_press);
        #endif
        _press = b;
    }
    boolean button::on() {
        if (_pin_number>=0) {
            if (_pullup)    return (digitalRead(_pin_number)==LOW);
            else            return (digitalRead(_pin_number)!=LOW);
        } else {
            return false;
        }
    }

	void button::reset() {
		_status 		= end_and_clear;
	}

