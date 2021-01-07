#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#include <FS.h>   
#include <Arduino.h>
#include <dac_command.h>

#include "config_base.h"
#include "devices.h"
#include "devices_display.h"
#include "adriwifi.h"
#include "tools.h"
#include "udp.h"

#ifdef WEBSERVER_ENABLE
    #include "webserver_update.h"
#endif

#ifdef OLED_ENABLE
    #include "oled_menu.h"
    extern mod_hsv_loop hsvb_loop;
#endif

#include "devices_mod.h"

extern "C" {
    #include "user_interface.h"
}


// #define DEBUG

int devices_count 		= 0;

devices devices_array[MAX_DEVICES];

devices::devices(){}
devices devices; 

devices_edit::devices_edit(){}
devices_edit devices_edit; 



void devices_edit_setup (){
    // SPIFFS.remove("/devcices_edit.txt");

    devices_edit.parametres_spiffToArray();
    
    devices_edit.parametres_add("plafond",           "grp");
    devices_edit.parametres_add("salle a manger",    "grp");

    devices_edit.parametres_add("center",            "roomgrp_chambre");
    devices_edit.parametres_add("plafond",           "roomgrp_chambre");
    devices_edit.parametres_add("ecran",             "roomgrp_chambre");
    devices_edit.parametres_add("bureau",            "roomgrp_chambre");
    devices_edit.parametres_add("lit",               "roomgrp_chambre");
    devices_edit.parametres_add("salle a manger",    "roomgrp_salon");

    devices_edit.parametres_display();
    devices_edit.group_setup();
    devices_edit.room_setup();
    devices_edit.group_display();
    devices_edit.room_display();    
}




void devices_edit::group_setup(){

    int count = 0;

    for (int i = DG_BUREAU; i < DG_UNKNOW; ++i) {
        String name = devices_grp_to_string(mod_devices_grp_array[i]); 
        if (name=="")continue;
        groupes[count] = devices_grp_to_string(mod_devices_grp_array[i]); 
        count++;       
    }   
    grp_count = count;

    int pos = parametres_getPos("grp");

    if (pos < 0 ) return;

    String value = literal_value("value", parametres[pos]); 

    String list[20];

    int expCnt = explode(value, ';', list);
    for( int j = 0; j < expCnt-1; j++) {
        groupes[grp_count] = list[j]; 
        grp_count++;
    }

}
void devices_edit::group_display(){
    Serial.printf("\n[group_display] count: %d\n", grp_count);
    for (int i = 0; i < MAX_GROUPS; ++i) Serial.printf("[%d] - %s\n", i, groupes[i].c_str());
}


void devices_edit::room_setup(){

    int count = 0;

    for (int i = DR_CH; i < DR_UK; ++i) {
        String name = devices_room_to_string(mod_device_room_array[i]); 
        if (name=="")continue;
        rooms[count] = devices_room_to_string(mod_device_room_array[i]); 
        count++;       
    }   
    room_count = count;

    int pos = parametres_getPos("room");

    if (pos < 0 ) return;

    String value = literal_value("value", parametres[pos]); 

    String list[20];

    int expCnt = explode(value, ';', list);
    for( int j = 0; j < expCnt-1; j++) {
        rooms[room_count] = list[j]; 
        room_count++;
    }

}
void devices_edit::room_display(){
    Serial.printf("\n[group_display] count: %d\n", grp_count);
    for (int i = 0; i < MAX_GROUPS; ++i) Serial.printf("[%d] - %s\n", i, rooms[i].c_str());
}



void devices_edit::parametres_display() {
    Serial.println("\n[parametres_display]\n");
    for (int i = 0; i < MAX_PARAM; ++i) {
        Serial.printf("[%d] %s\n", i, parametres[i].c_str());
    }  
}

