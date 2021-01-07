#include "config_base.h"

#ifdef OLED_ENABLE

#include <oled_display.h>

#include <dac_command.h>

#include "oled_menu_func.h"

#include "adriwifi.h"

#include "udp.h"

#include "oled_menu.h"

#include "peripherals.h"

#include "tools.h"

#include "devices.h"
#include "devices_preset.h"
#include "devices_display.h"
#include "devices_output.h"

oled_settings_mods oled_setting_mod;	

	extern oled_menu_create * oled_menu_home;    
	extern oled_menu_create * oled_menu_color;                
	extern oled_menu_create * oled_menu_device;                
	extern oled_menu_create * oled_menu_device_grp;                
	extern oled_menu_create * oled_menu_device_grp_select;                
	extern oled_menu_create * oled_menu_device_output;                 
	extern oled_menu_create * oled_menu_device_output_selected;   
	extern oled_menu_create * oled_menu_device_select;   
	extern oled_menu_create * oled_menu_preset; 
	extern oled_menu_create * oled_menu_device_room; 
	extern oled_menu_create * oled_menu_device_room_select; 

	extern mod_hsv_loop hsvb_loop;

	extern devices 		devices;
	extern devices_edit devices_edit;
	extern output_edit 	output_edit;
#ifdef OLED_UPD_WEBSOCKET
	extern oled_requests oled_request;
#endif



boolean oled_is_outputSelect(String val, char * req, boolean toggle) {
	if (!om_toggle_select) 		return false;
	if (val != "1") 			return false;
	if (req != req_s_output) 	return false;
	if (toggle == true) 		return false;
	return true;

}
void oled_send_udpRequest_mdn(String val, char * req) {
	// String disp = "oled_send_udpRequest_mdn\n"; 
	// debug(&disp);

	String cmd = request_set_format(req, "&", "=");

	String dg 	= oled_menu_current->getItemName();	
	String 	s2 = "";

	#ifdef OLED_UPD_WEBSOCKET
		oled_request_clear();
	#endif

    for( int i = 0; i < devices_count; i++) {
        int output_count = devices_array[i].output_count;
        String s = "";
        for( int j = 0; j < output_count; j++) {
        	// if ((om_toggle_select) && (val == "1") && (req == req_s_output) && (!devices_array[i].toggleOnOff[j])) continue; 

        	if ( (oled_menu_starterItem == c_to_string(om_act_color)) && (output_isRelay(devices_array[i].output_type[j])) ) continue;
            if (dg != devices_array[i].output_grp[j]) continue;
            if (om_arborescence_item[1] == c_to_string(om_act_rommGrp)){
            	if (om_arborescence_item[2] != devices_array[i].device_room[j]) continue;
            } 

			String lib 	= devices.getLib(devices_array[i].device_name);
			if (lib == "multiLampe") 	{	s = device_setVal(req, val); cmd = request_set_format(device_set_req(req), "&", "=");}
			else  							s+= String(j) + "|" + val + "*";        
			
        }
        if (s == "") continue;
		String op 	= device_setOp(devices_array[i].device_name);
		if (op == "") continue;   

		#ifdef OLED_UPD_WEBSOCKET
			oled_request.device[oled_request.cnt][0] = devices_array[i].device_name;
			oled_request.device[oled_request.cnt][1] = "";
			oled_request.cnt++;	
		#endif

		s2 += 	literal_item("dn", devices_array[i].device_name);
		s2 += 	literal_item("op", op);
		s2 += 	literal_item("msg", cmd + s);
       	s2 += 	"#";
    }	
    #ifdef OLED_UPD_WEBSOCKET		
    	oled_request_sendString();
		udpMulti_send(s2);
		oled_request_start();   
	#else
	 	udpMulti_send(s2);
    #endif
}
void oled_send_udpRequest_mdn_room(String val, char * req) {
	// String disp = "oled_send_udpRequest_mdn_room\n"; 
	// debug(&disp);

	String cmd = request_set_format(req, "&", "=");

	String dg 	= oled_menu_current->getItemName();	
	String 	s2 = "";
	#ifdef OLED_UPD_WEBSOCKET
		oled_request_clear();
	#endif
    for( int i = 0; i < devices_count; i++) {
        int output_count = devices_array[i].output_count;
        String s = "";
        for( int j = 0; j < output_count; j++) {
        	if ( (oled_menu_starterItem == c_to_string(om_act_color)) && (output_isRelay(devices_array[i].output_type[j])) ) continue;
            if (dg != devices_array[i].device_room[j]) continue;

			String lib 	= devices.getLib(devices_array[i].device_name);
			if (lib == "multiLampe") 	{	s = device_setVal(req, val); cmd = request_set_format(device_set_req(req), "&", "=");}
			else  							s+= String(j) + "|" + val + "*";        
			
        }
        if (s == "") continue;
		String op 	= device_setOp(devices_array[i].device_name);
		if (op == "") continue;   
		#ifdef OLED_UPD_WEBSOCKET
			oled_request.device[oled_request.cnt][0] = devices_array[i].device_name;
			oled_request.device[oled_request.cnt][1] = "";
			oled_request.cnt++;	
		#endif
		s2 += 	literal_item("dn", devices_array[i].device_name);
		s2 += 	literal_item("op", op);
		s2 += 	literal_item("msg", cmd + s);
       	s2 += 	"#";
    }			
    #ifdef OLED_UPD_WEBSOCKET		
    	oled_request_sendString();
		udpMulti_send(s2);
		oled_request_start();   
	#else
	 	udpMulti_send(s2);
    #endif
}

