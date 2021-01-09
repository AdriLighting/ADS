#include "devicesoutput.h"
#include <adri_tools.h>

output_edit * output_editPtr;
output_edit * output_editPtr_get(){return output_editPtr;}
output_edit::output_edit(){ 
    output_editPtr = this;
    
    add("AMPLI",                 "ampli",        "center",           "",     "");
    add("FLEUR",                 "fleur",        "center",           "",     "");
    add("PLAFOND_LIT",           "celling_2",    "plafond",          "",     "");
    add("PLAFOND_ENTER",         "celling_e",    "plafond",          "",     "");
    add("PLAFOND_RELAY",         "ceiling",      "plafond",          "",     "");
    add("SALON_CANAPE_RELAY",    "canape",       "salle a manger",   "salon",    "");
    add("BUREAU_ECRAN",          "center",       "",                 "chambre",  "");

    load();
    display(this);    
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


void outputEdit_setup(output_edit * ptr){

}

int output_edit::spiffgetCount() {
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
int output_edit::spiffToArray(String * array) {
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

void output_edit::display(output_edit * ptr) {
    Serial.printf("\n[display]\n");
    // Serial.printf("ptr->count: %d\n", ptr->count);
    if ( ptr->count < 0 ) return;

    String nbr = "[i]";
    while (nbr.length()<6) nbr+=" ";
    String dn   = "dn";
    while (dn.length()<20) dn+=" ";
    String out  = "on";
    while (out.length()<10) out+=" ";
    String grp  = "new grp";
    while (grp.length()<15) grp+=" ";
    String room  = "new room";
    while (grp.length()<10) room+=" ";    
    String on   = "new on";
    while (on.length()<10) on+=" ";  
    Serial.printf("%s %s - %s - %s - %s - %s\n", nbr.c_str(), dn.c_str(), out.c_str(), grp.c_str(), room.c_str(), on.c_str());

    for (int i = 0; i < ptr->count; ++i) {
        String str  = ptr->outputsArray[i];
        nbr = "["+String(i)+"]";
        while (nbr.length()<6) nbr+=" ";
        dn  = literal_value("dn", str);
        while (dn.length()<20) dn+=" ";
        out = literal_value("ouput", str);
        while (out.length()<10) out+=" ";
        grp = literal_value("group", str);
        while (grp.length()<15) grp+=" ";
        room = literal_value("room", str);
        while (room.length()<10) room+=" ";        
        on  = literal_value("outputName", str);
        while (on.length()<10) on+=" ";        
        Serial.printf("%s %s - %s - %s - %s - %s\n", nbr.c_str(), dn.c_str(), out.c_str(), grp.c_str(), room.c_str(), on.c_str());
    } 
}

void output_edit::update(String dn, String on, String newGrp, String newRoom, String newOn) {
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
void output_edit::update(output_edit * ptr) {
    if ( ptr->count < 0 ) return;

    for (int i = 0; i < ptr->count; ++i) {
        String str  = ptr->outputsArray[i];
        String dn   = literal_value("dn",           str);
        String on   = literal_value("ouput",        str);
        String grp  = literal_value("group",        str);
        String room = literal_value("room",        str);
        update(dn, on, grp, room, "onN");
    } 
}

void output_edit::add(String device, String ouput, String group, String room, String outputName){

    Serial.printf("\n[add]\n");

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

        int ac = spiffgetCount();
        Serial.printf("spiffgetCount: %d\n", ac);
        if (ac < 0 ) return;

        String temparay[MAX_GROUPS_OUTPUT];
        int oc = spiffToArray(temparay);
        Serial.printf("spiffToArray: %d\n", oc);
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

void output_edit::removeDevice(String device){

    Serial.printf("\n[add]\n");

    String path = "/devcices_output.txt";

    File file = SPIFFS.open(path, "r");

    String line = "";

    if (!file) { 
        return;     
    }
    else {

        file.close();

        int ac = spiffgetCount();
        Serial.printf("spiffgetCount: %d\n", ac);
        if (ac < 0 ) return;

        String temparay[MAX_GROUPS_OUTPUT];
        int oc = spiffToArray(temparay);
        Serial.printf("spiffToArray: %d\n", oc);
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




String output_edit::set_output_info(String s, int *pos){
    String list[7];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = list[j].toInt();
    }
}
String output_edit::set_output_info(String s, mod_outputs_type *pos){
    String list[7];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = output_type_string_to_mod(list[j]);
    }
}
String output_edit::set_output_info(String s, mod_devices_room *pos){
    String list[7];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = devices_room_string_to_mod(list[j]);
    }
}
String output_edit::set_output_info(String s, mod_devices_grp *pos){
    String list[7];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = devices_grp_string_to_mod(list[j]);
    }
}
String output_edit::set_output_info(String s, String *pos){
    String list[7];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = list[j];
    }
}
String output_edit::set_output_info(String s, bool *pos){
    String list[7];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        if (list[j] == "0") pos[j] = false;
        else pos[j] = true;
    }
}
String output_edit::set_output_info(String s, uint8_t *pos){
    String list[7];
    int count = explode(s, ';', list);
    for( int j = 0; j < count; j++) {
        pos[j] = list[j].toInt();
    }
}


String output_edit::get_output_info(mod_outputs_type *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = output_type_to_string(pos[j]);
            if ( j < count -1) ret += v + ";";
            else ret += v;
    }
    return ret;
}
void output_edit::get_output_info(mod_outputs_type *pos, int count, JsonArray & object){
    // fsprintf("[get_output_info]\n");
    for( int j = 0; j < count; j++) {
        String v = output_type_to_string(pos[j]);
        object.add(v);
        // fsprintf("\t%S\n",v.c_str());
    }
}
String output_edit::get_output_info(mod_devices_room *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = devices_room_to_string(pos[j]);
            if ( j < count -1) ret += v + ";";
            else ret += v;
    }
    return ret;
}
void output_edit::get_output_info(mod_devices_room *pos, int count, JsonArray & object){
    // fsprintf("[get_output_info]\n");
    for( int j = 0; j < count; j++) {
        String v = devices_room_to_string(pos[j]);
        object.add(v);
        // fsprintf("\t%S\n",v.c_str());
    }
}
String output_edit::get_output_info(mod_devices_grp *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = devices_grp_to_string(pos[j]);
            if ( j < count -1) ret += v + ";";
            else ret += v;
    }
    return ret;
}
void output_edit::get_output_info(mod_devices_grp *pos, int count, JsonArray & object){
    // fsprintf("[get_output_info]\n");
    for( int j = 0; j < count; j++) {
        String v = devices_grp_to_string(pos[j]);
        object.add(v);
        // fsprintf("\t%S\n",v.c_str());
    }
}
String output_edit::get_output_info(String *pos, int count){
    // fsprintf("\n[get_output_info]\n");
    String ret;
    for( int j = 0; j < count; j++) {
        String v = pos[j];
            if ( j < count -1) ret += v + ";";
            else ret += v;
            // fsprintf("\t%S\n",v.c_str());
    }
    return ret;
}
void output_edit::get_output_info(String *pos, int count, JsonArray & object){
    // fsprintf("[get_output_info]\n");
    for( int j = 0; j < count; j++) {
        String v = pos[j];
        object.add(v);
        // fsprintf("\t%S\n",v.c_str());
    }
}
String output_edit::get_output_info(bool *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = String(pos[j]);
            if ( j < count -1) ret += v + ";";
            else ret += v;
    }
    return ret;
}
void output_edit::get_output_info(bool *pos, int count, JsonArray & object){
    // fsprintf("[get_output_info]\n");
    for( int j = 0; j < count; j++) {
        String v = String(pos[j]);
        object.add(v);
        // fsprintf("\t%S\n",v.c_str());
    }
}
String output_edit::get_output_info(int *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = String(pos[j]);
            if ( j < count -1) ret += v + ";";
            else ret += v;
    }
    return ret;
}
void output_edit::get_output_info(int *pos, int count, JsonArray & object){
    // fsprintf("[get_output_info]\n");
    for( int j = 0; j < count; j++) {
        String v = String(pos[j]);
        object.add(v);
        // fsprintf("\t%S\n",v.c_str());
    }
}
String output_edit::get_output_info(uint8_t *pos, int count){
    String ret;
    for( int j = 0; j < count; j++) {
        String v = String(pos[j]);
            if ( j < count -1) ret += v + ";";
            else ret += v;
    }
    return ret;
}
void output_edit::get_output_info(uint8_t *pos, int count, JsonArray & object){
    // fsprintf("[get_output_info]\n");
    for( int j = 0; j < count; j++) {
        String v = String(pos[j]);
        object.add(v);
        // fsprintf("\t%S\n",v.c_str());
    }
}

