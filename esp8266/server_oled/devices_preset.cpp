#include "config_base.h"

#include "devices_preset.h"


devices_preset * devices_preset_array[MAX_PRESET];

int devices_preset_array_nbr = 0;

devices_preset::devices_preset(String path){   
    if (devices_preset_array_nbr < MAX_PRESET) {
        lbl = String(devices_preset_array_nbr);
        filePath = path;
        devices_preset_array[devices_preset_array_nbr++] = this;
    }  
}



void preset_load() {
    String  path;
    String  send;
    String  folder  = "/output_preset/";
    String  ext     = ".txt";    
    String  s       = "";
    int     count   = 0;

    for( int i = 0; i < MAX_PRESET; i++) {
        path = folder + String(i) + ext;
        send= "";
        File file = SPIFFS.open(path, "r");
        if (file) {
            new devices_preset(path);
            file.close();
        } 
    }
}
    
void preset_display() {
    Serial.printf("\n[preset_display]\n");
    for (int j = 0; j < devices_preset_array_nbr; ++j) {
        String nbr = String(j);
        while (nbr.length()<10) nbr+=" ";
        String lbl      = devices_preset_array[j]->lbl;
        while (lbl.length()<10) lbl+=" ";
        String filePath = devices_preset_array[j]->filePath;
        Serial.printf("%s - %s - %s\n", nbr.c_str(), lbl.c_str(), filePath.c_str());
    }
}
