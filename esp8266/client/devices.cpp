#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#include <dac_command.h>
#include <arduino.h>
#include <ESP8266WiFi.h>

#include "devices_mod.h"
#include "device_def.h"
#include "devices.h"
#include "tools.h"
#include "adriwifi.h"
#include "request.h"

#include "device_init.h"

extern "C" {
    #include "user_interface.h"
}


extern wifi_id wifi_id;


String value_get_device_name()      {return device_name;}
String value_get_device_ip()        {return wifi_id.ip;}
String value_get_output_count()     {return String(output_count);}
String value_get_output_pin()       {String ret = get_output_info("", output_pin,   ARRAY_SIZE(output_pin));    return ret;}
String value_get_output_type()      {String ret = get_output_info("", output_type,  ARRAY_SIZE(output_type));   return ret;}
String value_get_device_room()      {String ret = get_output_info("", device_room,  ARRAY_SIZE(device_room));   return ret;}
String value_get_output_grp()       {String ret = get_output_info("", output_grp,   ARRAY_SIZE(output_grp));    return ret;}
String value_get_output_location()  {String ret = get_output_info("", output_location,  ARRAY_SIZE(output_location));   return ret;}
String value_get_toggleOnOff()      {String ret = get_output_info("", toggleOnOff,  ARRAY_SIZE(toggleOnOff));   return ret;}
String value_get_output_state()     {String ret = get_output_info("", output_state, ARRAY_SIZE(output_state));  return ret;}
String value_get_strip_size()       {String ret = get_output_info("", strip_size,   ARRAY_SIZE(strip_size));    return ret;}
String value_get_strip_color()      {String ret = get_output_info("", strip_color,  ARRAY_SIZE(strip_color));   return ret;}
String value_get_strip_colorOff()   {String ret = get_output_info("", strip_colorOff,   ARRAY_SIZE(strip_colorOff));    return ret;}
String value_get_strip_bri()        {String ret = get_output_info("", strip_bri,    ARRAY_SIZE(strip_bri));     return ret;}
String value_get_output_selected()  {String ret = get_output_info("", output_selected, ARRAY_SIZE(output_selected));    return ret;}
String value_get_patternStrip_selected()    {String ret = get_output_info("", patternStrip_selected, ARRAY_SIZE(patternStrip_selected));return ret;}
String value_get_device_lib()       {return device_lib;}
String value_get_strip_hue()        {String ret = get_output_info("", strip_hue,    ARRAY_SIZE(strip_hue));     return ret;}
String value_get_strip_sat()        {String ret = get_output_info("", strip_sat,    ARRAY_SIZE(strip_sat));     return ret;}
String value_get_strip_hBri()       {String ret = get_output_info("", strip_hBri,   ARRAY_SIZE(strip_hBri));    return ret;}
String value_get_trueWhite()        {String ret = get_output_info("", trueWhite,    ARRAY_SIZE(trueWhite));     return ret;}

void value_set_device_name(String value)            {device_name    = value;}
void value_set_device_ip(String value)              {wifi_id.ip     = value;}
void value_set_output_count(String value)           {output_count   = value.toInt();}

void value_set_output_pin(String value)             {set_output_int(value,          output_pin);}
void value_set_output_type(String value)            {set_output_type(value,         output_type);}
void value_set_device_room(String value)            {set_output_room(value,         device_room);}
void value_set_output_grp(String value)             {set_output_devices_grp(value,  output_grp);}
void value_set_output_location(String value)        {set_output_string(value,       output_location);}
void value_set_toggleOnOff(String value)            {set_output_bool(value,         toggleOnOff);}
void value_set_output_state(String value)           {set_output_int(value,          output_state);}
void value_set_strip_size(String value)             {set_output_int(value,          strip_size);}
void value_set_strip_color(String value)            {set_output_string(value,       strip_color);}
void value_set_strip_colorOff(String value)         {set_output_string(value,       strip_colorOff);}
void value_set_strip_bri(String value)              {set_output_int8(value,         strip_bri);}
void value_set_output_selected(String value)        {set_output_int(value,          output_selected);}
void value_set_patternStrip_selected(String value)  {set_output_int(value,          patternStrip_selected);}
void value_set_device_lib(String value)             {device_lib     = value;}
void value_set_strip_hue(String value)              {set_output_int8(value,         strip_hue);}
void value_set_strip_sat(String value)              {set_output_int8(value,         strip_sat);}
void value_set_strip_hBri(String value)             {set_output_int8(value,         strip_hBri);}
void value_set_trueWhite(String value)              {set_output_bool(value,         trueWhite);}


