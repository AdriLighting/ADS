#ifndef DEVICE_H
#define DEVICE_H 

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

	String 				oRoom_str 		(mod_devices_room name);
	mod_devices_room 	oRoom_mod 		(String name);

	String 				oType_str 		(mod_outputs_type name);
	mod_outputs_type 	oType_mod 		(String name);

	String 				oGrp_str		(mod_devices_grp name);
	mod_devices_grp		oGrp_mod 		(String name);
};
device_mod * device_modPtrGet();


class device
{

	device_mod 			* _device_mod;

    String      		device_name 			= "DEVICENAME";
    IPAddress   		device_ip				= {192, 168, 0, 1};
    String 				device_lib				= "DEVICE_LIB";

    mod_outputs_type    output_type 			[DEVICE_OUTPUTS_MAX];
    int                 output_pin 				[DEVICE_OUTPUTS_MAX];
    mod_devices_room    device_room 			[DEVICE_OUTPUTS_MAX];
    mod_devices_grp     output_grp 				[DEVICE_OUTPUTS_MAX];
    String              output_location 		[DEVICE_OUTPUTS_MAX];

    int     			output_selected 		[DEVICE_OUTPUTS_MAX];
    int                 output_state 			[DEVICE_OUTPUTS_MAX]; 

    uint8_t             strip_bri 				[DEVICE_OUTPUTS_MAX];
    int                 strip_size 				[DEVICE_OUTPUTS_MAX]; 

    boolean             toggleOnOff 			[DEVICE_OUTPUTS_MAX]; 
    boolean             trueWhite 				[DEVICE_OUTPUTS_MAX]; 

    uint8_t 			strip_hue 				[DEVICE_OUTPUTS_MAX];
    uint8_t 			strip_sat 				[DEVICE_OUTPUTS_MAX];
    uint8_t 			strip_hBri 				[DEVICE_OUTPUTS_MAX];

    String  			strip_color 			[DEVICE_OUTPUTS_MAX];
    String  			strip_colorOff 			[DEVICE_OUTPUTS_MAX];

    int     			patternStrip_selected	[DEVICE_OUTPUTS_MAX];


public:
    int                 output_count    		= 0;//


	device(String name, int oc, String lib);
	~device();

	void set_oType(mod_outputs_type value[]);
	void get_oType(String & ret);

	void set_oPin(int value[]);
	void get_oPin(String & ret);

	void set_oRoom(mod_devices_room value[]);
	void get_oRoom(String & ret);

	void set_oGroup(mod_devices_grp value[]);
	void get_oGroup(String & ret);

	void set_oLocation(String value[]);
	void get_oLocation(String & ret);


	void set_oSelect(int value[]);
	void get_oSelect(String & ret);

	void set_oState(int value[]);
	void get_oState(String & ret);


	void set_sBri(uint8_t value[]);
	void get_sBri(String & ret);

	void set_sSize(int value[]);
	void get_sSize(String & ret);


	void set_toggle(boolean value[]);
	void get_toggle(String & ret);

	void set_trueWhite(boolean value[]);
	void get_trueWhite(String & ret);


	void set_sHue(uint8_t value[]);
	void get_sHue(String & ret);

	void set_sSat(uint8_t value[]);
	void get_sSat(String & ret);

	void set_sHbri(uint8_t value[]);
	void get_sHbri(String & ret);


	void set_sColor(String value[]);
	void get_sColor(String & ret);

	void set_sColorOff(String value[]);
	void get_sColorOff(String & ret);


	void set_pSelect(int value[]);
	void get_pSelect(String & ret);

	void print_output(String title, String str);

};
device * devicePtrGet();
void set_def();
void print_output();
#endif


