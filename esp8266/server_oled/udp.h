// #ifdef UDPMULTICLIENT	

// #ifndef UDP_H
//       	#define UDP_H

// 		#ifdef ESP8266
// 		#include <ESP8266WiFi.h>
// 		#endif 
// 		#ifdef ESP32
// 		#include <WiFi.h>
// 		#endif

// 		#define UDP_CLIENT_MAX 2

// 		class udpServer  {
// 			public:
// 			   IPAddress 	ip;
// 			   boolean 		connect;
// 			   int 			port;

// 			udpServer();  

// 		}  ;
// #endif

// #endif
#include "config_base.h"

#ifndef UDP_H
      	#define UDP_H
		#define UDPMULTI
		#define UDPDOMOTIC

		#include <WiFiUdp.h>

		extern String 	udp_receive_buffer;
		extern int 		udp_port;
		extern boolean 	udp_loop();
		extern boolean 	debug_udp;
		extern boolean 	udp_rec;
		extern uint8_t 	udp_multi_timebase;
		extern int 		udp_multi_masterNumLeds;
		extern int 		udp_multi_master_mh;
		extern int 		udp_multi_master_mw;
		extern long 	udp_multi_master_lc;
		void udp_start();
		void udp_parse(String * udp_msg);
		void udp_send(String transmit_buffer);
		bool udp_receive(String * receive_buffer);
		bool udp_loop();
		void udpMulti_send(String transmit_buffer);
		void udpMulti_send(String serveSend, String transmit_buffer);
		#ifdef WEBSERVER_ENABLE
		extern boolean udp_appi_rep;
		#endif
#endif