void set_output_bool(String s, boolean *pos){
    String list[8];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = list[j].toInt();
    }
}
void set_output_int(String s, int *pos){
    String list[8];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = list[j].toInt();
    }
}
void set_output_int8(String s, uint8_t *pos){
    String list[8];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = list[j].toInt();
    }
}
void set_output_string(String s, String *pos){
    String list[8];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = list[j];
    }
}

void set_output_type(String s, mod_outputs_type *pos){
    String list[8];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = output_type_string_to_mod(list[j]);
    }
}
void set_output_room(String s, mod_devices_room *pos){
    String list[8];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = devices_room_string_to_mod(list[j]);
    }
}
void set_output_devices_grp(String s, mod_devices_grp *pos){
    String list[8];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = devices_grp_string_to_mod(list[j]);
    }
}



PROGMEM device_values device_value [] = { 
    {cmdName_device_name,               0,  0, 0,   &value_get_device_name,             &value_set_device_name},
    {cmdName_device_ip,                 1,  0, 0,   &value_get_device_ip,               &value_set_device_ip},
    {cmdName_output_count,              2,  0, 0,   &value_get_output_count,            &value_set_output_count},
    {cmdName_output_pin,                3,  1, 0,   &value_get_output_pin,              &value_set_output_pin},
    {cmdName_output_type,               4,  1, 0,   &value_get_output_type,             &value_set_output_type},
    {cmdName_device_room,               5,  1, 0,   &value_get_device_room,             &value_set_device_room},
    {cmdName_output_grp,                6,  1, 0,   &value_get_output_grp,              &value_set_output_grp},
    {cmdName_output_location,           7,  1, 0,   &value_get_output_location,         &value_set_output_location},
    {cmdName_toggleOnOff,               8,  1, 1,   &value_get_toggleOnOff,             &value_set_toggleOnOff},
    {cmdName_output_state,              9,  1, 1,   &value_get_output_state,            &value_set_output_state},
    {cmdName_strip_size,                10, 1, 0,   &value_get_strip_size,              &value_set_strip_size},
    {cmdName_strip_color,               11, 1, 1,   &value_get_strip_color,             &value_set_strip_color},
    {cmdName_strip_colorOff,            12, 1, 1,   &value_get_strip_colorOff,          &value_set_strip_colorOff},
    {cmdName_strip_bri,                 13, 1, 1,   &value_get_strip_bri,               &value_set_strip_bri},
    {cmdName_output_selected,           14, 1, 1,   &value_get_output_selected,         &value_set_output_selected},
    {cmdName_patternStrip_selected,     15, 1, 1,   &value_get_patternStrip_selected,   &value_set_patternStrip_selected},
    {cmdName_device_lib,                16, 0, 0,   &value_get_device_lib,              &value_set_device_lib},
    {cmdName_strip_hue,                 17, 1, 1,   &value_get_strip_hue,               &value_set_strip_hue},
    {cmdName_strip_sat,                 18, 1, 1,   &value_get_strip_sat,               &value_set_strip_sat},
    {cmdName_strip_hBri,                19, 1, 1,   &value_get_strip_hBri,              &value_set_strip_hBri},
    {cmdName_strip_trueWhite,           20, 1, 1,   &value_get_trueWhite,               &value_set_trueWhite}
};
uint8_t device_valueCount = ARRAY_SIZE(device_value);

String c_to_string(char * name){
    char buffer[80];
    sprintf(buffer, "%s", name);
    return String(buffer);
}

// #define DEBUG
boolean device_debug = true;

// String value_set_command(boolean Debug, char    * id){
//     #ifdef DEBUG_TRACE
//         Serial.printf("> savToStringT\n");
//     #endif     
//     String line;
//     String ret;

//     #ifdef DEBUG
//         if (Debug)Serial.printf("\n[savToStringt] - [%d]\n", device_valueCount);
//     #endif

//     for (int i = 0; i < device_valueCount; i++) {
//         if (id == device_value[i].id ){}
     
//     }
//     #ifdef DEBUG
//         if (Debug) {
//             Serial.printf(ret.c_str());
//             Serial.printf("[savToStringt] Done\n");
//         }
//     #endif
//     return ret;
// }


