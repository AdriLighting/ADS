#include "config_base.h"
#include "devices_output.h"
#include "devices.h"
#include "tools.h"

output_edit::output_edit(){ }
output_edit output_edit;

void output_edit_setup(){
    outputEdit_add("AMPLI",                 "ampli",        "center",           "",     "");
    outputEdit_add("FLEUR",                 "fleur",        "center",           "",     "");
    outputEdit_add("PLAFOND_LIT",           "celling_2",    "plafond",          "",     "");
    outputEdit_add("PLAFOND_ENTER",         "celling_e",    "plafond",          "",     "");
    outputEdit_add("PLAFOND_RELAY",         "ceiling",      "plafond",          "",     "");
    outputEdit_add("SALON_CANAPE_RELAY",    "canape",       "salle a manger",   "salon",    "");
    outputEdit_add("BUREAU_ECRAN",          "center",       "",                 "chambre",  "");

    // outputEdit_removeDevice("AMPLI");

    output_edit.load();
    outputEdit_display();    
}

int outputEdit_spiffgetCount() {
    String  path = "/devcices_output.txt";
    File    file = SPIFFS.open(path, "r");

    if (!file) return -1;

    int     countParam  = 0;
    String  xml;

    while (file.position()<file.size()) {
        xml = file.readStringUntil('\n');
        if (xml != "") {countParam++;}
        yield();
    } 
    file.close();
    return countParam;
}
int outputEdit_spiffToArray(String * array) {
    String  path = "/devcices_output.txt";
    File    file = SPIFFS.open(path, "r");

    if (!file) return-1;

    int     countParam  = 0;
    String  xml;

    while (file.position()<file.size()) {
        xml = file.readStringUntil('\n');
        if (xml != "") {
            array[countParam] = xml;
            countParam++;
        }
        yield();
    } 
    file.close();
    return countParam;
}
void output_edit::load(){
    // Serial.printf("\n[output_edit::load]\n");

    String  path = "/devcices_output.txt";
    File    file = SPIFFS.open(path, "r");

    if (!file) {
        count = -1; 
        return;
    }

    int     countParam  = 0;
    String  xml;

    while (file.position()<file.size()) {
        xml = file.readStringUntil('\n');
        if (xml != "") {
            outputsArray[countParam] = xml;
            countParam++;
        }
    } 
    file.close();
    
    count = countParam;
}

void outputEdit_display() {
    Serial.printf("\n[outputEdit_display]\n");
    // Serial.printf("output_edit.count: %d\n", output_edit.count);
    if ( output_edit.count < 0 ) return;

    String nbr = "[i]";
    while (nbr.length()<6) nbr+=" ";
    String dn   = "dn";
    while (dn.length()<20) dn+=" ";
    String out  = "on";
    while (out.length()<10) out+=" ";
    String grp  = "new grp";
    while (grp.length()<10) grp+=" ";
    String room  = "new room";
    while (grp.length()<10) room+=" ";    
    String on   = "new on";
    while (on.length()<10) on+=" ";  
    Serial.printf("%s %s - %s - %s - %s\n", nbr.c_str(), dn.c_str(), out.c_str(), grp.c_str(), room.c_str(), on.c_str());

    for (int i = 0; i < output_edit.count; ++i) {
        String str  = output_edit.outputsArray[i];
        nbr = "["+String(i)+"]";
        while (nbr.length()<6) nbr+=" ";
        dn  = literal_value("dn", str);
        while (dn.length()<20) dn+=" ";
        out = literal_value("ouput", str);
        while (out.length()<10) out+=" ";
        grp = literal_value("group", str);
        while (grp.length()<10) grp+=" ";
        room = literal_value("room", str);
        while (room.length()<10) room+=" ";        
        on  = literal_value("outputName", str);
        while (on.length()<10) on+=" ";        
        Serial.printf("%s %s - %s - %s - %s\n", nbr.c_str(), dn.c_str(), out.c_str(), grp.c_str(), room.c_str(), on.c_str());
    } 
}


