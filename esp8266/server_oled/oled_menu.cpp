#include "config_base.h"

#ifdef OLED_ENABLE

#include <oled_display.h>
#include "oled_menu.h"
#include "oled_menu_func.h"

#include "oled_display.h"
#include "tools.h"
#include "adriwifi.h"
#include "bmp.h"
#include "devices_display.h"
#include "devices.h"
#include "devices_mod.h"
#include "devices_preset.h"

extern devices devices;
extern devices_edit devices_edit;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

String oled_menu_selectedDevice;
String oled_menu_selectedGrp;
String oled_menu_selectedRoom;
String oled_menu_selected_grpRoom;
String c_to_string(char * name);
/*
ACTION 
    ON/OFF 
    COLOR


*/
// String om_act_light     = "lumierre";
// String om_act_color     = "color";
// String om_act_device    = "device";

boolean output_isRelay(int dPos, int oPos) {
    if (devices_array[dPos].output_type[oPos] == OT_RGBSTRIP)    return false;
    if (devices_array[dPos].output_type[oPos] == OT_RGBWSTRIP)   return false;
    return true;
}
boolean output_isStrip(int dPos, int oPos) {
    // if (output_type[pos] == OT_RELAY_LP) return false;
    // if (output_type[pos] == OT_RELAY_AC) return false;
    // return true;
    if (devices_array[dPos].output_type[oPos] == OT_RELAY_LP)   return false;
    if (devices_array[dPos].output_type[oPos] == OT_RELAY_AC)   return false;
    return true;    
}

// USER DECLAR OWN MENU
oled_menu_create * oled_menu_home;           

oled_menu_create * oled_menu_device;                

oled_menu_create * oled_menu_device_select;         

oled_menu_create * oled_menu_device_grp;     

oled_menu_create * oled_menu_device_room;  
   
oled_menu_create * oled_menu_device_room_select;     

oled_menu_create * oled_menu_device_grp_select;    

oled_menu_create * oled_menu_device_output;     

oled_menu_create * oled_menu_device_output_selected;              	

oled_menu_create * oled_menu_preset;                

// oled_menu_create * oled_menu_opt;     


boolean oled_menu_create_items_device();
boolean oled_menu_create_items_device_grp_select();
boolean oled_menu_create_items_device_output();
boolean oled_menu_create_items_preset();
void    oled_menu_create_items_device_grp();
boolean oled_menu_create_items_device_grp_room();
void    oled_menu_create_items_device_room();
boolean oled_menu_create_items_device_room_select();
void    oled_menu_create_items_device_room_grp();
boolean oled_menu_create_items_device_grp_select_room();
boolean oled_menu_create_items_options();

void m_backTo_home( oled_menu_move move)            {
    oled_menu_starterItem = "";
    oled_menu_init_menu (oled_menu_home);
}
void m_goTo_devices(            oled_menu_move move)    {oled_menu_create_items_device();           oled_menu_init_menu (oled_menu_device);       }
void m_goTo_device_grp(         oled_menu_move move)    {oled_menu_create_items_device_grp();       oled_menu_init_menu (oled_menu_device_grp);   }
void m_goTo_device_grp_room(    oled_menu_move move)    {
    oled_menu_selected_grpRoom = oled_menu_current->getItemName();
    if (oled_menu_create_items_device_grp_room()) oled_menu_init_menu (oled_menu_device_grp);   
}
void m_goTo_device_grp_room_2(    oled_menu_move move)    {
    if (oled_menu_create_items_device_grp_room()) oled_menu_init_menu (oled_menu_device_grp);   
}
void m_goTo_device_room_grp(    oled_menu_move move)    {
    oled_menu_selected_grpRoom = "";
    oled_menu_create_items_device_room_grp();  
    oled_menu_init_menu (oled_menu_device_room);   
}
void m_goTo_opt(    oled_menu_move move)    {
    // oled_menu_create_items_options();
    // oled_menu_init_menu (oled_menu_opt);   
}
void m_goTo_device_room(        oled_menu_move move)    {
    oled_menu_selected_grpRoom = "";
    oled_menu_create_items_device_room();      
    oled_menu_init_menu (oled_menu_device_room);   
}
void m_goTo_device_select_2(    oled_menu_move move)    {oled_menu_init_menu (oled_menu_device_select);}
void m_goTo_device_select_1(    oled_menu_move move)       {
    oled_menu_starterItem = oled_menu_current->getItemName();
    oled_menu_init_menu (oled_menu_device_select);
}

