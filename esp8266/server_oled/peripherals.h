/*
 * buttons.h
 *
 *  Created on: 27 févr. 2018
 *      Author: Nri
 */


#ifndef BUTTONS_H_
#define BUTTONS_H_
#include <Arduino.h>

#define PULLUP		true
#define PULLDOWN	false

enum button_status { inactive, raised, after_click, after_long, wait_release, end_and_clear };
enum button_event	{ button_idle, button_1_click,  button_2_click,  button_3_click,  button_4_click,  button_long};

class button {
public:
	unsigned long   _last_change;
	int             _id;
	int             _click_number;
	button_status   _status;
	boolean         _pullup;
	int             _short_press;
	int             _long_press;
	boolean         _press;
	int 			_pin_number;


					button(int id, int pin, boolean direction);
	void			loop();
	void			setup();
	void			end();
	boolean			on();
	int				click();
	boolean			long_press();
	void			change_state();
	void			reset();
};



#define BUTTON_MAX 10
#define LED_MAX 5

extern	button	* buttons	[BUTTON_MAX];

int button_create(int pin, boolean pullup);

void	peripherals_setup() ;
void	peripherals_loop() ;
void	peripherals_end();

#endif /* BUTTONS_H_ */
