#ifndef DEVICESOUTPUT_H
#define DEVICESOUTPUT_H 

    #include <arduino.h>

	#include "def.h"
	#include "devices_mod.h"
	#include "devicesmodule.h"

    #define MAX_GROUPS_OUTPUT   15

    class output_edit {
    public: 
        String  outputsArray[MAX_GROUPS_OUTPUT];
        int     count = 0; 
        output_edit();
        void load();

        int     spiffgetCount    ();
        int     spiffToArray     (String * array);
        void    display          (output_edit * ptr);
        void    update           (String dn, String on, String newGrp, String newRoom, String newOn);
        void    update           (output_edit * ptr);
        void    add              (String device, String ouput, String group, String room, String outputName);
        void    removeDevice     (String device);

        String set_output_info(String s, mod_outputs_type *pos);
        String set_output_info(String s, mod_devices_room *pos);
        String set_output_info(String s, mod_devices_grp *pos);
        String set_output_info(String s, int *pos);
        String set_output_info(String s, uint8_t *pos);
        String set_output_info(String s, String *pos);
        String set_output_info(String s, bool *pos);
        String get_output_info(mod_outputs_type *pos, int count);
        String get_output_info(mod_devices_room *pos, int count);
        String get_output_info(mod_devices_grp *pos, int count);
        String get_output_info(String *pos, int count);
        String get_output_info(bool *pos, int count);
        String get_output_info(int *pos, int count);
        String get_output_info(uint8_t *pos, int count);

        boolean output_isRelay      (mod_outputs_type type);
        boolean output_isStrip      (mod_outputs_type type);
        boolean output_isRGBWStrip  (mod_outputs_type type);
        boolean output_isRGBStrip   (mod_outputs_type type); 

        void device_add(String s, int pos);

    };
    output_edit * output_editPtr_get();
    void 	outputEdit_setup(output_edit * ptr);

#endif		