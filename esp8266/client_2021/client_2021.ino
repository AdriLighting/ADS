#include "ADS_lib.h"

#include <adri_wifiConnect.h>

void setup()
{


	Serial.begin(115200);

            WiFi.disconnect(true);
            WiFi.softAPdisconnect(true);
            WiFi.setPhyMode(WIFI_PHY_MODE_11B);

	delay(3000);


	Serial.println("");
	Serial.println("");

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
	
	print_output();
	// delay(3000);	
}

void loop()
{

}
