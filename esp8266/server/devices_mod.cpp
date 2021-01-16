#include "devices_mod.h"

String devices_lib_to_string(mod_devices_lib name) {
    String ret = ""; 
    switch (name) {
        case DL_MULTIDEVICE:    ret = "MULTIDEVICE";    break;
        case DL_MULTILAMP:      ret = "MULTILAMP";      break;
    }
    return ret;
}
mod_devices_lib mod_device_lib;


String devices_room_to_string(mod_devices_room name) {
    String ret = ""; 
    switch (name) {
        case DR_CH:             ret = "chambre";    break;
        case DR_SALON:          ret = "salon";      break;
        case DR_KITCHEN:        ret = "cuisine";      break;
        case DR_FREE:           ret = "libre";      break;
        case DR_UK:             ret = "inconnu";    break;
    }
    return ret;
}
mod_devices_room  devices_room_string_to_mod(String name) {
    mod_devices_room ret; 
    if (name == "chambre")      ret = DR_CH;
    if (name == "salon")        ret = DR_SALON;
    if (name == "cuisine")      ret = DR_KITCHEN;
    if (name == "libre")        ret = DR_FREE;
    if (name == "inconnu")      ret = DR_UK;
    return ret;
}
mod_devices_room mod_device_room;
mod_devices_room mod_device_room_array[] { 
    DR_CH,
    DR_SALON,
    DR_KITCHEN,
    DR_FREE,
    DR_UK
};

String output_type_to_string(mod_outputs_type name) {
    String ret = ""; 
    switch (name) {
        case OT_RGBSTRIP:       ret = "strip_rgb";      break;
        case OT_RGBWSTRIP:      ret = "strip_rgbw";     break;         
        case OT_RELAY_LP:       ret = "relay_lampe";    break;
        case OT_RELAY_AC:       ret = "relay_ac";       break;  
        default:
            ret = "strip_rgb"; 
            break;               
    }
    return ret;
}
mod_outputs_type output_type_string_to_mod(String name) {
   mod_outputs_type ret; 
   if (name == "strip_rgb")     ret = OT_RGBSTRIP;
   if (name == "strip_rgbw")    ret = OT_RGBWSTRIP;
   if (name == "relay_lampe")   ret = OT_RELAY_LP;
   if (name == "relay_ac")      ret = OT_RELAY_AC;
   return ret;
}
mod_outputs_type mod_output_type;

String devices_grp_to_string(mod_devices_grp name) {
    String ret = ""; 
    switch (name) {
        case DG_ECARN:          ret = "ecran";          break;
        case DG_BUREAU:         ret = "bureau";         break;
        case DG_CENTRE:         ret = "center";         break;
        // case DG_CENTRE_RELAY:   ret = "centre_relay";   break;
        case DG_LIT:            ret = "lit";            break;
        case DG_UNKNOW:         ret = "unknow";         break; 
        // case DG_FREE:           ret = "libre";          break;      
        default:
            ret = "unknow"; 
            break;                                
    }
    return ret;
}
mod_devices_grp devices_grp_string_to_mod(String name) {
    mod_devices_grp ret; 
    if (name == "ecran")        ret = DG_ECARN;
    if (name == "bureau")       ret = DG_BUREAU;
    if (name == "center")       ret = DG_CENTRE;
    if (name == "centre_relay") ret = DG_CENTRE_RELAY;
    if (name == "lit")          ret = DG_LIT;
    if (name == "unknow")       ret = DG_UNKNOW;
    if (name == "libre")        ret = DG_FREE;
    return ret;
}
mod_devices_grp mod_device_grp;

mod_devices_grp mod_devices_grp_array[] { 
    DG_BUREAU,
    DG_ECARN,
    DG_CENTRE,
    DG_CENTRE_RELAY,
    DG_LIT,
    DG_FREE,
    DG_UNKNOW
};