void m_goTo_device_output( oled_menu_move move) {
    if (oled_menu_create_items_device_output()) {
        oled_menu_init_menu (oled_menu_device_output);
    }
}
void m_goTo_device_select_grp( oled_menu_move move) {
    if (oled_menu_create_items_device_grp_select()) {
        oled_menu_init_menu (oled_menu_device_grp_select);
    }
}
void m_goTo_device_select_grp_room( oled_menu_move move) {
    if (oled_menu_create_items_device_grp_select_room()) {
        oled_menu_init_menu (oled_menu_device_grp_select);
    }
}
void m_goTo_device_select_room( oled_menu_move move) {
    if (oled_menu_create_items_device_room_select()) {
        oled_menu_init_menu (oled_menu_device_room_select);
    }
}
void m_goTo_preset( oled_menu_move move) {
    if (oled_menu_create_items_preset()) {
        oled_menu_init_menu (oled_menu_preset);
    }
}
void m_goTo_time( oled_menu_move move) {
    oled_display_mod = oled_display_time;
}
// const char* const arb_homeMenu_1[] PROGMEM = { omN_m_home, omN_m_device_select} ;
// const char* const arb_homeMenu_1_1[] PROGMEM = { omN_m_device_grp,   omN_m_device_grp_select} ;
// const char* const arb_homeMenu_1_2[] PROGMEM = { omN_m_device,       omN_m_device_output} ;
char PROGMEM om_act_light   [] = "on/off";
char PROGMEM om_act_color   [] = "color";
char PROGMEM om_act_time    [] = "horloge";
char PROGMEM om_act_preset  [] = "preset";
char PROGMEM om_act_rommGrp [] = "room_group";
// char PROGMEM om_act_opt     [] = "options";
PROGMEM oled_menu_item list_m1 [] = {           
    {om_act_light,  "", &om_tf, &om_tf, &om_tf,     &m_goTo_device_select_1},
    {om_act_color,  "", &om_tf, &om_tf, &om_tf,     &m_goTo_device_select_1},
    {om_act_preset, "", &om_tf, &om_tf, &om_tf,     &m_goTo_preset},
    {om_act_time,   "", &om_tf, &om_tf, &om_tf,     &m_goTo_time},
    // {om_act_opt,    "", &om_tf, &om_tf, &om_tf,     &m_goTo_opt},
};
PROGMEM oled_menu_item list_m2 [] = {           
    {"groupe",      "", &om_tf, &om_tf, &m_backTo_home, &m_goTo_device_grp},
    {"device",      "", &om_tf, &om_tf, &m_backTo_home, &m_goTo_devices},
    {"room",        "", &om_tf, &om_tf, &m_backTo_home, &m_goTo_device_room},
    {om_act_rommGrp,"", &om_tf, &om_tf, &m_backTo_home, &m_goTo_device_room_grp},
};

boolean om_toggle_select = false;
void om_toggle_select_change(oled_menu_move move) {
    // om_toggle_select = !om_toggle_select;
    // oled_menu_create_items_options();
    // oled_menu_init_menu (oled_menu_opt);   

}
boolean oled_menu_create_items_options(){    

    // int i = 0;
    // oled_menu_opt->contents[i].name       = "toggle_select"; 
    // oled_menu_opt->contents[i].subTitle   = "toggle: " + String(om_toggle_select); 
    // oled_menu_opt->contents[i].func_1     = &om_tf; 
    // oled_menu_opt->contents[i].func_2     = &om_tf;                        
    // oled_menu_opt->contents[i].func_3     = &m_backTo_home;                         
    // oled_menu_opt->contents[i].func_4     = &om_tf;  
    // oled_menu_opt->contents[i].info_1     = "";          
    // oled_menu_opt->size = 1;
    // return true;
}


boolean oled_menu_create_items_preset(){    
    if (devices_preset_array_nbr < 0) return false;

    for (int i = 0; i < devices_preset_array_nbr; ++i) {

        String lbl      = devices_preset_array[i]->lbl;
        String filePath = devices_preset_array[i]->filePath;

        oled_menu_preset->contents[i].name       = lbl; 
        oled_menu_preset->contents[i].subTitle   = ""; 
        oled_menu_preset->contents[i].func_1     = &om_tf; 
        oled_menu_preset->contents[i].func_2     = &om_tf;                        
        oled_menu_preset->contents[i].func_3     = &m_backTo_home;                         
        oled_menu_preset->contents[i].func_4     = &om_tf;  
        oled_menu_preset->contents[i].info_1     = filePath;          
    }
   
    oled_menu_preset->size = devices_preset_array_nbr;
    return true;
}

