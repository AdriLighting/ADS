
#include <arduino.h>

#include "cs_getvalue.h"

#include "devices.h"

#include "tools.h"

#include "adriwifi.h"

#include <dac_command.h>

extern wifi_id wifi_id;


String getV_patternStrip_selected(){
    String os = get_output_info("",    patternStrip_selected,   output_count);
    return os;
}
String getV_output_selected(){
    String os = get_output_info("",    output_selected,         output_count);
    return os;
}
String getV_strip_bri(){
    String os = get_output_info("",    strip_bri,               output_count);
    return os;
}
String getV_strip_colorOff(){
    String os = get_output_info("",    strip_colorOff,          output_count);
    return os;
}
String getV_strip_color(){
    String os = get_output_info("",    strip_color,             output_count);
    return os;
}
String getV_strip_size(){
    String os = get_output_info("",    strip_size,              output_count);
    return os;
}
String getV_output_state(){
    String os = get_output_info("",    output_state,            output_count);
    return os;
}
String getV_toggleOnOff(){
    String os = get_output_info("",    toggleOnOff,             output_count);
    return os;
}
String getV_output_location(){
    String os = get_output_info("",    output_location,         output_count);
    return os;
}
String getV_device_room(){
    String os = get_output_info("",    device_room,             output_count);
    return os;
}
String getV_output_type(){
    String os = get_output_info("",    output_type,             output_count);
    return os;
}
String getV_output_pin(){
    String os = get_output_info("",    output_pin,              output_count);
    return os;
}
String getV_output_count(){
    return String(output_count);
}
String getV_device_ip(){
    return  wifi_id.ip;
}
String getV_device_name(){
    return device_name;
}

PROGMEM cmdApp_cs_lists cmdApp_cs_list [] = { 
    {cmdName_device_name,               &getV_device_name},
    {cmdName_device_ip,                 &getV_device_ip},
    {cmdName_output_count,              &getV_output_count},
    {cmdName_output_pin,                &getV_output_pin},
    {cmdName_output_type,               &getV_output_type},
    {cmdName_device_room,               &getV_device_room},
    {cmdName_output_location,           &getV_output_location},
    {cmdName_toggleOnOff,               &getV_toggleOnOff},
    {cmdName_output_state,              &getV_output_state},
    {cmdName_strip_size,                &getV_strip_size},
    {cmdName_strip_color,               &getV_strip_color},
    {cmdName_strip_colorOff,            &getV_strip_colorOff},
    {cmdName_strip_bri,                 &getV_strip_bri},
    {cmdName_output_selected,           &getV_output_selected},
    {cmdName_patternStrip_selected,     &getV_patternStrip_selected}
    // {cmdName_, },
};
