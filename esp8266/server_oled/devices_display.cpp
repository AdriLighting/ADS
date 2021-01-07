#include "devices_display.h"
#include "devices.h"
#include "tools.h"

extern devices devices;

String devices_display(int mod) {
    String s1 = "";
    for( int i = 0; i < devices_count ; i++) { 
      device_display(devices_array[i].device_name, mod, &s1) ;        
    }   
    return s1;
}
String device_display(String s, int mod, String * s1){
    
    // fs(sys_device_name,             "device_name");
    // fs(sys_device_ip,               "device_ip");
    // fs(sys_device_lib,              "device_lib");
    // fs(sys_isConnected,             "isConnected");
    // fs(sys_isConnected_lastTime,    "isConnected_lastTime");    
    // fs(sys_output_count,            "output_count");


    // fs(sys_output_pin,              "output_pin");
    // fs(sys_output_type,             "output_type");
    // fs(sys_device_room,             "device_room");
    // fs(sys_output_grp,              "output_grp");
    // fs(sys_output_location,         "output_location");
    // fs(sys_toggleOnOff,             "toggleOnOff");
    // fs(sys_output_state,            "output_state");
    // fs(sys_strip_size,              "strip_size");
    // fs(sys_strip_color,             "strip_color");
    // fs(sys_strip_colorOff,          "strip_colorOff");
    // fs(sys_strip_bri,               "strip_bri");
    // fs(sys_output_selected,         "output_selected");
    // fs(sys_patternStrip_selected,   "patternStrip_selected");
    // fs(sys_strip_hue,               "strip_hue");
    // fs(sys_strip_sat,               "strip_sat");
    // fs(sys_strip_hBri,              "strip_hBri");



    // int pos = devices.getListPos(s);
    // int output_count = devices_array[pos].output_count;

    // if ((mod == 0) || (mod == 1)) {
    //     * s1 += "\n";
    //     * s1 += info_parm(fsget(sys_device_name),           devices_array[pos].device_name,                      ":", 25, "\n", "", "   ");        
    //     * s1 += info_parm(fsget(sys_device_ip),             devices_array[pos].device_ip,                        ":", 25, "\n", "", "   ");        
    //     * s1 += info_parm(fsget(sys_device_lib),            devices_array[pos].device_lib,                       ":", 25, "\n", "", "   ");        
    //     * s1 += info_parm(fsget(sys_isConnected),           String(devices_array[pos].isConnected),              ":", 25, "\n", "", "   ");        
    //     * s1 += info_parm(fsget(sys_isConnected_lastTime),  String(devices_array[pos].isConnected_lastTime),     ":", 25, "\n", "", "   ");        
    //     * s1 += info_parm(fsget(sys_output_count),          String(devices_array[pos].output_count),             ":", 25, "\n", "", "   ");  
    // }

    // if (mod == 1) {
    //     * s1 += "\n";
    //     for( int j = 0; j < output_count; j++) {
    //         // * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_output_pin)          , String(devices_array[pos].output_pin[j]),                     ":", 25, "\n", "", "   "); 
    //         * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_output_type)         , output_type_to_string(devices_array[pos].output_type[j]),     ":", 25, "\n", "", "   "); 
    //         * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_device_room)         , devices_array[pos].device_room[j],                             ":", 25, "\n", "", "   "); 
    //         * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_output_grp)          , devices_array[pos].output_grp[j],                             ":", 25, "\n", "", "   "); 
    //         * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_output_location)     , devices_array[pos].output_location[j],                        ":", 25, "\n", "", "   "); 
    //         // * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_toggleOnOff)         , String(devices_array[pos].toggleOnOff[j]),                    ":", 25, "\n", "", "   "); 
    //         // * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_output_state)        , String(devices_array[pos].strip_size[j]),                     ":", 25, "\n", "", "   "); 
    //         // * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_strip_size)          , String(devices_array[pos].output_state[j]),                   ":", 25, "\n", "", "   "); 
    //         // * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_strip_color)         , devices_array[pos].strip_color[j],                            ":", 25, "\n", "", "   "); 
    //         // * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_strip_colorOff)      , devices_array[pos].strip_colorOff[j],                         ":", 25, "\n", "", "   "); 
    //         // * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_strip_bri)           , String(devices_array[pos].strip_bri[j]),                      ":", 25, "\n", "", "   "); 
    //         // * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_output_selected)     , String(devices_array[pos].output_selected[j]),                ":", 25, "\n", "", "   "); 
    //         // * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_patternStrip_selected), String(devices_array[pos].patternStrip_selected[j]),         ":", 25, "\n", "", "   "); 
    //         // * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_strip_hue)           , String(devices_array[pos].strip_hue[j]),                      ":", 25, "\n", "", "   "); 
    //         // * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_strip_sat)           , String(devices_array[pos].strip_sat[j]),                      ":", 25, "\n", "", "   "); 
    //         // * s1 += "\t" + info_parm( String(j) + " " + fsget(sys_strip_hBri)          , String(devices_array[pos].strip_hBri[j]),                     ":", 25, "\n", "", "   "); 
    //         * s1 += "\t---\n";
    //     } 
    //     * s1 += "[...]\n";       

    // } 
    // return * s1;
}


