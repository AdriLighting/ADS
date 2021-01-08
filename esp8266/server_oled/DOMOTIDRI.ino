#include "config_base.h"

#ifdef OLED_ENABLE
	#include <oled_display.h>
#endif


#ifdef EVENT_TIMER
	#include <calendar.h>
#endif

#include <dac_command.h>

#include "adriwifi.h"
#include "udp.h"
#include "serverota.h"

#ifdef WEBSERVER_ENABLE
	#include "webserver_update.h"
#endif

#include "devices.h"
#include "devices_preset.h"
#include "devices_display.h"
#include "devices_output.h"

#ifdef OLED_ENABLE
	#include "oled_menu.h"
	#include "oled_menu_func.h"
#endif

#include "tools.h"
#include "peripherals.h"


#include "tasks.h"

#define started true
#define stopped false

task * peripherals_task;

task * web_rc_task;

task * devices_update_task;

task * udp_task;

task * keyboard_task;


void keyboard_loop();
void button_process_events();
void button_startup_events();

extern devices 		devices;
extern devices_edit devices_edit;
extern output_edit 	output_edit;

mod_hsv_loop hsvb_loop;


#ifdef NTP_TIME
	#include "ntp_time.h"
#endif

#ifdef TFT_ENABLE
	#include "tft_display.h"
#endif

#ifdef OW_METEO
	#include "meteo.h"
#endif
#ifdef DHT_ENABLE
    #include "meteo_dht.h"
#endif 


void setup() {

	Serial.begin(115200);

	// 
	SPIFFS.begin();
	delay(1000);


	// DEVICES SETUP
	devices_edit_setup();
	output_edit_setup();
	#ifdef DEVICEFROMSPIFF
		deviceSpiff_load(); // SPIFFS.remove("/devcices_save.txt"); 
	#endif
	preset_load(); // SPIFFS.remove("/output_preset.txt");
		
    // INIT OLED MENU
	#ifdef OLED_ENABLE
		oled_init();
	#endif

	// INIT DHT22	
	#ifdef DHT_ENABLE
	    dht_init();
	#endif	
	    
	// CONNECT TO WIFI
	IPAddress 	ip(192, 168, 0, 141);
	_wfifi_connect(0, "ssid", "pswd", ip, DEVICENAME, false, false); 

	// INIT WEBSERVER	
	debug_webSocketServer 	= true;		
	server_setup();

	// CONNECT UDP 
	udp_start();

	// TELENET SETUP
	telnet_setup();
	
	// OLED SETUP
    #ifdef OLED_ENABLE
	    oled_menu_setup();
	    hsvb_loop = hsv_loop_n;
    #endif

    // INIT NTP
	#ifdef NTP_TIME    
	    ntp_time_init();
	#endif

    // TFT SETUP
    #ifdef TFT_ENABLE
		#include "tft_display.h"
    	tft_setup();
    #endif

	// OPEN WEATHER METEO API UPDATE
    #ifdef OW_METEO
    	openWather_updateData() ;
    #endif


    // INIT TASK
    peripherals_task    = new task("peripherals",   	peripherals_setup,  	peripherals_loop,   	peripherals_end,    100000,		stopped);
    web_rc_task    		= new task("web_rc",  			web_rc_setup,  			web_rc_loop,   			web_rc_end,    		100000,		stopped);
    devices_update_task	= new task("devices_update",	devices_update_setup,	devices_update_loop,	devices_update_end,	100000,		stopped);
    udp_task			= new task("udp",				udp_rc_setup,			udp_rc_loop,			udp_rc_end,			5000,		stopped);
    keyboard_task       = new task("keyboard",			keyboard_setup,			keyboard_loop,			keyboard_end,		100000,		stopped);

    // START TASK
    peripherals_task    ->start();
    web_rc_task    		->start();
    devices_update_task	->start();
    udp_task			->start();
    keyboard_task		->start();

    // INIT BTN
    button_startup_events(); 





}


void loop() {


    // TASKS LOOP
    schedule_all_tasks();
    button_process_events(); 
    #ifdef OLED_ENABLE
		oled_menu_hsvLoop();
	#endif
    #ifdef OW_METEO
    	openWather_loop() ;
    #endif	
	#ifdef NTP_TIME
	    if (now() != prevDisplay) { 
	        prevDisplay = now();
			time_sendToUdp();
			#ifdef TFT_ENABLE
				tft_update_time();
				tft_update_temp();
			#endif			
	    }   
		#ifdef EVENT_TIMER
	    	// calendar_update_all();
		#endif  	    
    #endif

	ota_loop();

}

void stats_setup	()	{								}
void stats_loop		()	{	task_all_stats_display();	}
void stats_end		()	{								}


void web_rc_setup(){
}
void web_rc_loop() {
	telnet_loop ();	
	server_loop();
}
void web_rc_end() {
}

void udp_rc_setup(){
}
void udp_rc_loop() {
	udp_loop();
}
void udp_rc_end() {
}

void devices_update_setup(){
}
void devices_update_loop() {
	devices.upd_udpDevices();
	#ifdef WEBSERVER_ENABLE
		webSocketUpdate_loop();
	#endif	
	
}
void devices_update_end() {
}



String user_menu_info(){
	String s;
	s += "w = restart esp\n";
	s = "\n[MENU]\n" + s + "\n";
	return s;
}
String user_menu(char c) {
	String s;
	String send_str;

	switch (c) {
		#ifdef TFT_ENABLE
		case '1' : tft_display_server_statu(true); break;
		case '2' : tft_display_tdm(); openWather_updateData(); break;
		case '3' : tft_display_tm(); openWather_updateData(); break;
		case '4' : tft_bmp_fileList_init("/bmp");tft_bmp_fileList_display(); break;
		#endif
		case 'w' : 
			s = "ESP.reset";
			delay(1000);
			ESP.reset(); 
			break;


		default  : if  (c!=char(10) && (c!=char(13)) ) { s= "command "+String(c)+" not known (ASCII " + String(int(c)) + ")\n";  }
	}
	return s;
}
void keyboard_setup	(){}
void keyboard_end	(){}
void keyboard_loop	(){
	char input[20];
	if (telnet_get(input)) {
		char c=input[0];
		String s=user_menu(c);
		debug_mode=both;
		debug(&s);
	}	
	if (Serial.available()) {
		char c=Serial.read();
		String s=user_menu(c);
		debug_mode=both;
		debug(&s);
	}


}

void button_startup_events() {

	#ifdef OLED_ENABLE
		oled_button_startup_events();

    #endif
}


void button_process_events() {

	#ifdef OLED_ENABLE
		oled_button_process_events();
    #endif
}




#ifdef NTP_TIME   


#endif

#ifdef EVENT_TIMER

void myFunc_1(){
    Serial.printf("\n\t[myFunc_1]\n");

    int pos = calendar_event_selected;
    if(pos >= 0) calendarPrint_0(calendar_array[pos]->pos, pos);

}
#endif