boolean oled_menu_create_items_device_output(){
    String dn           = oled_menu_current->getItemName();   
    int pos             = devices.getListPos(dn);
    if (pos < 0) return false;

    oled_menu_selectedDevice    = dn;
    int count                   = 0;
    int output_count            = devices_array[pos].output_count;
    for( int i = 0; i < output_count; i++) {
        if (om_arborescence_item[0] == c_to_string(om_act_color)) {
            if (!output_isStrip(pos, i)) continue;
        }
        count++;
        oled_menu_device_output->contents[i].name       = devices_array[pos].output_location[i]; 
        oled_menu_device_output->contents[i].subTitle   = devices_array[pos].output_grp[i]; 
        oled_menu_device_output->contents[i].func_1     = &om_tf; 
        oled_menu_device_output->contents[i].func_2     = &om_tf;                        
        oled_menu_device_output->contents[i].func_3     = &m_goTo_devices;                         
        oled_menu_device_output->contents[i].func_4     = &om_tf;  
        oled_menu_device_output->contents[i].info_1     = devices_array[pos].device_name + "|" + String(i);  
    }
    if (count == 0) return false;
    oled_menu_device_output->size = count;
    return true;
}

boolean oled_menu_create_items_device(){
    int count = 0;
    for( int i = 0; i < devices_count; i++) { 
        // if (!devices_array[i].isConnected) continue;
        oled_menu_device->contents[count].name      = devices_array[i].device_name; 
        oled_menu_device->contents[count].subTitle  = "connected: " + String(devices_array[i].isConnected);  
        oled_menu_device->contents[count].func_1    = &om_tf; 
        oled_menu_device->contents[count].func_2    = &om_tf;                         
        oled_menu_device->contents[count].func_3    = &m_goTo_device_select_2;                         
        oled_menu_device->contents[count].func_4    = &m_goTo_device_output; 
        oled_menu_device->contents[count].info_1    = "";  

        count++;                       
    } 
    if (count==0) return false;
    oled_menu_device->size = count;
    return true;
}

boolean oled_menu_create_items_device_grp_select(){
   
    String dn               = oled_menu_current->getItemName();   
    oled_menu_selectedGrp   = dn;
    int count               = 0;
    int output_count;
    for( int i = 0; i < devices_count; i++) {
        output_count = devices_array[i].output_count;
        for( int j = 0; j < output_count; j++) {
            if (dn != devices_array[i].output_grp[j]) continue;
            if (om_arborescence_item[0] == c_to_string(om_act_color)) {
                if (!output_isStrip(i, j)) continue;
            }

            oled_menu_device_grp_select->contents[count].name       = devices_array[i].output_location[j]; 
            oled_menu_device_grp_select->contents[count].subTitle   = "connected: " + String(devices_array[i].isConnected);  
            oled_menu_device_grp_select->contents[count].func_1     = &om_tf; 
            oled_menu_device_grp_select->contents[count].func_2     = &om_tf;                        
            oled_menu_device_grp_select->contents[count].func_3     = &m_goTo_device_grp;                         
            oled_menu_device_grp_select->contents[count].func_4     = &om_tf;  

            oled_menu_device_grp_select->contents[count].info_1     = devices_array[i].device_name + "|" + String(j);
            count++;
        }
    }
    if (count == 0) return false;
    oled_menu_device_grp_select->size = count;
    return true;
}

void oled_menu_create_items_device_grp(){
    int count = 0;
    int count_2 = 0;
    int output_count;
    for (int i = 0; i < devices_edit.grp_count; ++i) {
        
        oled_menu_device_grp->contents[count].name       = devices_edit.groupes[i]; 
        oled_menu_device_grp->contents[count].subTitle   = ""; 
        oled_menu_device_grp->contents[count].func_1     = &om_tf; 
        oled_menu_device_grp->contents[count].func_2     = &om_tf;                        
        oled_menu_device_grp->contents[count].func_3     = &m_goTo_device_select_2;                         
        oled_menu_device_grp->contents[count].func_4     = &m_goTo_device_select_grp;   
        oled_menu_device_grp->contents[count].info_1     = "";
        count++;  
     
    }       

    oled_menu_device_grp->size = count;
}

