#include "config_base.h"


#ifndef DEVICES_PRESET_H
	#define DEVICES_PRESET_H
    #include <arduino.h>
    #include <FS.h>   


    #define MAX_PRESET 15

    class devices_preset {
    public: 
        boolean     empty;
        String      filePath;
        String      lbl;
        devices_preset(String path);
    };
    extern int devices_preset_array_nbr;
    extern devices_preset * devices_preset_array[];

    void preset_display();
    void preset_load() ;
#endif