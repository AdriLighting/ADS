

#ifndef DEVICES_MOD_H
	#define DEVICES_MOD_H
    #include <arduino.h>

    enum mod_devices_room { 
       DR_CH,
       DR_FREE,
       DR_KITCHEN,
       DR_UK,
       DR_SALON
    };

    enum mod_devices_lib { DL_MULTIDEVICE, DL_MULTILAMP};
    enum mod_outputs_type { 
        OT_RGBSTRIP,
        OT_RGBWSTRIP,
        OT_RELAY_AC,
        OT_SONDE,
        OT_RELAY_LP
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


    String devices_room_to_string(mod_devices_room name);
    String output_type_to_string(mod_outputs_type name);
    String devices_grp_to_string(mod_devices_grp name);

    mod_devices_room    devices_room_string_to_mod(String name);
    mod_outputs_type    output_type_string_to_mod(String name);
    mod_devices_grp     devices_grp_string_to_mod(String name) ;
    extern mod_devices_grp mod_devices_grp_array[];
    extern mod_devices_room mod_device_room_array[];
#endif