boolean oled_menu_create_items_device_grp_select_room(){
   
    String dn               = oled_menu_current->getItemName();   
    oled_menu_selectedGrp   = dn;
    int count               = 0;
    int output_count;
    for( int i = 0; i < devices_count; i++) {
        output_count = devices_array[i].output_count;
        for( int j = 0; j < output_count; j++) {
            if (dn != devices_array[i].output_grp[j]) continue;
            
            if (om_arborescence_item[0] == c_to_string(om_act_color)) {
                if (!output_isStrip(i, j)) continue;
            }

            if (om_arborescence_item[1] == c_to_string(om_act_rommGrp)) {
                if (om_arborescence_item[2] != devices_array[i].device_room[j]) continue;
            }

            oled_menu_device_grp_select->contents[count].name       = devices_array[i].output_location[j]; 
            oled_menu_device_grp_select->contents[count].subTitle   = "connected: " + String(devices_array[i].isConnected);  
            oled_menu_device_grp_select->contents[count].func_1     = &om_tf; 
            oled_menu_device_grp_select->contents[count].func_2     = &om_tf;                        
            oled_menu_device_grp_select->contents[count].func_3     = &m_goTo_device_grp_room_2;                         
            oled_menu_device_grp_select->contents[count].func_4     = &om_tf;  

            oled_menu_device_grp_select->contents[count].info_1     = devices_array[i].device_name + "|" + String(j);
            count++;
        }
    }
    if (count == 0) return false;
    oled_menu_device_grp_select->size = count;
    return true;
}

boolean oled_menu_create_items_device_room_select(){
   
    String dn               = oled_menu_current->getItemName();   
    oled_menu_selectedRoom  = dn;
    int count               = 0;
    int output_count;
    for( int i = 0; i < devices_count; i++) {
        output_count = devices_array[i].output_count;
        for( int j = 0; j < output_count; j++) {

            if (dn != devices_array[i].device_room[j]) continue;

            if (om_arborescence_item[0] == c_to_string(om_act_color)) {
                if (!output_isStrip(i, j)) continue;
            }
            oled_menu_device_room_select->contents[count].name       = devices_array[i].output_location[j]; 
            oled_menu_device_room_select->contents[count].subTitle   = "connected: " + String(devices_array[i].isConnected);  
            oled_menu_device_room_select->contents[count].func_1     = &om_tf; 
            oled_menu_device_room_select->contents[count].func_2     = &om_tf;                        
            oled_menu_device_room_select->contents[count].func_3     = &m_goTo_device_room;                         
            oled_menu_device_room_select->contents[count].func_4     = &om_tf;  

            oled_menu_device_room_select->contents[count].info_1     = devices_array[i].device_name + "|" + String(j);
            count++;
        }
    }
    if (count == 0) return false;
    oled_menu_device_room_select->size = count;
    return true;
}

boolean oled_menu_create_items_device_grp_room(){
    int count = 0;
    int output_count;
    String search = "roomgrp_" + oled_menu_selected_grpRoom;
    int pos = devices_edit.parametres_getPos(search);
    String  list[20];
    String value = literal_value("value", devices_edit.parametres[pos]);
    int expCnt = explode(value, ';', list); 

    boolean addGrp = false;   
    for (int i = 0; i < devices_edit.grp_count; ++i) {

        addGrp = false;
        for( int j = 0; j < expCnt; j++) {
            if (devices_edit.groupes[i] == list[j]) {addGrp = true;}
        }

        if (!addGrp) continue;

        oled_menu_device_grp->contents[count].name       = devices_edit.groupes[i]; 
        oled_menu_device_grp->contents[count].subTitle   = ""; 
        oled_menu_device_grp->contents[count].func_1     = &om_tf; 
        oled_menu_device_grp->contents[count].func_2     = &om_tf;                        
        oled_menu_device_grp->contents[count].func_3     = &m_goTo_device_room_grp;                         
        oled_menu_device_grp->contents[count].func_4     = &m_goTo_device_select_grp_room;   
        oled_menu_device_grp->contents[count].info_1     = "";
        count++;          
    }       
    if (count == 0) return false;
    oled_menu_device_grp->size = count;
    return true;
}
void oled_menu_create_items_device_room(){
    int count = 0;
    for (int i = 0; i < devices_edit.room_count; ++i) {
        oled_menu_device_room->contents[count].name       = devices_edit.rooms[i]; 
        oled_menu_device_room->contents[count].subTitle   = ""; 
        oled_menu_device_room->contents[count].func_1     = &om_tf; 
        oled_menu_device_room->contents[count].func_2     = &om_tf;                        
        oled_menu_device_room->contents[count].func_3     = &m_goTo_device_select_2;                         
        oled_menu_device_room->contents[count].func_4     = &m_goTo_device_select_room;   
        oled_menu_device_room->contents[count].info_1     = "";
        count++;       
    }       

    oled_menu_device_room->size = count;
}
void oled_menu_create_items_device_room_grp(){
    int count = 0;

    for (int i = 0; i < devices_edit.room_count; ++i) {
        oled_menu_device_room->contents[count].name       = devices_edit.rooms[i]; 
        oled_menu_device_room->contents[count].subTitle   = ""; 
        oled_menu_device_room->contents[count].func_1     = &om_tf; 
        oled_menu_device_room->contents[count].func_2     = &om_tf;                        
        oled_menu_device_room->contents[count].func_3     = &m_goTo_device_select_2;                         
        oled_menu_device_room->contents[count].func_4     = &m_goTo_device_grp_room;   
        oled_menu_device_room->contents[count].info_1     = "";
        count++;       
    }       

    oled_menu_device_room->size = count;
}

