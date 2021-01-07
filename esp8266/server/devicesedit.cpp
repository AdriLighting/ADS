#include "devicesedit.h"

#include <adri_tools.h>
#include "devices_mod.h"

devices_edit::devices_edit(){
    // SPIFFS.remove("/devcices_edit.txt");
    parametres_spiffToArray();
    
    // parametres_add("plafond",           "grp");
    // parametres_add("salle a manger",    "grp");

    // parametres_add("salle a manger",    "room");

    parametres_display();
    group_setup();
    room_setup();
    group_display();
    room_display();        
}

void devices_edit::group_setup(){

    int count = 0;

    for (int i = DG_BUREAU; i < DG_UNKNOW; ++i) {
        String name = devices_grp_to_string(mod_devices_grp_array[i]); 
        if (name=="")continue;
        groupes[count] = devices_grp_to_string(mod_devices_grp_array[i]); 
        count++;       
    }   
    grp_count = count;

    int pos = parametres_getPos("grp");

    if (pos < 0 ) return;

    String value = literal_value("value", parametres[pos]); 

    String list[20];

    int expCnt = explode(value, ';', list);
    for( int j = 0; j < expCnt-1; j++) {
        groupes[grp_count] = list[j]; 
        grp_count++;
    }

}
void devices_edit::group_display(){
    Serial.printf("\n[group_display] count: %d\n", grp_count);
    for (int i = 0; i < MAX_GROUPS; ++i) Serial.printf("[%d] - %s\n", i, groupes[i].c_str());
}


void devices_edit::room_setup(){

    int count = 0;

    for (int i = DR_CH; i < DR_UK; ++i) {
        String name = devices_room_to_string(mod_device_room_array[i]); 
        if (name=="")continue;
        rooms[count] = devices_room_to_string(mod_device_room_array[i]); 
        count++;       
    }   
    room_count = count;

    int pos = parametres_getPos("room");

    if (pos < 0 ) return;

    String value = literal_value("value", parametres[pos]); 

    String list[20];

    int expCnt = explode(value, ';', list);
    for( int j = 0; j < expCnt-1; j++) {
        rooms[room_count] = list[j]; 
        room_count++;
    }

}
void devices_edit::room_display(){
    Serial.printf("\n[room_display] count: %d\n", grp_count);
    for (int i = 0; i < MAX_GROUPS; ++i) Serial.printf("[%d] - %s\n", i, rooms[i].c_str());
}



void devices_edit::parametres_display() {
    Serial.println("\n[parametres_display]\n");
    for (int i = 0; i < MAX_PARAM; ++i) {
        Serial.printf("[%d] %s\n", i, parametres[i].c_str());
    }  
}

int devices_edit::parametres_spiffToArray() {
    String path = "/devcices_edit.txt";
    File    file        = SPIFFS.open(path, "r");
    String  line        = "";
    int     countParam  = 0;

    while (file.position()<file.size()) {
        String xml = file.readStringUntil('\n');
        if (xml != "") {
            parametres[countParam] = xml;
            countParam++;
        }
        yield();
    } 

    file.close();

    return countParam;
}

int devices_edit::parametres_getPos(String s_op) {
    int ret = -1;
    for (int i = 0; i < MAX_PARAM; ++i) {
        String op = literal_value("op", parametres[i]);
        if (op == s_op) {
            ret = i;
            break;
        } 
    }
    return ret;
}
void devices_edit::parametres_add(String s_value, String s_op) {

    String path = "/devcices_edit.txt";

        File file = SPIFFS.open(path, "r");

        String line = "";


        if (!file) {

            line =  literal_item("op",  s_op);
            line += literal_item("value",  s_value + ";");

            
            file = SPIFFS.open(path, "w");
            file.println(line);
            file.close();

            parametres[0]=line;

        } else {

            file.close();
                 
            int countParam = parametres_spiffToArray();

            // Serial.printf("\ns_value %s - s_op %s\n", s_value.c_str(), s_op.c_str());
            // Serial.printf("countParam %d\n", countParam);

            String value = "";

            int pos = -1;

            for (int i = 0; i < MAX_PARAM; ++i) {
                String op = literal_value("op", parametres[i]);
                if (op == s_op) {
                    value = literal_value("value", parametres[i]);
                    pos = i;
                    break;
                } 
            }

            String  list[20];
            int     expCnt      = explode(value, ';', list);
            String  newValue    = "";
            for( int j = 0; j < expCnt-1; j++) {
                if (s_value == list[j]) continue;
                newValue += list[j] + ";"; 
            }
            newValue += s_value + ";"; 

            // Serial.printf("pos %d\n", pos);
            // Serial.printf("newValue %s\n", newValue.c_str());
            
            if (pos != -1) {
                line =  literal_item("op",  s_op);
                line += literal_item("value",  newValue);
                parametres[pos] = line;

            }  else {
                
                int newPos = 0;
                for (int i = 0; i < MAX_PARAM; ++i) {
                    // Serial.printf("len line: %d - %d - %s\n",i, parametres[i].length(), parametres[i].c_str());
                    if (parametres[i].length() > 1) {

                    } else {
                        newPos = i;
                        break;                        
                    }
                }  

                // Serial.printf("NEWpos %d\n", newPos);

                line =  literal_item("op",  s_op);
                line += literal_item("value",  newValue);

                parametres[newPos] = line;                
            }


            line = "";
            for (int i = 0; i < MAX_PARAM; ++i) {
                if (parametres[i] != "") line += parametres[i]+"\n";
            }  
            
            file = SPIFFS.open(path, "w");
            file.println(line);                    
            file.close();

            // Serial.printf("\nDONE\n");
         
   }

}

void devices_edit::parametres_remove(String s_value, String s_op) {

    String path = "/devcices_edit.txt";

        File file = SPIFFS.open(path, "r");

        String line = "";


        if (!file) {


        } else {

            file.close();
                 
            parametres_spiffToArray();

            String value = "";

            int pos = -1;

            for (int i = 0; i < MAX_PARAM; ++i) {
                String op = literal_value("op", parametres[i]);
                if (op == s_op) {
                    value = literal_value("value", parametres[i]);
                    pos = i;
                    break;
                } 
            }
            if (pos < 0) return;

                String  list[20];
                int     expCnt      = explode(value, ';', list);
                String  newValue    = "";
                for( int j = 0; j < expCnt-1; j++) {
                    if (s_value == list[j]) continue;
                    newValue += list[j] + ";"; 
                }
                line =  literal_item("op",  s_op);
                line += literal_item("value",  newValue);                
                parametres[pos] = line;

            line = "";
            for (int i = 0; i < MAX_PARAM; ++i) {
                line += parametres[i]+"\n";
            }  
            
            file = SPIFFS.open(path, "w");
            file.println(line);                    
            file.close();

            parametres_spiffToArray();
         
   }

}
