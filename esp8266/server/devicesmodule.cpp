#include "devicesmodule.h"

#include <adri_tools.h>

#define DEBUG

int devices_count = 0;

devices devices_array[MAX_DEVICES];


devices * devicesPtr;
devices * devicesPtrGet(){return devicesPtr;}
devices::devices(){devicesPtr=this;}

void devices::addDevice(String dn, String str) {
    if (deviceIsListed(dn)) {
        return;
    }
    
    if (devices_count < MAX_DEVICES) {
        
        Serial.printf("[addDevice] ajout d'un nouveau device : %s\n",dn.c_str());

        output_editPtr_get()->device_add(str, devices_count);

        devices_array[devices_count].isConnected = false;
        devices_array[devices_count].isConnected_lastTime = millis();

        devices_count++;
    }
    else {
        fsprintln("[addDevice] : LIMITE ATTEINTE");
    }
}
boolean devices::deviceIsListed(String dn) {
    boolean ret = false;
    for( int j = 0; j < MAX_DEVICES; j++) { 
        if (dn == devices_array[j].device_name) {
            ret = true;
            break;
        }
    }  
    return ret;
}
int devices::getListPos(String dn) {
    int ret = -1;
    for( int j = 0; j < MAX_DEVICES; j++) { 
        if (dn == devices_array[j].device_name) {
            ret = j;
            break;
        }
    }   
    return ret;
}
int devices::geOutputPosByName(int dPos, String search) {
    int ret = -1;
    int cnt = devices_array[dPos].output_count;
    for( int j = 0; j < cnt; j++) { 
        if (search == devices_array[dPos].output_location[j]) {
            ret = j;
            break;
        }
    }   
    return ret;
}
String devices::getLib(String dn) {
    String ret = "";
    int pos = getListPos(dn);
    if (pos < 0) return ret;
    ret = devices_array[pos].device_lib;
    return ret;
}

void devices::deviceIsConnected(String dn, bool state){
    int pos = getListPos(dn);
    if (pos >= 0) devices_array[pos].isConnected_lastTime = millis();
}

fs(devicesPrint_dn,         "name");
fs(devicesPrint_lib,        "lib");
fs(devicesPrint_ip,         "ip");
fs(devicesPrint_oc,         "oc");
fs(devicesPrint_connected,  "connected");
fs(devicesPrint_room,       "room");
fs(devicesPrint_grp,        "grp");
fs(devicesPrint_location,   "location");
fs(devicesPrint_toggle, "   toggle");
fs(devicesPrint_trueWhite,  "trueWhite");
fs(devicesPrint_os,         "os");
fs(devicesPrint_color,      "color");
fs(devicesPrint_bri,        "bri");
fs(devicesPrint_hue,        "hue");
fs(devicesPrint_sat,        "sat");
fs(devicesPrint_hBri,       "hBri");
void devices::print(){
	fsprintf("\n[devices::print]\n")
	fsprintf("[%15s][%15s][%15s][%15s][%15s]\n", 
		fsget(devicesPrint_dn).c_str(),
		fsget(devicesPrint_lib).c_str(),
		fsget(devicesPrint_ip).c_str(),
		fsget(devicesPrint_oc).c_str(),
		fsget(devicesPrint_connected).c_str()
	);	
	fsprintf("\t[%15s][%15s][%15s][%15s][%15s][%15s][%15s][%15s][%15s][%15s][%15s]", 
		fsget(devicesPrint_room).c_str(),
		fsget(devicesPrint_grp).c_str(),
		fsget(devicesPrint_location).c_str(),
		fsget(devicesPrint_toggle).c_str(),
		fsget(devicesPrint_trueWhite).c_str(),
		fsget(devicesPrint_os).c_str(),
		fsget(devicesPrint_color).c_str(),
		fsget(devicesPrint_bri).c_str(),
		fsget(devicesPrint_hue).c_str(),
		fsget(devicesPrint_sat).c_str(),
		fsget(devicesPrint_hBri).c_str()
	);		
	for( int j = 0; j < devices_count; j++) { 
		fsprintf("\n[%15s][%15s][%15s][%15d][%15d]\n", 
			devices_array[j].device_name.c_str(),
			devices_array[j].device_lib.c_str(),
			devices_array[j].device_ip.c_str(),
			devices_array[j].output_count,
			devices_array[j].isConnected
		);
		int pos =  devices_array[j].output_count;
		for( int A = 0; A < pos; A++) { 
			fsprintf("\t[%15s][%15s][%15s][%15d][%15d][%15d][%15s][%15d][%15d][%15d][%15d]\n", 
				devices_array[j].device_room[A].c_str(),
				devices_array[j].output_grp[A].c_str(),
				devices_array[j].output_location[A].c_str(),
				devices_array[j].toggleOnOff[A],
				devices_array[j].trueWhite[A],
				devices_array[j].output_selected[A],
				devices_array[j].strip_color[A].c_str(),
				devices_array[j].strip_bri[A],
				devices_array[j].strip_hue[A],
				devices_array[j].strip_sat[A],
				devices_array[j].strip_hBri[A]
			);
		} 		
	}  
}

unsigned long   timer_udp_checkClient_lastChange_1;
int             timer_udp_checkClient_1 = 3000;
void devices::upd_udpDevices(){
    #ifdef OLED_ENABLE
        if (hsvb_loop != hsv_loop_n) return;
    #endif
    // deviceUdp_getClient_loop();
    
    String s;

    #ifdef DEBUG
        char buffer[120];
    #endif

    if ( (millis() - timer_udp_checkClient_lastChange_1) > (timer_udp_checkClient_1)) {  
        timer_udp_checkClient_lastChange_1 = millis();

        for( int i = 0; i < devices_count; i++) { 

            if(devices_array[i].device_name == "") continue;

            if (( millis() - devices_array[i].isConnected_lastTime < 5000) ) {  
                if (!devices_array[i].isConnected) {

                    devices_array[i].isConnectedCount=0;
                    devices_array[i].isConnected = true;
                    devices_array[i].isConnected_lastTime = millis();

                    #ifdef OLED_ENABLE
                        oled_menu_create_items_device();
                    #endif
                    // device_display(devices_array[i].device_name, 0, &s);
                    // webserverSend_list_devicesOutput_toSocket();
                    #ifdef DEBUG
                        sprintf(buffer, "[im re her] %s\n", devices_array[i].device_name.c_str());  
                        // debug(buffer); 
                        fsprintf("\n%S",buffer);
                    #endif

                } else devices_array[i].isConnected = true;
            }  else {

                devices_array[i].isConnectedCount+=1;

                if (devices_array[i].isConnectedCount > 1) {

                    devices_array[i].isConnected = false;

                    #ifdef OLED_ENABLE
                        oled_menu_create_items_device();
                    #endif
                    // webserverSend_list_devicesOutput_toSocket();
                    #ifdef DEBUG
                        sprintf(buffer, "[im not her] %s\n", devices_array[i].device_name.c_str());  
                        // debug(buffer); 
                        fsprintf("\n%S",buffer);
                    #endif

                }
            }                
        }  
    }      
}




