
#ifndef NTP_TIME_H
	#define NTP_TIME_H
	#include <Arduino.h>
	#include <TimeLib.h>

	void ntp_time_init();
	void time_sendToUdp();
	time_t getNtpTime() ;
	extern time_t prevDisplay;
#endif