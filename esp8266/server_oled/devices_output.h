#include "config_base.h"


#ifndef DEVICES_OUTPUT_H
	#define DEVICES_OUTPUT_H
    #include <arduino.h>
    #include <FS.h>   
    #define MAX_GROUPS_OUTPUT   15

    class output_edit {
    public: 
        String  outputsArray[MAX_GROUPS_OUTPUT];
        int     count = 0; 
        output_edit();
        void load();
    };
    
    void outputEdit_display();
    void outputEdit_add(String device, String ouput, String group, String room, String outputName);
    void outputEdit_update();
    void outputEdit_removeDevice(String device);
    void output_edit_setup();
#endif