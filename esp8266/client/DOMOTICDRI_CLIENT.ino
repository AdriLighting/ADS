
#include "config_base.h"
#include "device_def.h"
#include "devices.h"
#include "relay_func.h"
#include "adriwifi.h"
#include "serverota.h"
#include "udp.h"
#include "output_func.h"

#include "tools.h"

#if CLIENT_SELECT==CLIENT_LED
	#include "fastled_func.h"
	#include "led_effect.h"
#endif

void digitPin_init(){
	#ifdef DIGIT_PIN_S1
		pinMode(DIGIT_PIN_S1, INPUT_PULLUP);
	#endif
	#ifdef DIGIT_PIN_S2
		pinMode(DIGIT_PIN_S2, INPUT_PULLUP);
	#endif		
}
void setup() {
	
	relay_setup();
	digitPin_init();

	#if DEVICE_SELECT==PLAFOND_RELAY
		relay_set_on(0);
	#endif
	#if DEVICE_SELECT==SALON_CANAPE_RELAY
		relay_set_on(0);
	#endif

	delay(1000);

	Serial.begin(115200);
	debug_webServer 		= true;	
	debug_httpServer 		= true;	
	debug_webSocketServer 	= true;		
	

	#if CLIENT_SELECT==CLIENT_LED
		strip_setup();
	#endif

	SPIFFS.begin();

	output_dDefault();
	
	String aVal = value_loadFromSPIFF(false, "/" + FILENAME_SAVVALUE + ".txt");
	if (aVal != "") value_load_fromString(true, aVal);

	output_preset_load(false);

	_wfifi_connect(0, "ssid", "pswd", device_ip, DEVICENAME, false, false);	
	server_setup();
	telnet_setup();
	udp_start();

}

void keyboard_loop();
void button_loop();
void loop() {

	keyboard_loop();
	button_loop();
	udp_loop();
	server_loop();
	ota_loop();
	telnet_loop ();
}
void button_loop_1(){

}

void button_loop(){
		#if DIGIT_MOD==DIGIT_MOD_1
			if (digitalRead(DIGIT_PIN_S1) == OUTPUT_ON){
				delay(50);      
				while (digitalRead(DIGIT_PIN_S1) == OUTPUT_ON) {
					delay(5);
				}
				Serial.printf("1\n");
				output_all_toggleOnOff();
			    String aVal = value_sav_toStringt(true);
			    value_savToSPIFF(true, "/" + FILENAME_SAVVALUE + ".txt", aVal);					
			}			
		#endif	
		#if DIGIT_MOD==DIGIT_MOD_3
			if (digitalRead(DIGIT_PIN_S1) == OUTPUT_OFF){
				delay(50);      
				while (digitalRead(DIGIT_PIN_S1) == OUTPUT_OFF) {
					delay(5);
				}
				Serial.printf("1\n");
				output_all_toggleOnOff();
			    String aVal = value_sav_toStringt(true);
			    value_savToSPIFF(true, "/" + FILENAME_SAVVALUE + ".txt", aVal);					
			}			
		#endif				
		#if DIGIT_MOD==DIGIT_MOD_2
			if (digitalRead(DIGIT_PIN_S1) == OUTPUT_ON){
				delay(50);      
				while (digitalRead(DIGIT_PIN_S1) == OUTPUT_ON) {
					delay(5);
				}    
				Serial.printf("1\n");
				output_selected_toggleOnOff("0|0", true);
			    String aVal = value_sav_toStringt(true);
			    value_savToSPIFF(true, "/" + FILENAME_SAVVALUE + ".txt", aVal);	
			} 	
			if (digitalRead(DIGIT_PIN_S2) == OUTPUT_ON){
				delay(50);      
				while (digitalRead(DIGIT_PIN_S2) == OUTPUT_ON) {
					delay(5);
				}    
				Serial.printf("1\n");
				output_selected_toggleOnOff("1|0", true);
			    String aVal = value_sav_toStringt(true);
			    value_savToSPIFF(true, "/" + FILENAME_SAVVALUE + ".txt", aVal);	
			} 					
		#endif	
}

String user_menu_info(){
	String s;
	s += "a = devices_display\n";
	s += "z = devices_display full\n";
	s += "r = restart esp\n";
	s += "q = toggle all device\n";
	s = "\n[MENU]\n" + s + "\n";
	return s;
}

String user_menu(char c) {
	String s;
	String s2;
	switch (c) {
		case 'm' : s = user_menu_info(); 	break;
		case 'a' : s = device_display(0); 	break;
		case 'z' : s = device_display(1); 	break;
		case 'q' : s= "toggle all device";
		    for( int i = 0; i < output_count; i++) { 
		        if (output_selected[i] == 1) {
		        	// selectedStrip_toggleOnOff(String(i)+"|0");
		        }
		    }
			break;
		case 'r' : 
			s = "ESP.reset";
			delay(1000);
			ESP.reset(); 
			break;				
		default  : if  (c!=char(10) && (c!=char(13)) ) { s= "command "+String(c)+" not known (ASCII " + String(int(c)) + ")";  }
	}
	return s;
}
void keyboard_setup	(){}
void keyboard_end	(){}

void keyboard_loop	(){
	if (Serial.available()) {
		char c=Serial.read();
		String s=user_menu(c);
		debug_mode=both;
		debug(&s);
	}
	char input[20];
	if (telnet_get(input)) {
		char c=input[0];
		String s=user_menu(c);
		debug_mode=both;
		debug(&s);
	}	
}