int devices_edit::parametres_spiffToArray() {
    String path = "/devcices_edit.txt";
    File    file        = SPIFFS.open(path, "r");
    String  line        = "";
    int     countParam  = 0;

    while (file.position()<file.size()) {
        String xml = file.readStringUntil('\n');
        if (xml != "") {
            parametres[countParam] = xml;
            countParam++;
        }
        yield();
    } 

    file.close();

    return countParam;
}

int devices_edit::parametres_getPos(String s_op) {
    int ret = -1;
    for (int i = 0; i < MAX_PARAM; ++i) {
        String op = literal_value("op", parametres[i]);
        if (op == s_op) {
            ret = i;
            break;
        } 
    }
    return ret;
}
void devices_edit::parametres_add(String s_value, String s_op) {

    String path = "/devcices_edit.txt";

        File file = SPIFFS.open(path, "r");

        String line = "";


        if (!file) {

            line =  literal_item("op",  s_op);
            line += literal_item("value",  s_value + ";");

            
            file = SPIFFS.open(path, "w");
            file.println(line);
            file.close();

            parametres[0]=line;

        } else {

            file.close();
                 
            int countParam = parametres_spiffToArray();

            // Serial.printf("\ns_value %s - s_op %s\n", s_value.c_str(), s_op.c_str());
            // Serial.printf("countParam %d\n", countParam);

            String value = "";

            int pos = -1;

            for (int i = 0; i < MAX_PARAM; ++i) {
                String op = literal_value("op", parametres[i]);
                if (op == s_op) {
                    value = literal_value("value", parametres[i]);
                    pos = i;
                    break;
                } 
            }

            String  list[20];
            int     expCnt      = explode(value, ';', list);
            String  newValue    = "";
            for( int j = 0; j < expCnt-1; j++) {
                if (s_value == list[j]) continue;
                newValue += list[j] + ";"; 
            }
            newValue += s_value + ";"; 

            // Serial.printf("pos %d\n", pos);
            // Serial.printf("newValue %s\n", newValue.c_str());
            
            if (pos != -1) {
                line =  literal_item("op",  s_op);
                line += literal_item("value",  newValue);
                parametres[pos] = line;

            }  else {
                
                int newPos = 0;
                for (int i = 0; i < MAX_PARAM; ++i) {
                    // Serial.printf("len line: %d - %d - %s\n",i, parametres[i].length(), parametres[i].c_str());
                    if (parametres[i].length() > 1) {

                    } else {
                        newPos = i;
                        break;                        
                    }
                }  

                // Serial.printf("NEWpos %d\n", newPos);

                line =  literal_item("op",  s_op);
                line += literal_item("value",  newValue);

                parametres[newPos] = line;                
            }


            line = "";
            for (int i = 0; i < MAX_PARAM; ++i) {
                if (parametres[i] != "") line += parametres[i]+"\n";
            }  
            
            file = SPIFFS.open(path, "w");
            file.println(line);                    
            file.close();

            // Serial.printf("\nDONE\n");
         
   }

}

void devices_edit::parametres_remove(String s_value, String s_op) {

    String path = "/devcices_edit.txt";

        File file = SPIFFS.open(path, "r");

        String line = "";


        if (!file) {


        } else {

            file.close();
                 
            parametres_spiffToArray();

            String value = "";

            int pos = -1;

            for (int i = 0; i < MAX_PARAM; ++i) {
                String op = literal_value("op", parametres[i]);
                if (op == s_op) {
                    value = literal_value("value", parametres[i]);
                    pos = i;
                    break;
                } 
            }
            if (pos < 0) return;

                String  list[20];
                int     expCnt      = explode(value, ';', list);
                String  newValue    = "";
                for( int j = 0; j < expCnt-1; j++) {
                    if (s_value == list[j]) continue;
                    newValue += list[j] + ";"; 
                }
                line =  literal_item("op",  s_op);
                line += literal_item("value",  newValue);                
                parametres[pos] = line;

            line = "";
            for (int i = 0; i < MAX_PARAM; ++i) {
                line += parametres[i]+"\n";
            }  
            
            file = SPIFFS.open(path, "w");
            file.println(line);                    
            file.close();

            parametres_spiffToArray();
         
   }

}


