#ifndef device_h
#define device_h 

#include <Arduino.h>
#include <ESP8266WiFi.h>

#define DEVICE_OUTPUTS_MAX 6
    enum mod_devices_lib { DL_MULTIDEVICE, DL_MULTILAMP};

    enum mod_devices_room { 
       DR_CH,
       DR_FREE,
       DR_KITCHEN,
       DR_UK,
       DR_SALON
    };
    enum mod_devices_grp { 
        DG_BUREAU,
        DG_ECARN,
        DG_CENTRE,
        DG_CENTRE_RELAY,
        DG_LIT,
        DG_FREE,
        DG_UNKNOW
    };  
    enum mod_outputs_type { 
        OT_RGBSTRIP,
        OT_RGBWSTRIP,
        OT_RELAY_AC,
        OT_SONDE,
        OT_RELAY_LP
    };      

class device_mod
{
public:
	device_mod();
	~device_mod();
	String 				lib_str 		(mod_devices_lib name);

	String 				room_str 		(mod_devices_room name);
	mod_devices_room 	room_mod 		(String name);

	String 				output_type_str (mod_outputs_type name);
	mod_outputs_type 	output_type_mod (String name);

	String 				grp_string 		(mod_devices_grp name);
	mod_devices_grp		grp_mod 		(String name);
};
device_mod * device_modPtrGet();

class device
{
    String      		device_name 			= "DEVICENAME";
    IPAddress   		device_ip				= {192, 168, 0, 1};

    int                 output_count    		= 0;//
    mod_outputs_type    output_type 			[DEVICE_OUTPUTS_MAX];
    int                 output_pin 				[DEVICE_OUTPUTS_MAX];
    mod_devices_room    device_room 			[DEVICE_OUTPUTS_MAX];
    mod_devices_grp     output_grp 				[DEVICE_OUTPUTS_MAX];
    String              output_location 		[DEVICE_OUTPUTS_MAX];
    uint8_t             strip_bri 				[DEVICE_OUTPUTS_MAX];
    int                 strip_size 				[DEVICE_OUTPUTS_MAX]; 

    boolean             toggleOnOff 			[DEVICE_OUTPUTS_MAX]; 
    int                 output_state 			[DEVICE_OUTPUTS_MAX]; 
    boolean             trueWhite 				[DEVICE_OUTPUTS_MAX]; 

    uint8_t 			strip_hue 				[DEVICE_OUTPUTS_MAX];
    uint8_t 			strip_sat 				[DEVICE_OUTPUTS_MAX];
    uint8_t 			strip_hBri 				[DEVICE_OUTPUTS_MAX];

    String  			strip_color 			[DEVICE_OUTPUTS_MAX];
    String  			strip_colorOff 			[DEVICE_OUTPUTS_MAX];

    int     			output_selected 		[DEVICE_OUTPUTS_MAX];
    int     			patternStrip_selected	[DEVICE_OUTPUTS_MAX];

    String 				device_lib				= "DEVICE_LIB";

public:
	device(String name, int oc);
	~device();
	
};
device * devicePtrGet();
#endif