PROGMEM oled_menu_longClick_1 oled_menu_longClick_1_list [] = { 

};


PROGMEM oled_menu_longClick_2 oled_menu_longClick_2_list [] = { 

};

PROGMEM oled_menu_longClick_3 oled_menu_longClick_3_list [] = { 

};

PROGMEM oled_menu_longClick_4 oled_menu_longClick_4_list [] = { 

};

void oled_init() {
    #ifdef ADAFRUIT_SD1306_LIB
        if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
            Serial.println(F("SSD1306 allocation failed"));
            for(;;); 
        }
        oled_clear();
        display.display();
    #endif 
    #ifdef SD1306WIRE_LIB
        display.init();
        display.flipScreenVertically();
        display.setFont(ArialMT_Plain_10);  
        oled_clear();
        display.display();        
    #endif   
}
void oled_menu_setup(){
    oled_setting_mod = oled_settings_none;
    om_toggle_select = false;

    // DEFINE LONG_CLICK LIST SIZE
    oled_menu_longClick_1_count = ARRAY_SIZE(oled_menu_longClick_1_list);
    oled_menu_longClick_2_count = ARRAY_SIZE(oled_menu_longClick_2_list);
    oled_menu_longClick_3_count = ARRAY_SIZE(oled_menu_longClick_3_list);
    oled_menu_longClick_4_count = ARRAY_SIZE(oled_menu_longClick_4_list);

    // USER CREATE OWN MENU OJECT
    oled_menu_home                      = new oled_menu_create("home");
    oled_menu_device                    = new oled_menu_create("device");
    oled_menu_device_select             = new oled_menu_create("device_select");
    oled_menu_device_grp                = new oled_menu_create("device_grp");
    oled_menu_device_grp_select         = new oled_menu_create("device_grp_select");
    oled_menu_device_room               = new oled_menu_create("device_room");
    oled_menu_device_room_select        = new oled_menu_create("device_room_select");
    oled_menu_device_output             = new oled_menu_create("device_output");
    oled_menu_device_output_selected    = new oled_menu_create("output_selected");
    oled_menu_preset                    = new oled_menu_create("preset");
    // oled_menu_opt                       = new oled_menu_create("options");


    // USER CREATE OWN MENU ITEMS
    oled_menu_create_items_v2(list_m1, ARRAY_SIZE(list_m1), oled_menu_home);  
    oled_menu_create_items_v2(list_m2, ARRAY_SIZE(list_m2), oled_menu_device_select); 
    oled_menu_create_items_device();
    oled_menu_create_items_device_grp();
    oled_menu_create_items_device_room();
    // oled_menu_create_items_device_output();

    for( int i = 0; i < oled_menu_array_nbr; i++) {
    	// oled_menu_array[i]->print();
    	oled_menu_array[i]->clickmoveUp 	= true;
    	oled_menu_array[i]->clickmoveDown 	= true;
    	oled_menu_array[i]->clickmoveFunc 	= 1;

        oled_menu_array[i]->cursor          = 0;
        oled_menu_array[i]->startItem       = 0;
        oled_menu_array[i]->itemsCnt        = 0; 

        oled_menu_array[i]->item_perPage    = 3;

        oled_menu_array[i]->i_yMenu         = 20;
        oled_menu_array[i]->i_xTitle        = 10;
        oled_menu_array[i]->i_yTitle        = 0;            
        oled_menu_array[i]->i_xSubTitle     = 12;       
    }

	oled_menu_set_main(oled_menu_home);
	oled_display_mod = oled_display_menu;
    oled_menu_click_initMenu(oled_menu_main); 
    
}

#endif