void oled_send_udpRequest_mdn(String val, char * req, String dg) {
	// String disp = "oled_send_udpRequest_mdn 2\n"; 
	// debug(&disp);

	String cmd = request_set_format(req, "&", "=");

	String outPut 	= oled_menu_current->getItemName();	
	String 	s2 = "";
	#ifdef OLED_UPD_WEBSOCKET
		oled_request_clear();
	#endif
    for( int i = 0; i < devices_count; i++) {
        int output_count = devices_array[i].output_count;
        String s = "";
        for( int j = 0; j < output_count; j++) {
        	if ( (oled_menu_starterItem == c_to_string(om_act_color)) && (output_isRelay(devices_array[i].output_type[j])) ) continue;
            if (dg != devices_array[i].output_grp[j]) continue;
            if (om_arborescence_item[1] == c_to_string(om_act_rommGrp)){
            	if (om_arborescence_item[2] != devices_array[i].device_room[j]) continue;
            }             

            if (outPut == devices_array[i].output_location[j]) {
				String lib 	= devices.getLib(devices_array[i].device_name);
				if (lib == "multiLampe") 	{s = device_setVal(req, val); cmd = request_set_format(device_set_req(req), "&", "=");}
				else  						s = String(j) + "|" + val + "*";  
            	break;
            }
        }
        if (s == "") continue;
		String op = device_setOp(devices_array[i].device_name);
		if (op == "") continue;  
		#ifdef OLED_UPD_WEBSOCKET
			oled_request.device[oled_request.cnt][0] = devices_array[i].device_name;
			oled_request.device[oled_request.cnt][1] = "";
			oled_request.cnt++;	
		#endif
		s2 += 	literal_item("dn", devices_array[i].device_name);
		s2 += 	literal_item("op", op);
		s2 += 	literal_item("msg", cmd + s);
       	s2 += 	"#";
    }			
    #ifdef OLED_UPD_WEBSOCKET		
    	oled_request_sendString();
		udpMulti_send(s2);
		oled_request_start();   
	#else
	 	udpMulti_send(s2);
    #endif
}
void oled_send_udpRequest_mdn_room(String val, char * req, String dg) {
	// String disp = "oled_send_udpRequest_mdn_room 2\n"; 
	// debug(&disp);

	String cmd = request_set_format(req, "&", "=");
	
	String outPut 	= oled_menu_current->getItemName();	
	String 	s2 = "";
	#ifdef OLED_UPD_WEBSOCKET
		oled_request_clear();
	#endif
    for( int i = 0; i < devices_count; i++) {
        int output_count = devices_array[i].output_count;
        String s = "";
        for( int j = 0; j < output_count; j++) {
        	if ( (oled_menu_starterItem == c_to_string(om_act_color)) && (output_isRelay(devices_array[i].output_type[j])) ) continue;
            if (dg != devices_array[i].device_room[j]) continue;
            if (outPut == devices_array[i].output_location[j]) {
				String lib 	= devices.getLib(devices_array[i].device_name);
				if (lib == "multiLampe") 	{s = device_setVal(req, val); cmd = request_set_format(device_set_req(req), "&", "=");}
				else  						s = String(j) + "|" + val + "*";  
            	break;
            }
        }
        if (s == "") continue;
		String op = device_setOp(devices_array[i].device_name);
		if (op == "") continue;      
		#ifdef OLED_UPD_WEBSOCKET
			oled_request.device[oled_request.cnt][0] = devices_array[i].device_name;
			oled_request.device[oled_request.cnt][1] = "";
			oled_request.cnt++;	
    	#endif
		s2 += 	literal_item("dn", devices_array[i].device_name);
		s2 += 	literal_item("op", op);
		s2 += 	literal_item("msg", cmd + s);
       	s2 += 	"#";
    }			
    #ifdef OLED_UPD_WEBSOCKET		
    	oled_request_sendString();
		udpMulti_send(s2);
		oled_request_start();   
	#else
	 	udpMulti_send(s2);
    #endif
}

