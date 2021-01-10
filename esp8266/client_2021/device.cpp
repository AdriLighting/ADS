#include "device.h"
#include <adri_tools.h>


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
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = _device_mod->oType_str(output_type[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}
void device::set_oPin(int value[]) 					{for (int i = 0; i < output_count; ++i) output_pin[i] 		=  value[i];}
void device::get_oPin(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = String(output_pin[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}
void device::set_oRoom(mod_devices_room value[])	{for (int i = 0; i < output_count; ++i) device_room[i] 		=  value[i];}
void device::get_oRoom(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = _device_mod->oRoom_str(device_room[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}
void device::set_oGroup(mod_devices_grp value[])	{for (int i = 0; i < output_count; ++i) output_grp[i] 		=  value[i];}
void device::get_oGroup(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = _device_mod->oGrp_str(output_grp[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}
void device::set_oLocation(String value[])			{for (int i = 0; i < output_count; ++i) output_location[i] 	=  value[i];}
void device::get_oLocation(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = output_location[i];
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}

void device::set_oSelect(int value[])				{for (int i = 0; i < output_count; ++i) output_selected[i] 	=  value[i];}
void device::get_oSelect(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = String(output_selected[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}
void device::set_oState(int value[])				{for (int i = 0; i < output_count; ++i) output_state[i] 	=  value[i];}
void device::get_oState(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = String(output_state[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}

void device::set_sBri(uint8_t value[])				{for (int i = 0; i < output_count; ++i) strip_bri[i] 		=  value[i];}
void device::get_sBri(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = String(strip_bri[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}
void device::set_sSize(int value[])					{for (int i = 0; i < output_count; ++i) strip_size[i] 		=  value[i];}
void device::get_sSize(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = String(strip_size[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}

void device::set_toggle(boolean value[])			{for (int i = 0; i < output_count; ++i) toggleOnOff[i] 		=  value[i];}
void device::get_toggle(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = String(toggleOnOff[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}
void device::set_trueWhite(boolean value[])			{for (int i = 0; i < output_count; ++i) trueWhite[i] 		=  value[i];}
void device::get_trueWhite(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = String(trueWhite[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}

void device::set_sHue(uint8_t value[])				{for (int i = 0; i < output_count; ++i) strip_hue[i] 		=  value[i];}
void device::get_sHue(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = String(strip_hue[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}
void device::set_sSat(uint8_t value[])				{for (int i = 0; i < output_count; ++i) strip_sat[i] 		=  value[i];}
void device::get_sSat(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = String(strip_sat[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}
void device::set_sHbri(uint8_t value[])				{for (int i = 0; i < output_count; ++i) strip_hBri[i] 		=  value[i];}
void device::get_sHbri(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = String(strip_hBri[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}

void device::set_sColor(String value[])				{for (int i = 0; i < output_count; ++i) strip_color[i] 		=  value[i];}
void device::get_sColor(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = strip_color[i];
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}
void device::set_sColorOff(String value[])			{for (int i = 0; i < output_count; ++i) strip_colorOff[i]	=  value[i];}
void device::get_sColorOff(String & ret)	{
	ret = "";
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = strip_colorOff[i];
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}

void device::set_pSelect(int value[])				{for (int i = 0; i < output_count; ++i) patternStrip_selected[i]	=  value[i];}
void device::get_pSelect(String & ret)	{
	ret = "";
	for (int i = 0; i < output_count; ++i) {
		String v = String(patternStrip_selected[i]);
		if ( i < output_count -1) 	ret += v + ";";
	    else 						ret += v;			
	}
}

void device::print_output(String title, String str){
	fsprintf("[%s]\n", title.c_str());
	String list[output_count];
	int cnt = explode(str, ';', list);
	for (int i = 0; i < cnt; ++i)
	{
		fsprintf("\t[%3d][%-15s]\n", i, list[i].c_str());
	}
}

void set_def(){
	device * ptr = new device("DEVICE", 2, "MULTIDEVICE");


    mod_outputs_type    output_type 		[] = { OT_RGBSTRIP     , OT_RGBWSTRIP};
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

    uint8_t             strip_hue 			[] = {0                , 0};
    uint8_t             strip_sat 			[] = {255              , 255};
    uint8_t             strip_hBri 			[] = {255              , 255};

    String              strip_color 		[] = {"0.0.255"        , "0.0.255"};
    String              strip_colorOff 		[] = {"0.0.0"			, "0.0.0"};


    int                 patternStrip_selected[] = {0, 0};

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
}

    const char PROGMEM str_output_pos				[] = "pos";
    const char PROGMEM str_output_type 				[] = "output_type";
    const char PROGMEM str_output_pin 				[] = "output_pin";
    const char PROGMEM str_device_room 				[] = "device_room";
    const char PROGMEM str_output_grp 				[] = "output_grp";
    const char PROGMEM str_output_location 			[] = "output_location";

    const char PROGMEM str_output_selected 			[] = "output_selected";
    const char PROGMEM str_output_state 			[] = "output_state";

    const char PROGMEM str_strip_bri 				[] = "strip_bri";
    const char PROGMEM str_strip_size 				[] = "strip_size";

    const char PROGMEM str_toggleOnOff 				[] = "toggleOnOff";
    const char PROGMEM str_trueWhite 				[] = "trueWhite";

    const char PROGMEM str_strip_hue 				[] = "strip_hue";
    const char PROGMEM str_strip_sat 				[] = "strip_sat";
    const char PROGMEM str_strip_hBri 				[] = "strip_hBri";

    const char PROGMEM str_strip_color 				[] = "strip_color";
    const char PROGMEM str_strip_colorOff 			[] = "strip_colorOff";
    
    const char PROGMEM str_patternStrip_selected 	[] = "patternStrip_selected";
void print_output(){
	device * ptr = devicePtr;

    int oc = ptr->output_count+1;

    String output_type 		[oc];
    String output_pin		[oc];
    String device_room 		[oc];
    String output_grp		[oc];
    String output_location 	[oc];

    String output_selected 	[oc];
    String output_state		[oc]; 

    String strip_bri		[oc];
    String strip_size		[oc]; 

    String toggleOnOff 		[oc]; 
    String trueWhite		[oc]; 

    String strip_hue		[oc];
    String strip_sat		[oc];
    String strip_hBri		[oc];

    String strip_color 		[oc];
    String strip_colorOff	[oc];


    String patternStrip_selected[oc];

    String ret;

    ptr->get_oType(ret);
    explode(ret, ';', output_type);
    ptr->get_oPin(ret);
    explode(ret, ';', output_pin);
    ptr->get_oRoom(ret);
    explode(ret, ';', device_room);
    ptr->get_oGroup(ret);
    explode(ret, ';', output_grp);
    ptr->get_oLocation(ret);
    explode(ret, ';', output_location);

    ptr->get_oSelect(ret);
    explode(ret, ';', output_selected);
    ptr->get_oState(ret);
    explode(ret, ';', output_state);

    ptr->get_sBri(ret);
    explode(ret, ';', strip_bri);
    ptr->get_sSize(ret);
    explode(ret, ';', strip_size);

    ptr->get_toggle(ret);
    explode(ret, ';', toggleOnOff);
    ptr->get_trueWhite(ret);
    explode(ret, ';', trueWhite);    

    ptr->get_sHue(ret);
    explode(ret, ';', strip_hue);
    ptr->get_sSat(ret);
    explode(ret, ';', strip_sat);    
    ptr->get_sHbri(ret);
    explode(ret, ';', strip_hBri);     

    ptr->get_sColor(ret);
    explode(ret, ';', strip_color);
    ptr->get_sColorOff(ret);
    explode(ret, ';', strip_colorOff);  

    ptr->get_pSelect(ret);
    explode(ret, ';', patternStrip_selected);   

    fsprintf("\n[%-10s][%-20s][%-20s][%-20s][%-20s][%-20s][%-20s]\n",
    	str_output_pos,
    	str_output_type,
    	str_device_room,
    	str_output_grp,
    	str_output_location,
    	str_output_pin,
    	str_strip_size

    ) ; 
    oc = oc-1;
    for (int i = 0; i < oc; ++i)
    {
	    fsprintf("[%-10d][%-20s][%-20s][%-20s][%-20s][%-20s][%-20s]\n",
	    	i,
	    	output_type[i].c_str(),
	    	device_room[i].c_str(),
	    	output_grp[i].c_str(),
	    	output_location[i].c_str(),
	    	output_pin[i].c_str(),
	    	strip_size[i].c_str()
	    ) ; 
    }   

    fsprintf("\n[%-10s][%-20s][%-20s][%-20s][%-20s]\n",
    	str_output_pos,
    	str_output_selected,
    	str_output_state,
    	str_toggleOnOff,
    	str_trueWhite
    ) ; 
    for (int i = 0; i < oc; ++i)
    {
	    fsprintf("[%-10d][%-20s][%-20s][%-20s][%-20s]\n",
	    	i,
	    	output_selected[i].c_str(),
	    	output_state[i].c_str(),
	    	toggleOnOff[i].c_str(),
	    	trueWhite[i].c_str()	    	
	    ) ; 
    }   

    fsprintf("\n[%-10s][%-20s][%-20s][%-20s][%-20s]\n",
    	str_output_pos,
    	str_strip_hue,
    	str_strip_sat,
    	str_strip_hBri,
    	str_strip_bri
    ) ; 
    for (int i = 0; i < oc; ++i)
    {
	    fsprintf("[%-10d][%-20s][%-20s][%-20s][%-20s]\n",
	    	i,
	    	strip_hue[i].c_str(),
	    	strip_sat[i].c_str(),
	    	strip_hBri[i].c_str(),
	    	strip_bri[i].c_str()
	    ) ; 
    }  

    fsprintf("\n[%-10s][%-20s][%-20s][%-20s]\n",
    	str_output_pos,
    	str_strip_color,
    	str_strip_colorOff,
    	str_patternStrip_selected
    ) ; 
    for (int i = 0; i < oc; ++i)
    {
	    fsprintf("[%-10d][%-20s][%-20s][%-20s]\n",
	    	i,
	    	strip_color[i].c_str(),
	    	strip_colorOff[i].c_str(),
	    	patternStrip_selected[i].c_str()
	    ) ; 
    }    
}


/*



    Serial.flush();
    delay(500);    





*/