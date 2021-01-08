#include "device.h"
device_mod * device_modPtr = nullptr;
device_mod * device_modPtrGet(){return device_modPtr;}
device_mod::device_mod(){device_modPtr = this;}
String device_mod::lib_str(mod_devices_lib name) {
    String ret = ""; 
    switch (name) {
        case DL_MULTIDEVICE:    ret = "MULTIDEVICE";    break;
        case DL_MULTILAMP:      ret = "MULTILAMP";      break;
    }
    return ret;
}
String device_mod::room_str(mod_devices_room name) {
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
mod_devices_room  device_mod::room_mod(String name) {
    mod_devices_room ret; 
    if (name == "chambre")      ret = DR_CH;
    if (name == "salon")        ret = DR_SALON;
    if (name == "cuisine")      ret = DR_KITCHEN;
    if (name == "libre")        ret = DR_FREE;
    if (name == "inconnu")      ret = DR_UK;
    return ret;
}
String device_mod::output_type_str(mod_outputs_type name) {
    String ret = ""; 
    switch (name) {
        case OT_RGBSTRIP:       ret = "strip_rgb";      break;
        case OT_RGBWSTRIP:      ret = "strip_rgbw";     break;         
        case OT_RELAY_LP:       ret = "relay_lampe";    break;
        case OT_RELAY_AC:       ret = "relay_ac";       break;         
    }
    return ret;
}
mod_outputs_type device_mod::output_type_mod(String name) {
   mod_outputs_type ret; 
   if (name == "strip_rgb")     ret = OT_RGBSTRIP;
   if (name == "strip_rgbw")    ret = OT_RGBWSTRIP;
   if (name == "relay_lampe")   ret = OT_RELAY_LP;
   if (name == "relay_ac")      ret = OT_RELAY_AC;
   return ret;
}

String device_mod::grp_string(mod_devices_grp name) {
    String ret = ""; 
    switch (name) {
        case DG_ECARN:          ret = "ecran";          break;
        case DG_BUREAU:         ret = "bureau";         break;
        case DG_CENTRE:         ret = "center";         break;
        case DG_CENTRE_RELAY:   ret = "centre_relay";   break;
        case DG_LIT:            ret = "lit";            break;
        case DG_UNKNOW:         ret = "unknow";         break; 
        case DG_FREE:           ret = "libre";          break;                              
    }
    return ret;
}
mod_devices_grp device_mod::grp_mod(String name) {
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
mod_devices_grp mod_devices_grp_array[] { 
    DG_BUREAU,
    DG_ECARN,
    DG_CENTRE,
    DG_CENTRE_RELAY,
    DG_LIT,
    DG_FREE,
    DG_UNKNOW
};

device * devicePtr = nullptr;
device * devicePtrGet(){return devicePtr;}
device::device(){devicePtr = this; new device_mod();}
/*








*/