#include "config_base.h"

#ifdef WEBSERVER_ENABLE

#include "webserver_update.h"
#include "devices.h"
#include "tools.h"
#include "serverota.h"
#include "colors.h"
#include "adriwifi.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

extern devices_edit devices_edit;
extern devices devices;
extern wifi_id wifi_id;

webSocketUpdate * webSocketUpdate_array[MAX_DEVICES];
int webSocketUpdate_array_nbr = 0;

webSocketUpdate::webSocketUpdate(String add){   
    if (webSocketUpdate_array_nbr < MAX_DEVICES) {

        Serial.printf("[webSocketUpdate] : %d - %s\n", webSocketUpdate_array_nbr, add.c_str());

        name = add;
        webSocketUpdate_array[webSocketUpdate_array_nbr++] = this;

    } else Serial.printf("[webSocketUpdate] FUUUUUUUUUUULLLLLLLL\n");
}

void webSocketUpdate_loop(){
    if (!webSocketServer_isConnected)   return;
    if (websocket_client==NULL)         return;
    
    for( int i = 0; i < devices_count; i++) { 
        if(devices_array[i].device_name == "") continue;
        if (devices_array[i].isConnected != webSocketUpdate_array[i]->isConnected ) {
            // Serial.printf("\n[webSocketUpdate_loop] updtate isConnected - dn: %s - isConnected: %d\n", devices_array[i].device_name.c_str(), devices_array[i].isConnected );  
            webserverSend_list_devicesOutput_toSocket();

        }
    } 
} 


String webserverSend_list_devicesOn(){
    boolean     start = true;
    String      dn;
    String      list;
    String      list_o;
    int         oc;

    // for( int i = 0; i < MAX_DEVICES; i++) { 
    //     if (!devices_array[i].isConnected) continue;
    //     dn      = devices_array[i].device_name;
    //     if (dn == "") continue;

    //     oc = devices_array[i].output_count;
    //     for( int j = 0; j < oc; j++) {
    //         if (devices_array[i].toggleOnOff[j]) {
    //             list += devices_array[i].output_grp[j] + " - " + devices_array[i].output_location[j] + "\n";
    //         }

    //     }    
    // }  
    int cnt = 0;
    for (int i = 0; i < MAX_GROUPS; ++i) {
        String grp = devices_edit.groupes[i];
        cnt = 0;
        list_o = "";
        for( int x = 0; x < MAX_DEVICES; x++) { 
            dn = devices_array[x].device_name;
            if (dn == "") continue;
            if (!devices_array[x].isConnected) continue;
            oc = devices_array[x].output_count;
            for( int j = 0; j < oc; j++) {
                if (grp == devices_array[x].output_grp[j]) {
                    if (devices_array[x].toggleOnOff[j]) {
                        list_o += "     " + devices_array[x].output_location[j] + "\n";
                        cnt++;
                    }
                }
            }    
        }  
        if (list_o!="") {
            list += grp + "\n" + list_o;
        }
    }
    return list;
}

String webserverSend_list_devices(){
	boolean 	start = true;
	String 		s_json;
	String 		dn;
	String 		list;

    for( int j = 0; j < MAX_DEVICES; j++) { 
    	dn 		= devices_array[j].device_name;
    	list 	+= dn + ";";
    }  
    s_json = "{";
    s_json += jsonAddStringValue(start, "list_dn", list);
    s_json += "}";
    return s_json;
}
String webserverSend_list_group(){
    boolean     start = true;
    String      dn;
    String      list;

    for (int i = 0; i < devices_edit.grp_count; ++i) {
        dn      = devices_edit.groupes[i];
        list    += dn + ";";
    }  

    return list;
}

