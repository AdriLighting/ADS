#ifndef DEVICESMODULE_H
#define DEVICESMODULE_H 

    #include <arduino.h>

	#include "def.h"
	#include "devices_mod.h"
	#include "devicesoutput.h"

    class devices {
    public:

        boolean         isAdded = false;
        boolean         isConnected;
        unsigned long   isConnected_lastTime;      
        int             isConnectedCount = 0;

        String device_lib;
        String device_name;
        String device_ip;

        int                 output_count = 0;//
        int                 output_pin[MAX_OUTPUTS];
        mod_outputs_type    output_type[MAX_OUTPUTS];

        String              device_room[MAX_OUTPUTS];
        String              output_grp[MAX_OUTPUTS];
        String              output_location[MAX_OUTPUTS];

        bool    toggleOnOff[MAX_OUTPUTS]; 
        bool    trueWhite[MAX_OUTPUTS]; 
        // int     output_state[MAX_OUTPUTS]; 

        int     output_selected[MAX_OUTPUTS];
        // int     patternStrip_selected[MAX_OUTPUTS];

        // int     strip_size[MAX_OUTPUTS];
        String  strip_color[MAX_OUTPUTS];
        // String  strip_colorOff[MAX_OUTPUTS];
        uint8_t strip_bri[MAX_OUTPUTS];

        uint8_t strip_hue[MAX_OUTPUTS];
        uint8_t strip_sat[MAX_OUTPUTS];
        uint8_t strip_hBri[MAX_OUTPUTS];


    	devices();
        void    addDevice(String dn, String str);
        int     getListPos(String dn);
        boolean deviceIsListed(String dn);
        String  getLib(String dn);

        void    deviceIsConnected(String dn, bool state);
        void    upd_udpDevices();
        void    print();
        int     geOutputPosByName(int dPos, String search);

        

    };

    extern devices devices_array[];
    extern int devices_count;

    devices * devicesPtrGet();
    // void device_add(String s, int pos);

#endif