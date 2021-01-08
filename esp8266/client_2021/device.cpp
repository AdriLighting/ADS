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
String device_mod::oRoom_str(mod_devices_room name) {
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
mod_devices_room  device_mod::oRoom_mod(String name) {
    mod_devices_room ret; 
    if (name == "chambre")      ret = DR_CH;
    if (name == "salon")        ret = DR_SALON;
    if (name == "cuisine")      ret = DR_KITCHEN;
    if (name == "libre")        ret = DR_FREE;
    if (name == "inconnu")      ret = DR_UK;
    return ret;
}
String device_mod::oType_str(mod_outputs_type name) {
    String ret = ""; 
    switch (name) {
        case OT_RGBSTRIP:       ret = "strip_rgb";      break;
        case OT_RGBWSTRIP:      ret = "strip_rgbw";     break;         
        case OT_RELAY_LP:       ret = "relay_lampe";    break;
        case OT_RELAY_AC:       ret = "relay_ac";       break;         
    }
    return ret;
}
mod_outputs_type device_mod::oType_mod(String name) {
   mod_outputs_type ret; 
   if (name == "strip_rgb")     ret = OT_RGBSTRIP;
   if (name == "strip_rgbw")    ret = OT_RGBWSTRIP;
   if (name == "relay_lampe")   ret = OT_RELAY_LP;
   if (name == "relay_ac")      ret = OT_RELAY_AC;
   return ret;
}

String device_mod::oGrp_str(mod_devices_grp name) {
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
mod_devices_grp device_mod::oGrp_mod(String name) {
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
device::device(String name, int oc, String lib){
    _device_mod     = new device_mod();
	devicePtr 		= this; 
	device_name 	= name;
	device_lib 		= lib;
	output_count	= oc;

}
void device::set_oType(mod_outputs_type value[]) 	{for (int i = 0; i < output_count; ++i) output_type[i] 		=  value[i];}
void device::get_oType(String & ret)	{
	for (int i = 0; i < output_count; ++i) {
		String v = _device_mod->oType_str(output_type[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}
void device::set_oPin(int value[]) 					{for (int i = 0; i < output_count; ++i) output_pin[i] 		=  value[i];}
void device::get_oPin(String & ret)	{
	for (int i = 0; i < output_count; ++i) {
		String v = String(output_pin[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}
void device::set_oRoom(mod_devices_room value[])	{for (int i = 0; i < output_count; ++i) device_room[i] 		=  value[i];}
void device::get_oRoom(String & ret)	{
	for (int i = 0; i < output_count; ++i) {
		String v = _device_mod->oRoom_str(device_room[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}
void device::set_oGroup(mod_devices_grp value[])	{for (int i = 0; i < output_count; ++i) output_grp[i] 		=  value[i];}
void device::set_oLocation(String value[])			{for (int i = 0; i < output_count; ++i) output_location[i] 	=  value[i];}

void device::set_oSelect(int value[])				{for (int i = 0; i < output_count; ++i) output_selected[i] 	=  value[i];}
void device::set_oState(int value[])				{for (int i = 0; i < output_count; ++i) output_state[i] 	=  value[i];}

void device::set_sBri(uint8_t value[])				{for (int i = 0; i < output_count; ++i) strip_bri[i] 		=  value[i];}
void device::set_sSize(int value[])					{for (int i = 0; i < output_count; ++i) strip_size[i] 		=  value[i];}

void device::set_toggle(boolean value[])			{for (int i = 0; i < output_count; ++i) toggleOnOff[i] 		=  value[i];}
void device::set_trueWhite(boolean value[])			{for (int i = 0; i < output_count; ++i) trueWhite[i] 		=  value[i];}

void device::set_sHue(uint8_t value[])				{for (int i = 0; i < output_count; ++i) strip_hue[i] 		=  value[i];}
void device::set_sSat(uint8_t value[])				{for (int i = 0; i < output_count; ++i) strip_sat[i] 		=  value[i];}
void device::set_sHbri(uint8_t value[])				{for (int i = 0; i < output_count; ++i) strip_hBri[i] 		=  value[i];}

void device::set_sColor(String value[])				{for (int i = 0; i < output_count; ++i) strip_color[i] 		=  value[i];}
void device::set_sColorOff(String value[])			{for (int i = 0; i < output_count; ++i) strip_colorOff[i]	=  value[i];}

void device::set_pSelect(int value[])				{for (int i = 0; i < output_count; ++i) patternStrip_selected[i]	=  value[i];}

void set_def(){
	device * ptr = new device("DEVICE", 2, "MULTIDEVICE");


    mod_outputs_type    output_type 		[] = { OT_RGBSTRIP     , OT_RGBSTRIP};
    int                 output_pin 			[] = { D1              , D2};
    mod_devices_room    device_room 		[] = { DR_CH           , DR_CH};
    mod_devices_grp     output_grp 			[] = { DG_CENTRE       , DG_CENTRE};
    String              output_location 	[] = { "enter_right"   , "enter_table"};

    int                 output_selected 	[] = {1                , 1};
    int                 output_state 		[] = {0                , 0}; 

    uint8_t             strip_bri 			[] = { 60              , 60};
    int                 strip_size 			[] = { 60              , 60}; 

    boolean             toggleOnOff 		[] = {true             , true}; 
    boolean             trueWhite 			[] = {false            , false}; 

    uint8_t             strip_hue 			[] = {0                , 0}; ;
    uint8_t             strip_sat 			[] = {255              , 255};
    uint8_t             strip_hBri 			[] = {255              , 255};

    String              strip_color 		[] = {"0.0.255"        , "0.0.255"};
    String              strip_colorOff 		[] = {"0.0.0"			, "0.0.0"};


    int                 patternStrip_selected[] = {0, 0};


    int oc = ptr->output_count;
    Serial.println(oc);

    ptr->set_oType 		(output_type);
	ptr->set_oPin		(output_pin);
	ptr->set_oRoom		(device_room);
	ptr->set_oGroup		(output_grp);
	ptr->set_oLocation	(output_location);

	ptr->set_oSelect	(output_selected);
	ptr->set_oState		(output_state);

	ptr->set_sBri		(strip_bri);
	ptr->set_sSize		(strip_size);

	ptr->set_toggle		(toggleOnOff);
	ptr->set_trueWhite	(trueWhite);

	ptr->set_sHue		(strip_hue);
	ptr->set_sSat		(strip_sat);
	ptr->set_sHbri		(strip_hBri);

	ptr->set_sColor		(strip_color);
	ptr->set_sColorOff	(strip_colorOff);

	ptr->set_pSelect	(patternStrip_selected);

    String ret;
    ptr->get_oType(ret);
    Serial.println(ret);
}


/*








*/