String webserverSend_list_devicesOutput(){
	boolean 	start = true;
	String 		dn;
	String 		list;
	int 		oc;

    for( int i = 0; i < MAX_DEVICES; i++) { 

    	dn 		= devices_array[i].device_name;
    	if (dn == "") continue;

        webSocketUpdate_array[i]->isConnected = devices_array[i].isConnected;

    	oc 		= devices_array[i].output_count;

    	list += dn + "*";
    	list += devices_array[i].device_ip + "*";
        for( int j = 0; j < oc; j++) {
        	list += devices_array[i].output_location[j] + ";";
        	list += devices_array[i].output_grp[j] + ";";
            list += String(devices_array[i].toggleOnOff[j]) + ";";
            list += String(j) + ";";
            int  *c = splitColor(devices_array[i].strip_color[j]);
            list += devices_array[i].strip_color[j]+";";
            list += String(devices_array[i].strip_hue[j])+"."+ String(devices_array[i].strip_sat[j])+"."+ String(devices_array[i].strip_hBri[j])+";";
            list += String(devices_array[i].trueWhite[j])+";";
            list += String(devices_array[i].isConnected)+";";
        	list += String(devices_array[i].output_selected[j])+";";
        	list += "*";

        }    
        list += "|";	

    }  
    return list;
}
String webserverSend_selected_devicesOutput(){
    boolean     start = true;
    String      dn;
    String      list;
    int         oc;

    for( int i = 0; i < MAX_DEVICES; i++) { 

        dn      = devices_array[i].device_name;
        if (dn == "") continue;
        
        int sd = oled_request_search_device(dn);
        if ( sd < 0 ) continue;

        webSocketUpdate_array[i]->isConnected = devices_array[i].isConnected;

        oc      = devices_array[i].output_count;

        list += dn + "*";
        list += devices_array[i].device_ip + "*";
        for( int j = 0; j < oc; j++) {
            list += devices_array[i].output_location[j] + ";";
            list += devices_array[i].output_grp[j] + ";";
            list += String(devices_array[i].toggleOnOff[j]) + ";";
            list += String(j) + ";";
            int  *c = splitColor(devices_array[i].strip_color[j]);
            list += devices_array[i].strip_color[j]+";";
            list += String(devices_array[i].strip_hue[j])+"."+ String(devices_array[i].strip_sat[j])+"."+ String(devices_array[i].strip_hBri[j])+";";
            list += String(devices_array[i].trueWhite[j])+";";
            list += String(devices_array[i].isConnected)+";";
            list += String(devices_array[i].output_selected[j])+";";
            list += "*";

        }    
        list += "|";    

    }  
    return list;
}

String webserverSend_string(String str, String name){
	boolean 	start = true;
	String 		s_json;

    s_json = "{";
    s_json += jsonAddStringValue(start, name, str);
    s_json += "}";

    return s_json;	
}
void webserverSend_selected_devicesOutput_toSocket() {
    if (!webSocketServer_isConnected)   return;
    if (websocket_client==NULL)         return;

    static uint8_t cnt = 0;

    String  send_str =  webserverSend_selected_devicesOutput();
    if (send_str == "") return;

    Serial.printf("send_str: %d - %s\n",cnt, send_str.c_str() );
    cnt++;

    send_str =  webserverSend_string(send_str, "l_o");
    socket_send_msg(send_str, 0);   
    yield();
}
void webserverSend_list_devicesOutput_toSocket() {
    if (!webSocketServer_isConnected)   return;
    if (websocket_client==NULL)         return;

    static uint8_t cnt = 0;

    String  send_str =  webserverSend_list_devicesOutput();
    if (send_str == "") return;

    Serial.printf("send_str: %d - %s\n",cnt, send_str.c_str() );
    cnt++;

    send_str =  webserverSend_string(send_str, "l_o");
    socket_send_msg(send_str, 0);   
    yield();
}
void webserverSend_list_group_toSocket() {

    if (!webSocketServer_isConnected)   return;
    if (websocket_client==NULL)         return;

    // if ( millis() - webSocketServer_lastUpd > 10) {

        String  send_str =  webserverSend_list_group();
                send_str =  webserverSend_string(send_str, "l_g");

        // Serial.printf("\n[webserverSend_list_devicesOutput_toSocket] : %s\n", send_str.c_str());
                
        socket_send_msg(send_str, 0);   

        // webSocketServer_lastUpd = millis();

    // }


}


String appi_rep_2_loop(String op){
    String      s_json;

    String lo = webserverSend_selected_devicesOutput(); 

    s_json = "{";
    s_json += jsonAddStringValue(true, "op", op);
    s_json += jsonAddStringValue(false, "l_o", lo);
    s_json += "}";
    return s_json;
}
String appi_rep_loop(String op){
    String      s_json;

    String lo = webserverSend_list_devicesOutput(); 
    String lg = webserverSend_list_group();

    s_json = "{";
    s_json += jsonAddStringValue(true, "op", op);
    s_json += jsonAddStringValue(false, "l_o", lo);
    s_json += jsonAddStringValue(false, "l_g", lg);
    s_json += "}";
    return s_json;
}