// String s = "<dn>DOUBLEAPPLIQUE</dn><oc>2</oc><os>0;0;0;184.145.77;0.0.0;255;1;0;33;177;255;*1;1;0;190.139.77;0.0.255;255;1;0;30;177;255;*</os>#<dn>relay_lampe_adri</dn><oc>1</oc><os>0;0;0;0.0.0;0.0.0;0;0;0;0;0;0;*</os>#<dn>ENTER_RIGHT</dn><oc>2</oc><os>0;1;0;43.43.43;0.0.0;255;1;0;210;0;44;*1;1;0;41.27.8;0.0.0;184;1;0;31;219;61;*</os>#<dn>BUREAU_ECRAN</dn><oc>6</oc><os>0;1;0;138.115.69;0.0.0;166;1;0;34;160;186;*1;0;0;152.146.247;0.0.0;166;0;0;177;108;133;*2;0;0;205.194.146;0.0.0;166;1;0;38;108;138;*3;1;0;138.115.69;0.0.0;166;1;0;34;160;186;*4;0;0;205.194.146;0.0.0;166;1;0;38;108;138;*5;0;0;205.194.146;0.0.0;166;1;0;38;108;138;*</os>#<dn>BUREAU_HAUT</dn><oc>1</oc><os>0;0;0;207.159.114;0.0.0;245;1;0;28;140;138;*</os>#<dn>AMPLI</dn><oc>1</oc><os>0;0;0;24.11.3;0.0.0;133;1;0;24;231;34;*</os>#<dn>TDN</dn><oc>2</oc><os>0;0;0;43.38.33;0.0.0;138;1;0;30;82;50;*1;0;0;43.38.33;0.0.255;138;1;0;30;82;50;*</os>#";


boolean output_isRelay(mod_outputs_type type) {
    if (type == OT_RGBSTRIP) return false;
    if (type == OT_RGBWSTRIP) return false;
    return true;
}
boolean output_isStrip(mod_outputs_type type) {
    if (type == OT_RELAY_LP) return false;
    if (type == OT_RELAY_AC) return false;
    return true;
}
boolean output_isRGBWStrip(mod_outputs_type type) {
    if (type == OT_RGBWSTRIP) return true;
    return false;
}
boolean output_isRGBStrip(mod_outputs_type type) {
    if (type == OT_RGBSTRIP) return true;
    return false;
}

int getDouble(String * array, int max, String search){
    int ret = 0;
    for (int i = 0; i < max; ++i) {
        if (array[i] == search) ret++; 
    }    
    return ret;
}
void devices::addDevice(String dn, String str) {
    if (deviceIsListed(dn)) {
        return;
    }
    
    if (devices_count < MAX_DEVICES) {
        
        Serial.printf("[addDevice] ajout d'un nouveau device : %s\n",dn.c_str());

        device_add(str, devices_count);

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


// void deviceUdp_getClient(){
//     String req = literal_item("op",  "hello_client");
//     udpMulti_send(req);
// }

// unsigned long deviceUdp_get_loop_lastCall;
// void deviceUdp_getClient_loop(){
//     if ( millis() - deviceUdp_get_loop_lastCall > 1500) {
//         deviceUdp_get_loop_lastCall = millis();
//         deviceUdp_getClient();
//         yield();
//     }
// }


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
                        debug(buffer); 
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
                        debug(buffer); 
                    #endif

                }
            }                
        }  
    }      


}