boolean output_edit::output_isRelay(mod_outputs_type type) {
    if (type == OT_RGBSTRIP) return false;
    if (type == OT_RGBWSTRIP) return false;
    return true;
}
boolean output_edit::output_isStrip(mod_outputs_type type) {
    if (type == OT_RELAY_LP) return false;
    if (type == OT_RELAY_AC) return false;
    return true;
}
boolean output_edit::output_isRGBWStrip(mod_outputs_type type) {
    if (type == OT_RGBWSTRIP) return true;
    return false;
}
boolean output_edit::output_isRGBStrip(mod_outputs_type type) {
    if (type == OT_RGBSTRIP) return true;
    return false;
}


void output_edit::device_add(String s, int pos){

    // Serial.printf("\n[device_add] pos: %d - name: %s\n",pos,s.c_str() );
    
    String os;

    if(!devices_array[pos].isAdded){

        os = literal_value("0", s);
        devices_array[pos].device_name = os;

        os = literal_value("1", s);
        devices_array[pos].device_ip = os;

        os = literal_value("2", s);
        devices_array[pos].output_count = os.toInt();

        os = literal_value("3", s);
        set_output_info(os, devices_array[pos].output_pin); 

        os = literal_value("4", s);
        set_output_info(os, devices_array[pos].output_type);

        os = literal_value("5", s);
        set_output_info(os, devices_array[pos].device_room);

        os = literal_value("6", s);
        set_output_info(os, devices_array[pos].output_grp);

        os = literal_value("7", s);
        set_output_info(os, devices_array[pos].output_location);

    }

    os = literal_value("8", s);
    set_output_info(os, devices_array[pos].toggleOnOff);

    // os = literal_value("9", s);
    // set_output_info(os, devices_array[pos].output_state);

    // os = literal_value("10", s);
    // set_output_info(os, devices_array[pos].strip_size);

    os = literal_value("11", s);
    set_output_info(os, devices_array[pos].strip_color);

    // os = literal_value("12", s);
    // set_output_info(os, devices_array[pos].strip_colorOff);

    os = literal_value("13", s);
    set_output_info(os, devices_array[pos].strip_bri);

    os = literal_value("14", s);
    set_output_info(os, devices_array[pos].output_selected);

    // os = literal_value("15", s);
    // set_output_info(os, devices_array[pos].patternStrip_selected);

    if (!devices_array[pos].isAdded) devices_array[pos].device_lib = literal_value("16", s);

    os = literal_value("17", s);
    set_output_info(os, devices_array[pos].strip_hue);

    os = literal_value("18", s);
    set_output_info(os, devices_array[pos].strip_sat);

    os = literal_value("19", s);
    set_output_info(os, devices_array[pos].strip_hBri);    

    os = literal_value("20", s);
    set_output_info(os, devices_array[pos].trueWhite);    



    if (!devices_array[pos].isAdded) devices_array[pos].isAdded = true;             
}