char PROGMEM rep_wifi_reset                         [] = "wifi_reset";
char PROGMEM rep_wifi_subnet                        [] = "wifi_subnet";
char PROGMEM rep_wifi_gateway                       [] = "wifi_gateway";
char PROGMEM rep_wifi_ip                            [] = "wifi_ip";
char PROGMEM rep_wifi_ssid                          [] = "wifi_ssid";
char PROGMEM rep_wifi_psk                           [] = "wifi_psk";
char PROGMEM rep_wifiAp_ip                          [] = "wifiAp_ip";
char PROGMEM rep_wifiAp_psk                         [] = "wifiAp_psk";
char PROGMEM rep_wifiAp_ssid                        [] = "wifiAp_ssid";
String getValue_wifi_reset()                          { wfifi_getID_fromSPIFF(); return  String(wifi_id.mod);}
String getValue_wifi_subnet()                         { wfifi_getID_fromSPIFF(); return  wifi_id.subnet;}
String getValue_wifi_gateway()                        { wfifi_getID_fromSPIFF(); return  wifi_id.gateway;}
String getValue_wifi_ip()                             { wfifi_getID_fromSPIFF(); return  wifi_id.ip;}
String getValue_wifi_ssid()                           { wfifi_getID_fromSPIFF(); return  wifi_id.ssid;}
String getValue_wifi_psk()                            { wfifi_getID_fromSPIFF(); return  wifi_id.pswd;}
String getValue_wifiAp_ip()                           { wfifi_getID_fromSPIFF(); return  wifi_id.ap_ip;}
String getValue_wifiAp_psk()                          { wfifi_getID_fromSPIFF(); return  wifi_id.ap_psk;}
String getValue_wifiAp_ssid()                         { wfifi_getID_fromSPIFF(); return  wifi_id.ap_ssid;}
PROGMEM requestReponseLists reponseLists [] = {

{ rep_wifi_reset                            ,rft_number     ,&getValue_wifi_reset                       },
{ rep_wifi_ip                               ,rft_string     ,&getValue_wifi_ip                          },
{ rep_wifi_ssid                             ,rft_string     ,&getValue_wifi_ssid                        },
{ rep_wifi_psk                              ,rft_string     ,&getValue_wifi_psk                         },
{ rep_wifiAp_ip                             ,rft_string     ,&getValue_wifiAp_ip                        },
{ rep_wifiAp_psk                            ,rft_string     ,&getValue_wifiAp_psk                       },
{ rep_wifiAp_ssid                           ,rft_string     ,&getValue_wifiAp_ssid                      },

{ rep_wifi_subnet                           ,rft_string     ,&getValue_wifi_subnet                      },
{ rep_wifi_gateway                          ,rft_string     ,&getValue_wifi_gateway                     },

};
uint8_t reponseListsCount = ARRAY_SIZE(reponseLists);


String setReponseJson(int mod, String transmit_request, boolean display){

   if (display) fsprintf("\n[setReponseJson] : %d\n", reponseListsCount);

   boolean start = true;
   char buf[255];

   String s_json = "{";

   s_json += jsonAddStringValue(start, "op", "pattern_value");
   start = false;


   boolean exclued;
   for (int i = 0; i < reponseListsCount; ++i) {

         // exclued = false;
         // if (mod == 1) {
         //    for (int j = 0; j < reponseListsExluedCount; ++j) {
         //       if (reponseLists[i].id == reponseListsExlued[j].id) {exclued = true; break;}
         //    }

         // }
         // if (exclued) continue;

         String value   =  reponseLists[i].getValue();
         char * name    =  reponseLists[i].id;
                           sprintf(buf, "%s",name );

         if ( value != "continueloop") {    

               // if ((desc == "") && (request != "")) {desc = request_getDescriptions(request);}
               
            if (reponseLists[i].type == rft_number) {
               s_json += jsonAddIntValue(start, String(buf), value);
            }
            if (reponseLists[i].type == rft_string) {
               s_json += jsonAddStringValue(start, String(buf), value);
            }  
            if (start) start = false;
            // if (display) {                 
            //    Serial.printf("\t[%d] = reponse: %s - fileType: %s - value: %s\n", 
            //       i,
            //       buf, 
            //       request_reponse_fileTypes_toString(reponseLists[i].type).c_str(),
            //       value.c_str()
            //    );
            // }
            
         }
   }
   // if (mod == 0) s_json += displayPatternInfo(pattern.getName(), false, 0);
   s_json += "}";
   
   if (display) Serial.printf("Final json:\n\t%s\n[setReponseJson Done]\n", s_json.c_str());
   return s_json;   
}
#endif