#ifdef DEVICEFROMSPIFF

    deviceSpiff * deviceSpiff_array[MAX_DEVICES];
    int deviceSpiff_array_nbr = 0;

    deviceSpiff::deviceSpiff(String add){   
        if (deviceSpiff_array_nbr < MAX_DEVICES) {
            name = add;
            deviceSpiff_array[deviceSpiff_array_nbr++] = this;
            devices_count = deviceSpiff_array_nbr;
        } 
    }

    int deviceSpiff_toArray() {

        String  path = "/devcices_save.txt";
        File    file = SPIFFS.open(path, "r");

        if (!file) return-1;

        int     countParam  = 0;
        String  xml;

        while (file.position()<file.size()) {
            xml = file.readStringUntil('\n');
            if (xml != "") {
                new deviceSpiff(xml);

                #ifdef WEBSERVER_ENABLE
                    new webSocketUpdate(xml);
                #endif

                int pos = deviceSpiff_array_nbr - 1;
                devices_array[pos].device_name = literal_value("dn", deviceSpiff_array[pos]->name);
                devices_array[pos].isConnected = false;
                devices_array[pos].isConnected_lastTime = millis();
                countParam++;
            }
            yield();
        } 
        file.close();
        return countParam;
    }

    void deviceSpiff_write() {

        #ifdef DEBUG
            Serial.printf("\n[deviceSpiff_write]\n");
        #endif

        String  path = "/devcices_save.txt";
        File    file = SPIFFS.open(path, "w");
        if (file) { 
            String lines = "";
            for( int j = 0; j < deviceSpiff_array_nbr; j++) {

                String l_n = deviceSpiff_array[j]->name;

                #ifdef DEBUG
                    Serial.printf("write line - pos: %d - name: %s\n", j, l_n.c_str());
                #endif
                    
                if ( j < deviceSpiff_array_nbr -1 )  lines += l_n + "\n";
                else lines += l_n;

            }      

            file.print(lines);   
            file.close();        
        }
        #ifdef DEBUG
            Serial.printf("[deviceSpiff_write] END\n");
        #endif
    }

    int deviceSpiff_search(String dn){
        int cnt = -1;
        for( int j = 0; j < deviceSpiff_array_nbr; j++) {
            String l_n = literal_value("dn", deviceSpiff_array[j]->name);
            if (dn==l_n)  {cnt=j;}
        }
        return cnt;
    }

    void deviceSpiff_print(){
        Serial.printf("\n[deviceSpiff_print] cnt : %d\n", deviceSpiff_array_nbr);
        for( int j = 0; j < deviceSpiff_array_nbr; j++) {
            String l_n = deviceSpiff_array[j]->name;
            Serial.printf("pos: %d - name: %s\n", j, l_n.c_str());
        }
        Serial.printf("[deviceSpiff_print] END\n");
    }

    void deviceSpiff_load(){
        Serial.printf("\n[deviceSpiff_load]\n");

        deviceSpiff_toArray();

        Serial.printf("[deviceSpiff_load] END\n");

        deviceSpiff_print();
    }

    void deviceSpiff_addDevice(String dn){

        int search = deviceSpiff_search(dn);
        if (search < 0) {
            String name = literal_item("dn", dn);
            new deviceSpiff(name);

            #ifdef WEBSERVER_ENABLE
                new webSocketUpdate(name);
            #endif

            deviceSpiff_write();
        }

    }

    void deviceSpiff_addData(String dn, String data){

        int pos = deviceSpiff_search(dn);
        if (pos >= 0) { device_add(data, pos); }
    }

#endif