String value_sav_toStringt(boolean Debug){
    #ifdef DEBUG_TRACE
        Serial.printf("> savToStringT\n");
    #endif     
    String line;
    String ret;

    #ifdef DEBUG
        if (Debug)Serial.printf("\n[savToStringt] - [%d]\n", device_valueCount);
    #endif

    for (int i = 0; i < device_valueCount; i++) {
        String pos              =     String(device_value[i].position);
        String value            =     device_value[i].getValue();
        line                    =     literal_item(pos, value);
        ret                     +=    line;
        #ifdef DEBUG
            if (Debug) {Serial.printf("[%d] - %s\n", i, value.c_str());}
        #endif      
    }
    #ifdef DEBUG
        if (Debug) {
            Serial.printf(ret.c_str());
            Serial.printf("[savToStringt] Done\n");
        }
    #endif
    return ret;
}




String value_load_fromString(boolean Debug, String str){
    #ifdef DEBUG_TRACE
        Serial.printf("> savToStringT\n");
    #endif 

    if (str == "") return "";

    String ret = "";

    #ifdef DEBUG
        if (Debug)Serial.printf("\n[value_load_fromSpiff] - [%d]\n", device_valueCount);
    #endif

    for (int i = 0; i < device_valueCount; i++) {
        String  pos     = String(device_value[i].position);
        String  value   = literal_value(pos, str);
        int     type    = device_value[i].type;
        #ifdef DEBUG
            if (Debug) {Serial.printf("[%d] - %s\n", i, value.c_str());}
        #endif   
            // device_value[i].setValue(value);

            if (device_value[i].load == 1) device_value[i].setValue(value);
    }
    #ifdef DEBUG
        if (Debug) {
            Serial.printf(ret.c_str());
            Serial.printf("[value_load_fromSpiff] Done\n");
        }
    #endif
    return ret;
}

boolean value_savToSPIFF(boolean Debug, String filename, String temp){
    #ifdef DEBUG_TRACE
        fsprintln("> value_savToSPIFF");
    #endif     
    #ifdef DEBUG
        if (Debug) fsprintf("\n[value_savToSPIFF] Opening file: %s", filename.c_str() );
    #endif
    File f = SPIFFS.open(filename,"w");
    if (!f) {  
        #ifdef DEBUG
            if (Debug) fsprintln(" : ERROR writing file."); 
        #endif
        return false;
    }
    #ifdef DEBUG
        if (Debug) fsprintln("  : Succes writing file.");
    #endif
    f.print(temp);
    f.close();
    return true;
}

String value_loadFromSPIFF(boolean Debug, String filename){
    #ifdef DEBUG_TRACE
        fsprintln("> pattern::loadFromSPIFF");
    #endif 
    #ifdef DEBUG
        if (Debug) fsprintf("\n[pattern::loadFromSPIFF] Opening file: %s", filename.c_str() );
    #endif 

    String line = "";
    File f = SPIFFS.open(filename,"r");
    if (!f) { 
        if (Debug) fsprintln(" : ERROR reading file."); 
        return line;
    }
    while (f.available()) {line += char(f.read());}  
    f.close();
    #ifdef DEBUG
        if (Debug) fsprintln(" : Succes reading file.");
    #endif 
    return  line;    
} 


