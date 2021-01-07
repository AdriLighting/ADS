#include "config_base.h"
#include "request.h"
#include <dac_command.h>
#include "tools.h"
#include "adriwifi.h"
#include "udp.h"
#include "webserver_update.h"
#include "serverota.h"
#ifdef TFT_ENABLE
		#include "tft_display.h"
#endif
#ifdef OW_METEO
		#include "meteo.h"
#endif
extern wifi_id wifi_id;

// #define DEBUG

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

char PROGMEM req_wifi_ssid                          [] = "wifi_ssid";
char PROGMEM req_wifi_ssid_psk                      [] = "wifi_psk";
char PROGMEM req_wifi_ip                            [] = "wifi_ip";
char PROGMEM req_wifi_reset                         [] = "wifi_reset";

char PROGMEM req_wifi_subnet                        [] = "wifi_subnet";
char PROGMEM req_wifi_gateway                       [] = "wifi_gateway";

char PROGMEM req_wifiAp_ip                          [] = "wifiAp_ip";
char PROGMEM req_wifiAp_psk                         [] = "wifiAp_psk";
int command_wifi_ssid(String value){
#ifdef DEBUG
   if (command_debug) fsprintf("\n[command_wifi_ssid] : %s\n", value.c_str());
#endif
   wfifi_getID_fromSPIFF();
   wifi_id.ssid = value;
   wfifi_getID_toSPIFF();
   return 0;
}
int command_wifi_ssid_psk(String value){
#ifdef DEBUG
   if (command_debug) fsprintf("\n[command_wifi_ssid_psk] : %s\n", value.c_str());
#endif
   wfifi_getID_fromSPIFF();
   wifi_id.pswd = value;
   wfifi_getID_toSPIFF();
   return 0;
}
int command_wifi_ip(String value){
#ifdef DEBUG
   if (command_debug) fsprintf("\n[command_wifi_ip] : %s\n", value.c_str());
#endif
   wfifi_getID_fromSPIFF();
   wifi_id.ip = value;
   wfifi_getID_toSPIFF();
   return 0;
}
int command_wifi_reset(String value){
#ifdef DEBUG
   if (command_debug) fsprintf("\n[command_wifi_reset] : %s\n", value.c_str());
#endif
   wfifi_getID_fromSPIFF();
   wifi_id.mod = value.toInt();
   wfifi_getID_toSPIFF();
   restart_required = true;
   return 0;
}
int command_wifi_gateway(String value){
#ifdef DEBUG
   if (command_debug) fsprintf("\n[command_wifi_gateway] : %s\n", value.c_str());
#endif
   return 0;
}
int command_wifi_subnet(String value){
#ifdef DEBUG
   if (command_debug) fsprintf("\n[command_wifi_subnet] : %s\n", value.c_str());
#endif
   return 0;
}

int command_wifiAp_ip(String value){
#ifdef DEBUG
   if (command_debug) fsprintf("\n[command_wifiAp_ip] : %s\n", value.c_str());
#endif
   wfifi_getID_fromSPIFF();
   wifi_id.ap_ip = value;
   wfifi_getID_toSPIFF();
   return 0;
}
int command_wifiAp_psk(String value){
#ifdef DEBUG
   if (command_debug) fsprintf("\n[command_wifiAp_psk] : %s\n", value.c_str());
#endif
   return 0;
}

char PROGMEM req_aapp_get_list    [] = "aapp_get_list";

#ifdef TFT_ENABLE
		char PROGMEM req_aapp_tft_screen  [] = "aapp_tft_screen";
		String s;
		int command_aapp_tft_screen(String v){
				int pos = v.toInt();
				switch(pos){
						case 1 : tft_display_server_statu(true); break;
						case 2 : tft_display_tdm();   openWather_updateData(); break;
						case 3 : tft_display_tm();    openWather_updateData(); break;
						case 4 : tft_bmp_fileList_init("/bmp");tft_bmp_fileList_display(); break;
				}
		}