void device_add(String s, int pos){

    // Serial.printf("\n[device_add] pos: %d - name: %s\n",pos,s.c_str() );
    
    String os;

    if(!devices_array[pos].isAdded){

        os = literal_value("0", s);
        devices_array[pos].device_name = os;

        os = literal_value("1", s);
        devices_array[pos].device_ip = os;

        os = literal_value("2", s);
        devices_array[pos].output_count = os.toInt();

        os = literal_value("3", s);
        set_output_info(os, devices_array[pos].output_pin); 

        os = literal_value("4", s);
        set_output_info(os, devices_array[pos].output_type);

        os = literal_value("5", s);
        set_output_info(os, devices_array[pos].device_room);

        os = literal_value("6", s);
        set_output_info(os, devices_array[pos].output_grp);

        os = literal_value("7", s);
        set_output_info(os, devices_array[pos].output_location);

    }

    os = literal_value("8", s);
    set_output_info(os, devices_array[pos].toggleOnOff);

    // os = literal_value("9", s);
    // set_output_info(os, devices_array[pos].output_state);

    // os = literal_value("10", s);
    // set_output_info(os, devices_array[pos].strip_size);

    os = literal_value("11", s);
    set_output_info(os, devices_array[pos].strip_color);

    // os = literal_value("12", s);
    // set_output_info(os, devices_array[pos].strip_colorOff);

    os = literal_value("13", s);
    set_output_info(os, devices_array[pos].strip_bri);

    os = literal_value("14", s);
    set_output_info(os, devices_array[pos].output_selected);

    // os = literal_value("15", s);
    // set_output_info(os, devices_array[pos].patternStrip_selected);

    if (!devices_array[pos].isAdded) devices_array[pos].device_lib = literal_value("16", s);

    os = literal_value("17", s);
    set_output_info(os, devices_array[pos].strip_hue);

    os = literal_value("18", s);
    set_output_info(os, devices_array[pos].strip_sat);

    os = literal_value("19", s);
    set_output_info(os, devices_array[pos].strip_hBri);    

    os = literal_value("20", s);
    set_output_info(os, devices_array[pos].trueWhite);    



    if (!devices_array[pos].isAdded) devices_array[pos].isAdded = true;             
}

String set_output_info(String s, int *pos){
    String list[7];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = list[j].toInt();
    }
}
String set_output_info(String s, mod_outputs_type *pos){
    String list[7];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = output_type_string_to_mod(list[j]);
    }
}
String set_output_info(String s, mod_devices_room *pos){
    String list[7];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = devices_room_string_to_mod(list[j]);
    }
}
String set_output_info(String s, mod_devices_grp *pos){
    String list[7];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = devices_grp_string_to_mod(list[j]);
    }
}
String set_output_info(String s, String *pos){
    String list[7];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = list[j];
    }
}
String set_output_info(String s, bool *pos){
    String list[7];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        if (list[j] == "0") pos[j] = false;
        else pos[j] = true;
    }
}
String set_output_info(String s, uint8_t *pos){
    String list[7];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = list[j].toInt();
    }
}


String get_output_info(mod_outputs_type *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = output_type_to_string(pos[j]);
            if ( j < count -1) ret += v + ";";
            else ret += v;
    }
    return ret;
}
String get_output_info(mod_devices_room *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = devices_room_to_string(pos[j]);
            if ( j < count -1) ret += v + ";";
            else ret += v;
    }
    return ret;
}
String get_output_info(mod_devices_grp *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = devices_grp_to_string(pos[j]);
            if ( j < count -1) ret += v + ";";
            else ret += v;
    }
    return ret;
}
String get_output_info(String *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = pos[j];
            if ( j < count -1) ret += v + ";";
            else ret += v;
    }
    return ret;
}
String get_output_info(bool *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = String(pos[j]);
            if ( j < count -1) ret += v + ";";
            else ret += v;
    }
    return ret;
}
String get_output_info(int *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = String(pos[j]);
            if ( j < count -1) ret += v + ";";
            else ret += v;
    }
    return ret;
}
String get_output_info(uint8_t *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = String(pos[j]);
            if ( j < count -1) ret += v + ";";
            else ret += v;
    }
    return ret;
}