String t_cd(int mod) {
    String os;
    String s;
    String buffer = ""; 

    s="";
    s+= literal_item("0", device_name);
    s+= literal_item("1", wifi_id.ip);
    s+= literal_item("2", String(output_count));

    os = get_output_info(buffer,    output_pin,             ARRAY_SIZE(output_pin));
    s+= literal_item("3", String(os));

    os = get_output_info(buffer,    output_type,            ARRAY_SIZE(output_type));
    s+= literal_item("4", String(os));

    os = get_output_info(buffer,    device_room,            ARRAY_SIZE(device_room));
    s+= literal_item("5", String(os));

    os = get_output_info(buffer,    output_grp,             ARRAY_SIZE(output_grp));
    s+= literal_item("6", String(os));

    os = get_output_info(buffer,    output_location,        ARRAY_SIZE(output_location));
    s+= literal_item("7", String(os));

    os = get_output_info(buffer,    toggleOnOff,            ARRAY_SIZE(toggleOnOff));
    s+= literal_item("8", String(os));

    os = get_output_info(buffer,    output_state,           ARRAY_SIZE(output_state));
    s+= literal_item("9", String(os));

    os = get_output_info(buffer,    strip_size,             ARRAY_SIZE(strip_size));
    s+= literal_item("10", String(os));

    os = get_output_info(buffer,    strip_color,            ARRAY_SIZE(strip_color));
    s+= literal_item("11", String(os));

    os = get_output_info(buffer,    strip_colorOff,         ARRAY_SIZE(strip_colorOff));
    s+= literal_item("12", String(os));

    os = get_output_info(buffer,    strip_bri,              ARRAY_SIZE(strip_bri));
    s+= literal_item("13", String(os));

    os = get_output_info(buffer,    output_selected,        ARRAY_SIZE(output_selected));
    s+= literal_item("14", String(os));

    os = get_output_info(buffer,    patternStrip_selected,  ARRAY_SIZE(patternStrip_selected));
    s+= literal_item("15", String(os));

    s+= literal_item("16", device_lib);

    os = get_output_info(buffer,    strip_hue,              ARRAY_SIZE(strip_hue));
    s+= literal_item("17", String(os));

    os = get_output_info(buffer,    strip_sat,              ARRAY_SIZE(strip_sat));
    s+= literal_item("18", String(os));

    os = get_output_info(buffer,    strip_hBri,             ARRAY_SIZE(strip_hBri));
    s+= literal_item("19", String(os));

    os = get_output_info(buffer,    trueWhite,              ARRAY_SIZE(trueWhite));
    s+= literal_item("20", String(os));
           
    return s;
}
String device_display(int mod){
    String s1="";
    for (int i = 0; i < device_valueCount; i++) {
        String pos              =     String(device_value[i].position);
        String value            =     device_value[i].getValue();
        String name             =     c_to_string(device_value[i].id);
        // line                    =     literal_item(pos, value);
        // ret                     +=    line;
        String s = "[" + pos + "]";
        while (s.length()<5) s+=" ";
        s1+= info_parm(s + name, value, ":", 25, "\n", "", "   ");  
    }
    return s1;
}

String send_device(){
    String  s   = "";
    String  str = t_cd(2);

    s+= literal_item("op",   "add");
    s+= literal_item("dn",   device_name);
    s+= literal_item("ip",   wifi_id.ip);
    s+= literal_item("add",  str);    
    return s;
}
String send_rep(){
    String  s   = "";
    String  str = t_cd(2);

    s+= literal_item("op",   "rep");
    s+= literal_item("dn",   device_name);
    s+= literal_item("ip",   wifi_id.ip);
    s+= literal_item("add",  str);    
    return s;
}


String get_output_info(String buffer, mod_outputs_type *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = output_type_to_string(pos[j]);
        if (buffer != "") {
            ret += info_parm(buffer  + " " + String(j), v, ":", 18, "\n", "", "   ");   
        } else {
            if ( j < count -1) ret += v + ";";
            else ret += v;
        }
    }
    return ret;
}
String get_output_info(String buffer, mod_devices_room *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = devices_room_to_string(pos[j]);
        if (buffer != "") {
            ret += info_parm(buffer  + " " + String(j), v, ":", 18, "\n", "", "   ");   
        } else {
            if ( j < count -1) ret += v + ';';
            else ret += v;
        }
    }
    return ret;
}
String get_output_info(String buffer, mod_devices_grp *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = devices_grp_to_string(pos[j]);
        if (buffer != "") {
            ret += info_parm(buffer  + " " + String(j), v, ":", 18, "\n", "", "   ");   
        } else {
            if ( j < count -1) ret += v + ';';
            else ret += v;
        }
    }
    return ret;
}
String get_output_info(String buffer, String *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = pos[j];
        if (buffer != "") {
            ret += info_parm(buffer  + " " + String(j), v, ":", 18, "\n", "", "   ");   
        } else {
            if ( j < count -1) ret += v + ';';
            else ret += v;
        }
    }
    return ret;
}
String get_output_info(String buffer, bool *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = String(pos[j]);
        if (buffer != "") {
            ret += info_parm(buffer  + " " + String(j), v, ":", 18, "\n", "", "   ");
        } else {
            if ( j < count -1) ret += v + ';';
            else ret += v;
        }
    }
    return ret;
}
String get_output_info(String buffer, int *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = String(pos[j]);
        if (buffer != "") {
            ret += info_parm(buffer  + " " + String(j), v, ":", 18, "\n", "", "   ");
        } else {
            if ( j < count -1) ret += v + ';';
            else ret += v;
        }
    }
    return ret;
}
String get_output_info(String buffer, uint8_t *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = String(pos[j]);
        if (buffer != "") {
            ret += info_parm(buffer  + " " + String(j), v, ":", 18, "\n", "", "   ");
        } else {
            if ( j < count -1) ret += v + ';';
            else ret += v;
        }
    }
    return ret;
}