void outputEdit_update(String dn, String on, String newGrp, String newRoom, String newOn) {
    int output_count;

    for( int i = 0; i < devices_count; i++) {
        if (dn != devices_array[i].device_name) continue;
        output_count = devices_array[i].output_count;
        for( int j = 0; j < output_count; j++) {
            if (on == devices_array[i].output_location[j]) {
                if (newGrp  != "") devices_array[i].output_grp[j]   = newGrp;    
                if (newRoom != "") devices_array[i].device_room[j]  = newRoom;    
            } else {
                continue;
            }
        }
    }
}
void outputEdit_update() {
    if ( output_edit.count < 0 ) return;

    for (int i = 0; i < output_edit.count; ++i) {
        String str  = output_edit.outputsArray[i];
        String dn   = literal_value("dn",           str);
        String on   = literal_value("ouput",        str);
        String grp  = literal_value("group",        str);
        String room = literal_value("room",        str);
        outputEdit_update(dn, on, grp, room, "onN");
    } 
}

void outputEdit_add(String device, String ouput, String group, String room, String outputName){

    Serial.printf("\n[outputEdit_add]\n");

    String path = "/devcices_output.txt";

    File file = SPIFFS.open(path, "r");

    String line = "";

    if (!file) { 
        Serial.printf("[create file]\n");
        file =  SPIFFS.open(path, "w");
        line =  literal_item("dn",          device);
        line += literal_item("ouput",       ouput);        
        line += literal_item("group",       group);        
        line += literal_item("room",        room);        
        line += literal_item("outputName",  outputName);        
        file.println(line);
        file.close();        
    }
    else {

        file.close();

        int ac = outputEdit_spiffgetCount();
        Serial.printf("outputEdit_spiffgetCount: %d\n", ac);
        if (ac < 0 ) return;

        String temparay[MAX_GROUPS_OUTPUT];
        int oc = outputEdit_spiffToArray(temparay);
        Serial.printf("outputEdit_spiffToArray: %d\n", oc);
        if (oc < 0 ) return;

        int pos = -1;

        for (int i = 0; i < ac; ++i) {
            String dn = literal_value("dn", temparay[i]);
            if (dn == device) {
                pos = i;
                break;
            } 
        }

        if (pos != -1) {
            Serial.printf("[replace line] pos: %d\n", pos);
            line =  literal_item("dn",          device);
            line += literal_item("ouput",       ouput);        
            line += literal_item("group",       group); 
            line += literal_item("room",        room); 
            line += literal_item("outputName",  outputName); 
            temparay[pos] = line;

        }  else {
            Serial.printf("[create line] pos: %d\n", pos);
            line =  literal_item("dn",          device);
            line += literal_item("ouput",       ouput);        
            line += literal_item("group",       group); 
            line += literal_item("room",        room); 
            line += literal_item("outputName",  outputName); 
            temparay[ac] = line;
            ac++;
        }

        line = "";
        for (int i = 0; i < ac; ++i) {
            line += temparay[i]+"\n";
        }  
        
        file = SPIFFS.open(path, "w");
        file.print(line);                    
        file.close();
    }   
}
void outputEdit_removeDevice(String device){

    Serial.printf("\n[outputEdit_add]\n");

    String path = "/devcices_output.txt";

    File file = SPIFFS.open(path, "r");

    String line = "";

    if (!file) { 
        return;     
    }
    else {

        file.close();

        int ac = outputEdit_spiffgetCount();
        Serial.printf("outputEdit_spiffgetCount: %d\n", ac);
        if (ac < 0 ) return;

        String temparay[MAX_GROUPS_OUTPUT];
        int oc = outputEdit_spiffToArray(temparay);
        Serial.printf("outputEdit_spiffToArray: %d\n", oc);
        if (oc < 0 ) return;

        int pos = -1;

        for (int i = 0; i < ac; ++i) {
            String dn = literal_value("dn", temparay[i]);
            if (dn == device) {
                pos = i;
                break;
            } 
        }

        if (pos == -1) {return;} 

        line = "";
        for (int i = 0; i < ac; ++i) {
            if (i = pos) continue;
            line += temparay[i]+"\n";
        }  
        
        file = SPIFFS.open(path, "w");
        file.println(line);                    
        file.close();
    }   
}