void oled_send_udpRequest_grp_select(String val, char * req, String dg) {
	// String disp = "oled_send_udpRequest_grp_select\n"; 
	// debug(&disp);

    String 	list[80];
	String 	s2		= "";
	String 	cmd 	= request_set_format(req, "&", "=");
	String 	outPut 	= oled_menu_current->getItemName();	
	int 	pos 	= oled_menu_current->getPos();	
	String 	s 		= oled_menu_current->contents[pos].info_1;
    int 	count 	= explode(s, '|', list);	
    int 	dp 		= devices.getListPos(list[0]);
    int 	op 		= list[1].toInt();

    // char 	buffer[80];
    // sprintf(buffer, "inf: %s\ndp: %s - op: %s\n", s.c_str(), list[0].c_str(), list[1].c_str());
    // String p = String(buffer);
    // debug(&p);
    // sprintf(buffer, "dg: %s\narray g: %s\n", dg.c_str(), devices_array[dp].output_grp[op].c_str());
    // p = String(buffer);
    // debug(&p);   

	if ( (oled_menu_starterItem == c_to_string(om_act_color)) && (output_isRelay(devices_array[dp].output_type[op])) ) return;
    if (dg != devices_array[dp].output_grp[op]) return;
    if (om_arborescence_item[1] == c_to_string(om_act_rommGrp)){
    	if (om_arborescence_item[2] != devices_array[dp].device_room[op]) return;
    }	    
	if (outPut == devices_array[dp].output_location[op]) {
			String lib 	= devices.getLib(devices_array[dp].device_name);
			if (lib == "multiLampe") 	{s = device_setVal(req, val); cmd = request_set_format(device_set_req(req), "&", "=");}
			else  						s = String(op) + "|" + val + "*";  

        if (s == "") return;
		String op = device_setOp(devices_array[dp].device_name);
		if (op == "") return;   

		s2 += 	literal_item("dn", devices_array[dp].device_name);
		s2 += 	literal_item("op", op);
		s2 += 	literal_item("msg", cmd + s);
       	s2 += 	"#";

	    #ifdef OLED_UPD_WEBSOCKET	
			oled_request_clear();
			oled_request.device[oled_request.cnt][0] = devices_array[dp].device_name;
			oled_request.device[oled_request.cnt][1] = "";
			oled_request.cnt++;		    	
	    	oled_request_sendString();
			udpMulti_send(s2);
			oled_request_start();   
		#else
		 	udpMulti_send(s2);
	    #endif	

    }			

	// upd = "\n"+s2;
	// debug(&upd);


}

void oled_send_udpRequest_room_select(String val, char * req, String dg) {
	// String disp = "oled_send_udpRequest_room_select\n"; 
	// debug(&disp);

    String 	list[80];
	String 	s2		= "";
	String 	cmd 	= request_set_format(req, "&", "=");
	String 	outPut 	= oled_menu_current->getItemName();	
	int 	pos 	= oled_menu_current->getPos();	
	String 	s 		= oled_menu_current->contents[pos].info_1;
    int 	count 	= explode(s, '|', list);	
    int 	dp 		= devices.getListPos(list[0]);
    int 	op 		= list[1].toInt();

    // char 	buffer[80];
    // sprintf(buffer, "inf: %s\ndp: %s - op: %s\n", s.c_str(), list[0].c_str(), list[1].c_str());
    // String p = String(buffer);
    // debug(&p);
    // sprintf(buffer, "dg: %s\narray g: %s\n", dg.c_str(), devices_array[dp].device_room[op].c_str());
    // p = String(buffer);
    // debug(&p);    

   	if ( (oled_menu_starterItem == c_to_string(om_act_color)) && (output_isRelay(devices_array[dp].output_type[op])) ) return;
	if (dg != devices_array[dp].device_room[op]) return;
	if (outPut == devices_array[dp].output_location[op]) {

		String lib 	= devices.getLib(devices_array[dp].device_name);
		if (lib == "multiLampe") 	{s = device_setVal(req, val); cmd = request_set_format(device_set_req(req), "&", "=");}
		else  						s = String(op) + "|" + val + "*";  

        if (s == "") return;
		String op = device_setOp(devices_array[dp].device_name);
		if (op == "") return;        

		s2 += 	literal_item("dn", devices_array[dp].device_name);
		s2 += 	literal_item("op", op);
		s2 += 	literal_item("msg", cmd + s);
       	s2 += 	"#";

	    #ifdef OLED_UPD_WEBSOCKET	
			oled_request_clear();
			oled_request.device[oled_request.cnt][0] = devices_array[dp].device_name;
			oled_request.device[oled_request.cnt][1] = "";
			oled_request.cnt++;		    	
	    	oled_request_sendString();
			udpMulti_send(s2);
			oled_request_start();   
		#else
		 	udpMulti_send(s2);
	    #endif	       	
    }			
}