#endif
int command_aapp_get_list(String v){
		// if (v == "device") {
		//     String rep = webserverSend_list_devices();
		//     udp_send(rep);

		// }
}
PROGMEM cmdApp_req_lists cmdApp_req_list [] = { 
		#ifdef TFT_ENABLE
		{req_aapp_tft_screen,   &command_aapp_tft_screen},
		#endif
		{req_aapp_get_list,     &command_aapp_get_list},

		{req_wifi_gateway,      &command_wifi_gateway                          },
		{req_wifi_subnet,       &command_wifi_subnet                           },
		{req_wifi_ssid,         &command_wifi_ssid                             },
		{req_wifi_ssid_psk,     &command_wifi_ssid_psk                         },
		{req_wifi_ip,           &command_wifi_ip                               },
		{req_wifiAp_ip,         &command_wifiAp_ip                             },
		{req_wifiAp_psk,        &command_wifiAp_psk                            },

		{req_wifi_reset,&command_wifi_reset                            },   
};
uint8_t cmdApp_req_listCount = ARRAY_SIZE(cmdApp_req_list);


int setCommand(String search, String value, String request_mod, boolean display) {
	 char     * name;
	 char     buf[80];
	 String   s;
	 String   ret = "";
	 long     start = millis();

	 #ifdef DEBUG
			 fs(lpinf_titre, "[SetCommand] Start\n");
			 fs(lpinf_request_mod, "request_mod");
			 fs(lpinf_command, "command");
			 fs(lpinf_value, "value");
			 fs(lpinf_retvalue, "return value");
			 fs(lpinf_runCmd, "------------->RUN FUNCTION<---\n");
	 #endif

	 value.replace(" ", "");

	 int count = 0;

	 for (int i = 0; i < cmdApp_req_listCount; ++i) {
			name = cmdApp_req_list[i].name;
			sprintf(buf, "%s",name );
			String req = String(buf);
			if (req == search) {

				 // description = request_command_leds[i].description;
				 #ifdef DEBUG
						 if (display) {
								s+= fsget(lpinf_titre);
								s+= info_parm(fsget(lpinf_request_mod), request_mod);
								s+= info_parm(fsget(lpinf_command), req);
								s+= info_parm(fsget(lpinf_value), value);
								s+= fsget(lpinf_runCmd);
								Serial.printf("%s", s.c_str()); 
						 }  
				 #endif

				cmdApp_req_list[i].cmd(value);
				count++;

				 #ifdef DEBUG
						 if (display) {
								s   = "------------->RUN FUNCTION END<---\n";
								ret += req + " - ";
								// s +=info_parm(fsget(lpinf_retvalue), ret);
						 }  
				 #endif
				 break;
			}
	 }
	 long end = millis();
	 #ifdef DEBUG
			 if (display) {
					fs(lpinf_end, "Duration");
					s+=info_parm(fsget(lpinf_end), String(end - start));
					debug(&s);
					fsprintf("%s[SetCommand DONE] Ret: %s\n", s.c_str(), ret.c_str());  
			 }
	 #endif
	 // if (pattern.debugSetvalue) pattern.debugSetvalue  = false;
	 // if (command_debug) command_debug  = false;
	 return count;
}

int getCommandParm(String s_command, String s_valueCommand, String request_mod, boolean display, int count) {
		int ret = 0;
	 
		#ifdef DEBUG
				String s;
				if (display) {
						s = "[getCommandParm] [" + String(count) + "]\n";
						s += info_parm("request_mod", request_mod);
						s += info_parm("command", s_command);
						s += info_parm("value", s_valueCommand); 
				}   
		#endif
		if (request_mod == "cmd_type_1") { 

		} 
		else {

				#ifdef DEBUG
						if (display) Serial.printf("%s", s.c_str());  
				#endif

				ret = setCommand(s_command, s_valueCommand, request_mod, display);

		}

			 #ifdef DEBUG
					 if (display) {
							fsprintf("[getCommandParm DONE] Ret: %d\n", ret);
					 }   
			 #endif

	 return ret;          
}

int getCommand(String A_readString, String request_mod, boolean display) {
		String  s_command;
		String  s_valueCommand;
		String  s_readString = A_readString;
		char    inputChar[s_readString.length() + 1] ;
						s_readString.toCharArray(inputChar, s_readString.length() + 1);
		char    * Rcommand  = strtok(inputChar, "&");
		int     count       = 1;
		int     cmdCount    = 0;
		while (Rcommand != 0){          
				char* separator  = strchr(Rcommand, '=');
				if (separator != 0) {
						*separator  = 0;            
						++separator ;
						s_command = String(Rcommand);
						s_valueCommand = String(separator); 
						int c = getCommandParm( s_command,  s_valueCommand, request_mod, display, count);
						cmdCount += c;
						count++;
				}
				Rcommand = strtok(0, "&"); 
		}
		// s_command == "" ? s_command = "UNKNOW_COMMAND" : s_command = s_command;
		return cmdCount;
} 