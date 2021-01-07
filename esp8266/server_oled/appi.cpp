#include "config_base.h"

#ifdef WEBSERVER_ENABLE

#include <dac_command.h>

#include "adriwifi.h"

#include "udp.h"

#include "tools.h"

#include "devices.h"
#include "devices_preset.h"
#include "devices_display.h"
#include "devices_output.h"

	extern devices 		devices;
#ifdef OLED_UPD_WEBSOCKET
	extern oled_requests oled_request;
#endif

void appi_send_udpRequest_dn(String dn, char * req, String val, String upd) {
	// String disp = "oled_send_udpRequest_dn\n"; 
	// debug(&disp);

	String lib 	= devices.getLib(dn);
	if (lib == "multiLampe") req = device_set_req(req);
	String cmd 	= request_set_format(req, val, "&", "=");
	String op 	= device_setOp(dn);
	if (op == "") return;
	String 	s2 = "";
			s2 = 	literal_item("dn", 	dn);
			s2 += 	literal_item("op", 	op);
			s2 += 	literal_item("msg",	cmd);
			#ifdef OLED_UPD_WEBSOCKET
				if (upd!="") {
					oled_request_clear();
					udp_appi_rep = true;
					oled_request.device[oled_request.cnt][0] = dn;
					oled_request.device[oled_request.cnt][1] = "";
					oled_request.cnt++;		
			    	oled_request_sendString();
			    	oled_request_start();
		    	}
				udpMulti_send(s2);				   
			#else
			 	udpMulti_send(s2);
		    #endif	
}
boolean request_isTrueWhite(char * req){
	if(req == req_ss_lampBri) 	return true;
	if(req == req_lampBri) 		return true;
	return false;
}
char *  request_getTrueWhite(char * req){
	if(req == req_lampBri) 		return req_lampWhite;
	if(req == req_ss_lampBri) 	return req_ss_white;
}
	// if (request_isTrueWhite(req)) req = request_getTrueWhite(req);
void appi_send_udpRequest_mdn(String val, char * req, String dg, String upd) {
	// String disp = "oled_send_udpRequest_mdn\n"; 
	// debug(&disp);

	String cmd = request_set_format(req, "&", "=");

	String 	s2 = "";

	#ifdef OLED_UPD_WEBSOCKET
		if (upd!="") {
			oled_request_clear();
			udp_appi_rep = true;
		}
	#endif

    for( int i = 0; i < devices_count; i++) {
        int output_count = devices_array[i].output_count;
        String s = "";
        for( int j = 0; j < output_count; j++) {
        	// if ((om_toggle_select) && (val == "1") && (req == req_s_output) && (!devices_array[i].toggleOnOff[j])) continue; 

        	// if ( (oled_menu_starterItem == c_to_string(om_act_color)) && (output_isRelay(devices_array[i].output_type[j])) ) continue;
            if (dg != devices_array[i].output_grp[j]) continue;
            // if (om_arborescence_item[1] == c_to_string(om_act_rommGrp)){
            // 	if (om_arborescence_item[2] != devices_array[i].device_room[j]) continue;
            // } 

			String lib 	= devices.getLib(devices_array[i].device_name);
			if (lib == "multiLampe") 	{	s = device_setVal(req, val); cmd = request_set_format(device_set_req(req), "&", "=");}
			else  							s+= String(j) + "|" + val + "*";        
			
        }
        if (s == "") continue;
		String op 	= device_setOp(devices_array[i].device_name);
		if (op == "") continue;   

		#ifdef OLED_UPD_WEBSOCKET
			if (upd!="") {
				oled_request.device[oled_request.cnt][0] = devices_array[i].device_name;
				oled_request.device[oled_request.cnt][1] = "";
				oled_request.cnt++;
			}
		#endif

		s2 += 	literal_item("dn", devices_array[i].device_name);
		s2 += 	literal_item("op", op);
		s2 += 	literal_item("msg", cmd + s);
       	s2 += 	"#";
    }	
    #ifdef OLED_UPD_WEBSOCKET	
    	if (upd!="") {	
    		oled_request_sendString();
    		oled_request_start(); 
    	}
		udpMulti_send(s2);
	#else
	 	udpMulti_send(s2);
    #endif
}
#endif