void oled_send_udpRequest_md(char * req, String val)  {
	// String disp = "oled_send_udpRequest_md\n"; 
	// debug(&disp);

	String cmd = request_set_format(req, "&", "=");
	
	String 	s2 = "";

	int cnt_os;

	#ifdef OLED_UPD_WEBSOCKET
		oled_request_clear();
	#endif

    for( int i = 0; i < devices_count; i++) {
        int output_count = devices_array[i].output_count;
        String s = "";
        cnt_os = -1;
        for( int j = 0; j < output_count; j++) {

        	if ( (oled_menu_starterItem == c_to_string(om_act_color)) && (output_isRelay(devices_array[i].output_type[j])) ) continue;
        	if (req == req_s_output) {
        		if (om_toggle_select) {
        			if (!devices_array[i].toggleOnOff[j]) val = "0";
        			else val = "1";
        		}
        		cnt_os++;
        	}  else {
        		if (devices_array[i].output_selected[j] == 1) cnt_os++;
        	}
        	        	
			String lib 	= devices.getLib(devices_array[i].device_name);
			if (lib == "multiLampe")	{	s = device_setVal(req, val); cmd = request_set_format(device_set_req(req), "&", "=");}
			else  							s += String(j) + "|" + val + "*"; 

        }
        if (s == "") continue;
		String op = device_setOp(devices_array[i].device_name);
		if (op == "") continue;  

			#ifdef OLED_UPD_WEBSOCKET
				if (cnt_os >= 0) {
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
    	oled_request_sendString();
		udpMulti_send(s2);
		oled_request_start();   
	#else
	 	udpMulti_send(s2);
    #endif

}

void oled_send_udpRequest_dn(String dn, char * req, String val) {
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
				oled_request_clear();
				oled_request.device[oled_request.cnt][0] = dn;
				oled_request.device[oled_request.cnt][1] = "";
				oled_request.cnt++;		
		    	oled_request_sendString();
				udpMulti_send(s2);
				oled_request_start();   
			#else
			 	udpMulti_send(s2);
		    #endif	
}

void oled_menu_click_8(){ 

	String val;

		int pos 	= oled_menu_current->getPos();
		String dn 	= oled_menu_current->getItemName();
		String dn_m	= oled_menu_selectedDevice;

// devicearray by order
if (oled_menu_current == oled_menu_device) 				{ oled_send_udpRequest_dn(dn, req_lampOnOff, "on");	} 
// all device
if (oled_menu_current == oled_menu_device_select) 		{ oled_send_udpRequest_md(req_ss_lampOnOff, "0"); 		} 
// devicearray by order and group
if (oled_menu_current == oled_menu_device_output) 		{ val = device_setVal(dn_m, req_ss_lampOnOff, "0", pos); oled_send_udpRequest_dn(dn_m, req_ss_lampOnOff, val);}
// all device by group
if (oled_menu_current == oled_menu_device_grp) 			{ oled_send_udpRequest_mdn("0", req_ss_lampOnOff);		}

if (oled_menu_current == oled_menu_device_room) 		{ oled_send_udpRequest_mdn_room("0", req_ss_lampOnOff);		}

// slected dvice
if (oled_menu_current == oled_menu_device_grp_select) 	{ oled_send_udpRequest_grp_select("0", req_ss_lampOnOff, oled_menu_selectedGrp);}

if (oled_menu_current == oled_menu_device_room_select) 	{ oled_send_udpRequest_room_select("0", req_ss_lampOnOff, oled_menu_selectedRoom);}

}
void oled_menu_click_8_db(){ 

	String val;

		int pos 	= oled_menu_current->getPos();
		String dn 	= oled_menu_current->getItemName();
		String dn_m	= oled_menu_selectedDevice;

if (oled_menu_current == oled_menu_device) 				{ oled_send_udpRequest_dn(dn, req_lampOnOff, "off");	} 
if (oled_menu_current == oled_menu_device_select) 		{ oled_send_udpRequest_md(req_ss_lampOnOff, "1"); 		} 
if (oled_menu_current == oled_menu_device_output) 		{ val = device_setVal(dn_m, req_ss_lampOnOff, "1", pos); oled_send_udpRequest_dn(dn_m, req_ss_lampOnOff, val);}
if (oled_menu_current == oled_menu_device_grp) 			{ oled_send_udpRequest_mdn("1", req_ss_lampOnOff);		}
if (oled_menu_current == oled_menu_device_room)			{ oled_send_udpRequest_mdn_room("1", req_ss_lampOnOff);		}
if (oled_menu_current == oled_menu_device_grp_select) 	{ oled_send_udpRequest_grp_select("1", req_ss_lampOnOff, oled_menu_selectedGrp);}
if (oled_menu_current == oled_menu_device_room_select) 	{ oled_send_udpRequest_room_select("1", req_ss_lampOnOff, oled_menu_selectedRoom);}
}

void oled_menu_long_click_8(){ 

	String val;

		int pos 	= oled_menu_current->getPos();
		String dn 	= oled_menu_current->getItemName();
		String dn_m	= oled_menu_selectedDevice;

if (oled_menu_current == oled_menu_device) 				{ oled_send_udpRequest_dn(dn, req_toglle, "off");	} 
if (oled_menu_current == oled_menu_device_select) 		{ oled_send_udpRequest_md(req_ss_toglle, "1"); 		} 
if (oled_menu_current == oled_menu_device_output) 		{ val = device_setVal(dn_m, req_ss_toglle, "1", pos); oled_send_udpRequest_dn(dn_m, req_ss_toglle, val);}
if (oled_menu_current == oled_menu_device_grp) 			{ oled_send_udpRequest_mdn("1", req_ss_toglle);		}
if (oled_menu_current == oled_menu_device_room) 		{ oled_send_udpRequest_mdn_room("1", req_ss_toglle);		}
if (oled_menu_current == oled_menu_device_grp_select) 	{ oled_send_udpRequest_grp_select("1", req_ss_toglle, oled_menu_selectedGrp);}
if (oled_menu_current == oled_menu_device_room_select) 	{ oled_send_udpRequest_room_select("1", req_ss_toglle, oled_menu_selectedRoom);}
}

void oled_menu_click_5(){ 

	String 	req = "";
	String 	val;
	int 	pos 	= oled_menu_current->getPos();
	String 	dn 		= oled_menu_current->getItemName();
	String 	dn_m	= oled_menu_selectedDevice;


	if (oled_menu_current == oled_menu_preset) { 
		req = 	literal_item("op", "preset_sav");
		req += 	literal_item("preset", HOST_NAME_PREFIX + dn);
		udpMulti_send(req);
	}	

	if (oled_menu_starterItem == c_to_string(om_act_light)) {

if (oled_menu_current == oled_menu_device) 				{ oled_send_udpRequest_dn(dn, req_lampOnOff, "off");	}
if (oled_menu_current == oled_menu_device_select) 		{ oled_send_udpRequest_md(req_ss_lampOnOff, "1"); 		}
if (oled_menu_current == oled_menu_device_output) 		{ val = device_setVal(dn_m, req_ss_lampOnOff, "1", pos); oled_send_udpRequest_dn(dn_m, req_ss_lampOnOff, val);}
if (oled_menu_current == oled_menu_device_grp) 			{ oled_send_udpRequest_mdn("1", req_ss_lampOnOff);		}
if (oled_menu_current == oled_menu_device_room) 		{ oled_send_udpRequest_mdn_room("1", req_ss_lampOnOff);		}
if (oled_menu_current == oled_menu_device_grp_select) 	{ oled_send_udpRequest_grp_select("1", req_ss_lampOnOff, oled_menu_selectedGrp);}
if (oled_menu_current == oled_menu_device_room_select) 	{ oled_send_udpRequest_room_select("1", req_ss_lampOnOff, oled_menu_selectedRoom);}
	
	}
	if (oled_menu_starterItem == c_to_string(om_act_color)) {
		if (hsvb_loop != hsv_loop_h) {
			hsvb_loop = hsv_loop_h;
		    oled_display_mod = oled_display_disp;
		    oled_clear();
		    display.drawString(0, 10, "hsv_loop_h");	
		    display.display();
		 }
		else if (hsvb_loop == hsv_loop_h) {
			hsvb_loop = hsv_loop_n;
			oled_display_mod = oled_display_menu;
			oled_menu_current->create_menu();
		}			
	}	
}
void oled_menu_long_click_5(){ 

	String req;
	String val;

	int pos 	= oled_menu_current->getPos();
	String dn 	= oled_menu_current->getItemName();
	String dn_m	= oled_menu_selectedDevice;

	if (oled_menu_current == oled_menu_device_select) 		{ oled_send_udpRequest_md(req_s_output, "0"); 	}
	if (oled_menu_current == oled_menu_device_output) 		{ val = device_setVal(dn_m, req_s_output, "0", pos); oled_send_udpRequest_dn(dn_m, req_s_output, val);}
	if (oled_menu_current == oled_menu_device_grp) 			{ oled_send_udpRequest_mdn("0", req_s_output);	}
	if (oled_menu_current == oled_menu_device_room) 		{ oled_send_udpRequest_mdn_room("0", req_s_output);	}
	if (oled_menu_current == oled_menu_device_grp_select) 	{ oled_send_udpRequest_grp_select("0", req_s_output, oled_menu_selectedGrp);}		
	if (oled_menu_current == oled_menu_device_room_select) 	{ oled_send_udpRequest_room_select("0", req_s_output, oled_menu_selectedRoom);}	

}
void oled_menu_click_6(){

	String 	req="";
	String 	val;
	int 	pos 	= oled_menu_current->getPos();
	String 	dn 		= oled_menu_current->getItemName();
	String 	dn_m	= oled_menu_selectedDevice;
		
	if (oled_menu_current == oled_menu_preset) { 
		req = 	literal_item("op", "preset_load");
		req += 	literal_item("preset", HOST_NAME_PREFIX + dn);
		udpMulti_send(req);

	}	

	if (oled_menu_starterItem == c_to_string(om_act_light)) {

if (oled_menu_current == oled_menu_device) 				{ oled_send_udpRequest_dn(dn, req_lampOnOff, "on"); 	}
if (oled_menu_current == oled_menu_device_select) 		{ oled_send_udpRequest_md(req_ss_lampOnOff, "0"); 		}
if (oled_menu_current == oled_menu_device_output) 		{ val = device_setVal(dn_m, req_ss_lampOnOff, "0", pos); oled_send_udpRequest_dn(dn_m, req_ss_lampOnOff, val);}
if (oled_menu_current == oled_menu_device_grp) 			{ oled_send_udpRequest_mdn("0", req_ss_lampOnOff);		}
if (oled_menu_current == oled_menu_device_room) 		{ oled_send_udpRequest_mdn_room("0", req_ss_lampOnOff);		}
if (oled_menu_current == oled_menu_device_grp_select) 	{ oled_send_udpRequest_grp_select("0", req_ss_lampOnOff, oled_menu_selectedGrp);}	
if (oled_menu_current == oled_menu_device_room_select) 	{ oled_send_udpRequest_room_select("0", req_ss_lampOnOff, oled_menu_selectedRoom);}	
	
	}
	if (oled_menu_starterItem == c_to_string(om_act_color)) {
		if (hsvb_loop != hsv_loop_s) {
			hsvb_loop = hsv_loop_s;
		    oled_display_mod = oled_display_disp;
		    oled_clear();
		    display.drawString(0, 10, "hsv_loop_s");
		    display.display();	
		 }
		else if (hsvb_loop == hsv_loop_s) {
			hsvb_loop = hsv_loop_n;
			oled_display_mod = oled_display_menu;
			oled_menu_current->create_menu();
		}		
	}
			
}
void oled_menu_long_click_6(){

	String req;
	String val;

	int pos 	= oled_menu_current->getPos();
	String dn 	= oled_menu_current->getItemName();
	String dn_m	= oled_menu_selectedDevice;

if (oled_menu_current == oled_menu_device_select) 		{ oled_send_udpRequest_md(req_s_output, "1"); 	}
if (oled_menu_current == oled_menu_device_output) 		{ val = device_setVal(dn_m, req_s_output, "1", pos); oled_send_udpRequest_dn(dn_m, req_s_output, val);}
if (oled_menu_current == oled_menu_device_grp) 			{ oled_send_udpRequest_mdn("1", req_s_output);}
if (oled_menu_current == oled_menu_device_room) 		{ oled_send_udpRequest_mdn_room("1", req_s_output);}
if (oled_menu_current == oled_menu_device_grp_select) 	{ oled_send_udpRequest_grp_select("1", req_s_output, oled_menu_selectedGrp);}		
if (oled_menu_current == oled_menu_device_room_select) 	{ oled_send_udpRequest_room_select("1", req_s_output, oled_menu_selectedRoom);}	
			
}

void oled_menu_click_7(){
		if (oled_menu_current == oled_menu_main) return;
		
		if (oled_menu_starterItem == c_to_string(om_act_color)){

			if (hsvb_loop != hsv_loop_v) {
				hsvb_loop = hsv_loop_v;
			    oled_display_mod = oled_display_disp;
			    oled_clear();
			    display.drawString(0, 10, "hsv_loop_v");	
			    display.display();
			}
			else {
				if (hsvb_loop == hsv_loop_v) {
					hsvb_loop = hsv_loop_n;
					oled_display_mod = oled_display_menu;
					oled_menu_current->create_menu();
				}			
			}				
		}

		if (oled_menu_starterItem == c_to_string(om_act_light)){

			if (hsvb_loop != hsv_loop_b) {
				hsvb_loop = hsv_loop_b;
			    oled_display_mod = oled_display_disp;
			    oled_clear();
			    display.drawString(0, 10, "hsv_loop_b");	
			    display.display();
			}
			else {
				if (hsvb_loop == hsv_loop_b) {
					hsvb_loop = hsv_loop_n;
					oled_display_mod = oled_display_menu;
					oled_menu_current->create_menu();
				}			
			}					
		}
		
			
}

void oled_menu_long_click_7(){
		if (oled_menu_starterItem == c_to_string(om_act_color)){

			if (hsvb_loop != hsv_loop_w) {
				hsvb_loop = hsv_loop_w;
			    oled_display_mod = oled_display_disp;
			    oled_clear();
			    display.drawString(0, 10, "hsv_loop_w");	
			    display.display();
			}
			else {
				if (hsvb_loop == hsv_loop_w) {
					hsvb_loop = hsv_loop_n;
					oled_display_mod = oled_display_menu;
					oled_menu_current->create_menu();
				}			
			}				
		}			
}

unsigned long timer_hue_last;
int lastPotar;

boolean udp_hsv_loop = false;

void oled_menu_hsvLoop() {
	if (oled_display_mod == oled_display_setting) return;

	if (oled_menu_current == oled_menu_main){ 
		if (oled_display_mod == oled_display_disp) {
			oled_display_mod = oled_display_menu;
			oled_menu_current->create_menu();			
		}
		return;
	}
	String  udp_hsv_req ;
	long now = millis();
	if (hsvb_loop != hsv_loop_n) {


		if (!udp_hsv_loop) {
			udp_hsv_loop = true;
			udp_hsv_req = literal_item("udp_hsv_req", "1");
			udpMulti_send(udp_hsv_req);				
		}

		if ((now - timer_hue_last) > 350) {
			timer_hue_last = now;	

			if (analogRead(A0) != lastPotar) {
				int 	potar 		= analogRead(A0);
						lastPotar 	= potar;
				String 	val 		= "";
				String 	req 		= "";
				uint8_t potVal 		= map(potar, 0, 1024, 0, 255);
				int 	pos 		= oled_menu_current->getPos();
				String 	dn 			= oled_menu_current->getItemName();			
				String 	dn_m 		= oled_menu_selectedDevice;					
				String 	ret;					

				if (hsvb_loop == hsv_loop_h) {
if (oled_menu_current == oled_menu_device) 				{ oled_send_udpRequest_dn(dn, req_lampHue, String(potVal)); 	}
if (oled_menu_current == oled_menu_device_select) 		{ oled_send_udpRequest_md(req_ss_lampHue, String(potVal)); 		}
if (oled_menu_current == oled_menu_device_output) 		{ val = device_setVal(dn_m, req_ss_lampHue, String(potVal), pos); oled_send_udpRequest_dn(dn_m, req_ss_lampHue, val);}
if (oled_menu_current == oled_menu_device_grp) 			{ oled_send_udpRequest_mdn(String(potVal), req_ss_lampHue);}
if (oled_menu_current == oled_menu_device_room) 		{ oled_send_udpRequest_mdn_room(String(potVal), req_ss_lampHue);}
if (oled_menu_current == oled_menu_device_grp_select) 	{ oled_send_udpRequest_grp_select(String(potVal), req_ss_lampHue, oled_menu_selectedGrp);}		
if (oled_menu_current == oled_menu_device_room_select) 	{ oled_send_udpRequest_room_select(String(potVal), req_ss_lampHue, oled_menu_selectedRoom);}		
					ret = dn + "\nlp_hue: " +  String(potVal);
				}
				if (hsvb_loop == hsv_loop_s) {
if (oled_menu_current == oled_menu_device) 				{ oled_send_udpRequest_dn(dn, req_lampSat, String(potVal)); 	}
if (oled_menu_current == oled_menu_device_select) 		{ oled_send_udpRequest_md(req_ss_lampSat, String(potVal)); 		}
if (oled_menu_current == oled_menu_device_output) 		{ val = device_setVal(dn_m, req_ss_lampSat, String(potVal), pos); oled_send_udpRequest_dn(dn_m, req_ss_lampSat, val);}
if (oled_menu_current == oled_menu_device_grp) 			{ oled_send_udpRequest_mdn(String(potVal), req_ss_lampSat);}
if (oled_menu_current == oled_menu_device_room) 			{ oled_send_udpRequest_mdn_room(String(potVal), req_ss_lampSat);}
if (oled_menu_current == oled_menu_device_grp_select) 	{ oled_send_udpRequest_grp_select(String(potVal), req_ss_lampSat, oled_menu_selectedGrp);}		
if (oled_menu_current == oled_menu_device_room_select) 	{ oled_send_udpRequest_room_select(String(potVal), req_ss_lampSat, oled_menu_selectedRoom);}		

					ret = dn + "\nlp_sat: " +  String(potVal);
				}				
				if (hsvb_loop == hsv_loop_v) {	
if (oled_menu_current == oled_menu_device) 				{ oled_send_udpRequest_dn(dn, req_lampBri, String(potVal)); 	}
if (oled_menu_current == oled_menu_device_select) 		{ oled_send_udpRequest_md(req_ss_lampBri, String(potVal)); 		}
if (oled_menu_current == oled_menu_device_output) 		{ val = device_setVal(dn_m, req_ss_lampBri, String(potVal), pos); oled_send_udpRequest_dn(dn_m, req_ss_lampBri, val);}
if (oled_menu_current == oled_menu_device_grp) 			{ oled_send_udpRequest_mdn(String(potVal), req_ss_lampBri);}
if (oled_menu_current == oled_menu_device_room) 			{ oled_send_udpRequest_mdn_room(String(potVal), req_ss_lampBri);}
if (oled_menu_current == oled_menu_device_grp_select) 	{ oled_send_udpRequest_grp_select(String(potVal), req_ss_lampBri, oled_menu_selectedGrp);}		
if (oled_menu_current == oled_menu_device_room_select) 	{ oled_send_udpRequest_room_select(String(potVal), req_ss_lampBri, oled_menu_selectedRoom);}		
					ret = dn + "\nlp_bri: " +  String(potVal);
				}
				if (hsvb_loop == hsv_loop_b) {		
if (oled_menu_current == oled_menu_device) 				{ oled_send_udpRequest_dn(dn, req_ledBri, String(potVal)); 	}
if (oled_menu_current == oled_menu_device_select) 		{ oled_send_udpRequest_md(req_ss_ledBri, String(potVal)); 		}
if (oled_menu_current == oled_menu_device_output) 		{ val = device_setVal(dn_m, req_ss_ledBri, String(potVal), pos); oled_send_udpRequest_dn(dn_m, req_ss_ledBri, val);}
if (oled_menu_current == oled_menu_device_grp) 			{ oled_send_udpRequest_mdn(String(potVal), req_ss_ledBri);}	
if (oled_menu_current == oled_menu_device_room) 			{ oled_send_udpRequest_mdn_room(String(potVal), req_ss_ledBri);}	
if (oled_menu_current == oled_menu_device_grp_select) 	{ oled_send_udpRequest_grp_select(String(potVal), req_ss_ledBri, oled_menu_selectedGrp);}		
if (oled_menu_current == oled_menu_device_room_select) 	{ oled_send_udpRequest_room_select(String(potVal), req_ss_ledBri, oled_menu_selectedRoom);}		
					ret = dn + "\nstrip_bri: " +  String(potVal);
				}	

				if (hsvb_loop == hsv_loop_w) {		
if (oled_menu_current == oled_menu_device) 				{ oled_send_udpRequest_dn(dn, req_lampWhite, String(potVal)); 	}
if (oled_menu_current == oled_menu_device_select) 		{ oled_send_udpRequest_md(req_ss_white, String(potVal)); 		}
if (oled_menu_current == oled_menu_device_output) 		{ val = device_setVal(dn_m, req_ss_white, String(potVal), pos); oled_send_udpRequest_dn(dn_m, req_ss_white, val);}
if (oled_menu_current == oled_menu_device_grp) 			{ oled_send_udpRequest_mdn(String(potVal), req_ss_white);}	
if (oled_menu_current == oled_menu_device_room) 			{ oled_send_udpRequest_mdn_room(String(potVal), req_ss_white);}	
if (oled_menu_current == oled_menu_device_grp_select) 	{ oled_send_udpRequest_grp_select(String(potVal), req_ss_white, oled_menu_selectedGrp);}		
if (oled_menu_current == oled_menu_device_room_select) 	{ oled_send_udpRequest_room_select(String(potVal), req_ss_white, oled_menu_selectedRoom);}		
					ret = dn + "\nstrip_tw: " +  String(potVal);
				}												
			    oled_display_mod = oled_display_disp;
			    oled_clear();
			    display.drawString(0, 10, ret);	
			    display.display();

			}
		}
	} 


	if ((hsvb_loop == hsv_loop_n) && (udp_hsv_loop) ){
		udp_hsv_loop = false;
		udp_hsv_req = literal_item("udp_hsv_req", "0");
		udpMulti_send(udp_hsv_req);				
	}

}


	int push1;
	int push2;
	int push3;
	int push4;
	int push5;
	int push6;
	int push7;
	int push8;
	#define btn_down  D6
	#define btn_up    D5
	#define btn_back  D3
	#define btn_next  D4
	#define btn_c1    D7
	#define btn_c2    3
	#define btn_c3    10
	#define btn_c4    1


void oled_button_startup_events(){
	    push1 = button_create (btn_up, 		PULLUP);
	    push2 = button_create (btn_down, 	PULLUP);
	    push3 = button_create (btn_back, 	PULLUP);
	    push4 = button_create (btn_next,	PULLUP);
	    push5 = button_create (btn_c1,	 	PULLUP);
	    push6 = button_create (btn_c2,	 	PULLUP);
	    push7 = button_create (btn_c3,	 	PULLUP);
	    push8 = button_create (btn_c4,	 	PULLUP);	
}

	boolean button_getNbrClick(int nbr) {
		if (nbr == 1) return true;
		if (nbr == 2) return true;
		if (nbr == 3) return true;
		return false;
	}
	boolean button_getNbrClick(int nbr, int maxClick) {
		if (nbr == maxClick) return true;
		return false;
	}

void oled_choose_toggle(){
	oled_setting_mod = oled_settings_toggle;
    oled_display_mod = oled_display_setting;
    oled_clear();
    display.drawString(0, 10, "oled_choose_toggle");	
    display.drawString(10, 20, "togglSelect : ");	
    display.drawString(10, 30, String(om_toggle_select));	
    display.display();	
}
void oled_display_outputSelect(String s){
	oled_setting_mod = oled_settings_outputSelect;
    oled_display_mod = oled_display_setting;
    oled_clear();
    display.drawString(0, 10, "outputSelect");	
    display.drawString(10, 20, s);	
    display.display();	
}
void oled_display_settings_off(){
	oled_setting_mod = oled_settings_none;
	oled_display_mod = oled_display_menu;
	oled_menu_current->create_menu();
}

void oled_button_process_events() {
	    int click1         = buttons[push1]->click();
	    int click2         = buttons[push2]->click();
	    int click3         = buttons[push3]->click();
	    int click4         = buttons[push4]->click();
	    int click5         = buttons[push5]->click();
	    int click6         = buttons[push6]->click();
	    int click7         = buttons[push7]->click();
	    int click8         = buttons[push8]->click();

	    if (button_getNbrClick(click1) ) {oled_menu_click_move(false);} 
	    if (button_getNbrClick(click2) ) {oled_menu_click_move(true);} 
	    if (button_getNbrClick(click3) ) {oled_menu_click_3_set(oled_menu_move_back);} 
	    if (button_getNbrClick(click4) ) {oled_menu_click_4_set(oled_menu_move_next);}  

	    if ((oled_display_mod == oled_display_menu) || (oled_display_mod == oled_display_disp))  {



		    if (buttons[push5]->long_press()) {
		    	// debug("oled_menu_long_click_5\n");
		    	oled_menu_long_click_5();
		    	oled_display_outputSelect("unSelected");
		    	return;
		    } 

		    if (buttons[push6]->long_press()) {
		    	// debug("oled_menu_long_click_6\n");
		    	oled_menu_long_click_6();
		    	oled_display_outputSelect("selected");
		    	return;
		    }  
		    if (buttons[push8]->long_press()) {
		    	// debug("oled_menu_long_click_7\n");
		    	oled_menu_long_click_7();
		    	return;
		    } 
		    if (buttons[push7]->long_press()) {
		    	// debug("oled_menu_long_click_8\n");
		    	oled_menu_long_click_8();
		    	return;
		    } 

		    if (buttons[push3]->long_press()) {
		        oled_menu_long_click_1(oled_menu_move_none);
		        return;
		    } 	

		    if (button_getNbrClick(click5, 1) ) {oled_menu_click_5();}     
		    if (button_getNbrClick(click6, 1) ) {oled_menu_click_6();}     
		    if (button_getNbrClick(click7, 1) ) {oled_menu_click_8();}  
		    if (button_getNbrClick(click8, 1) ) {oled_menu_click_7();}    
		    if (button_getNbrClick(click5, 2) ) {oled_choose_toggle();}     
		    if (button_getNbrClick(click6, 2) ) {}    	      
		    if (button_getNbrClick(click7, 2) ) {oled_menu_click_8_db();}    	     
		    if (button_getNbrClick(click8, 2) ) {}		    	    
	    } else {

		    if (oled_display_mod == oled_display_setting) {
		    	if (oled_setting_mod == oled_settings_toggle) {
				    if (button_getNbrClick(click5, 1) ) {om_toggle_select = !om_toggle_select; oled_choose_toggle();}     
				    if (button_getNbrClick(click5, 2) ) {oled_display_settings_off();}     
				    if (button_getNbrClick(click6, 1) ) {oled_display_settings_off();}     
		    	}
		    	if (oled_setting_mod == oled_settings_outputSelect) {
				    if (button_getNbrClick(click5, 1) ) {oled_display_settings_off();}     
				    if (button_getNbrClick(click6, 1) ) {oled_display_settings_off();}     
		    	}		    	
		    }  		    	
	    }    
    
	    // if (click3==1) {fsprintln("bp3");} 

	    // if (click2==2) {oled_menu_click_1_set();} 
	    // if (click1==2) {oled_menu_click_2_set();}  

	    // if (buttons[push2]->long_press()) {
	    //     oled_menu_long_click_1();
	    // } 	
}
#endif