#ifdef OLED_UPD_WEBSOCKET

    oled_requests oled_request;

    int oled_request_search_device(String search){
        int ret = -1;
        for( int i = 0; i < oled_request.cnt; i++) { 
            if(search == oled_request.device[i][0]) {
                ret = i;
                break;
            }
        }
        Serial.printf("[oled_request_search_device] : %s - ret: %d\n",search.c_str(), ret );
        return ret;
    }
    void oled_request_upd_device(String search){
        int sc = oled_request_search_device(search);
        if (sc >= 0) { oled_request.device[sc][1] = "1"; }
    }

    void oled_request_end(){
        for( int i = 0; i < MAX_DEVICES; i++) { 
            oled_request.device[i][0] = "";
            oled_request.device[i][1] = "";
        }
        oled_request.send = false;
    }
    void oled_request_start(){
        oled_request.send = true;
    }
    void oled_request_clear(){
        for( int i = 0; i < MAX_DEVICES; i++) { 
            oled_request.device[i][0] = "";
            oled_request.device[i][1] = "";
        }
        oled_request.cnt = 0;
    }
    String oled_request_sendString(){
        String s = "";
        for( int i = 0; i < oled_request.cnt; i++) { 
            s += oled_request.device[i][0] + ";";
        }
        String send;
        send =  literal_item("op",  "upd_websocket");
        send += literal_item("msg", s);
        udpMulti_send(send);        
    }
    boolean oled_request_canSend(){
        int cnt = 0;


        // String d = "\ncansend - cnt: " + String(oled_request.cnt) + "\n";
        // debug(&d);

        for( int i = 0; i < oled_request.cnt; i++) { 
            if(oled_request.device[i][1] != "") {  
                cnt++;
                Serial.printf("[oled_request_canSend] add: %s - %d/%d\n", oled_request.device[i][0].c_str(), cnt, oled_request.cnt);          

            } else {
                String send;
                send =  literal_item("op",  "get_rep");
                send += literal_item("dn", oled_request.device[i][0]);   
                udpMulti_send(send);           
            }

                // debug(&d);        
        }
        boolean ret = false;

        if ( (cnt) > (oled_request.cnt-1)) {
            String d = "cnt: " + String(cnt) + "\n";
            debug(&d);        
            ret=true;
        }

        return ret;
    }
    int oled_request_cnt(){
        int cnt = 0;
        for( int i = 0; i < oled_request.cnt; i++) { 
            if(oled_request.device[i][1] != "") {  
                cnt++;
            } 
        }
        return cnt;
    }
#endif



String device_setOp(String dn) {
    String ret = "";
    String lib = devices.getLib(dn);
    if (lib == "multidevice")   ret = "lp";
    if (lib == "multiLampe")    ret = "request_lp";
    return ret;
}
char * device_set_req(char * cmd) {
    char * ret ;
    ret = cmd;
    if (cmd == req_ss_lampOnOff)    {ret = req_lampOnOff;}
    if (cmd == req_ss_ledBri)       {ret = req_ledBri;}
    if (cmd == req_ss_lampBri)      {ret = req_lampBri;}
    if (cmd == req_ss_lampSat)      {ret = req_lampSat;}
    if (cmd == req_ss_lampHue)      {ret = req_lampHue;}
    return ret;
}
String device_setVal(String dn, char * cmd, String val, int pos) {
    String ret = "";
    String lib = devices.getLib(dn);
    if (lib == "multidevice")   {ret = String(pos) + "|" + val;}
    if (lib == "multiLampe")    {
        if (cmd == req_ss_lampOnOff) {
            if (val == "1") ret = "off";
            if (val == "0") ret = "on";
        }
    }
    return ret;
}
String device_setVal(char * cmd, String val) {
    String ret = "";
    if (cmd == req_ss_lampOnOff) {
        if (val == "1") ret = "off";
        if (val == "0") ret = "on";
    }
    return ret;
}


String c_to_string(char * name){
    char buffer[80];
    sprintf(buffer, "%s", name);
    return String(buffer);
}
String request_set_format(char * cmd, String value, String e, String s){

    String ret = e + c_to_string(cmd) + s + value;
    return ret;
}
String request_set_format(char * cmd, String e, String s){

    String ret = e + c_to_string(cmd) + s ;
    return ret;
}