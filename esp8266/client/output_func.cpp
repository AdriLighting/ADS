#include "device_def.h"

#include "output_func.h"
#include "relay_func.h"

#include "devices.h"
#include "tools.h"
#include "adriwifi.h"

#if CLIENT_SELECT==CLIENT_LED
    #include "fastled_func.h"
#endif

#define DEBUG
#if CLIENT_SELECT==CLIENT_RELAY
    boolean command_debug = true;
#endif


   // if (name == "strip_rgb")     ret = OT_RGBSTRIP;
   // if (name == "strip_rgbw")    ret = OT_RGBWSTRIP;
   // if (name == "relay_lampe")   ret = OT_RELAY_LP;
   // if (name == "relay_ac")      ret = OT_RELAY_AC;

boolean output_isRelay(int pos) {
	if (output_type[pos] == OT_RGBSTRIP) return false;
	if (output_type[pos] == OT_RGBWSTRIP) return false;
	return true;
}
boolean output_isStrip(int pos) {
	if (output_type[pos] == OT_RELAY_LP) return false;
	if (output_type[pos] == OT_RELAY_AC) return false;
	return true;
}
boolean output_isRGBWStrip(int pos) {
    if (output_type[pos] == OT_RGBWSTRIP) return true;
    return false;
}
boolean output_isRGBStrip(int pos) {
    if (output_type[pos] == OT_RGBSTRIP) return true;
    return false;
}
int output_set_selected(String v){
   String Explode[2];

   explode(v, '|', Explode);  

    int strip = Explode[0].toInt();
    int state = Explode[1].toInt();

    output_selected[strip] = state;
}


boolean string_to_bool(String v) {
    if (v == "0") return false;
    return true;
}
int string_to_int(String v) {
    int ret = v.toInt();
    return ret;
}
void output_save_load(String s) {
    // Serial.printf("\n[output_save_load]\n");

    int count_1;
    int count_2;
    int count_3;

    String list[50];

    String list_o[50];

    String list_oi[50];

    count_1 = explode(s, '#', list);

    String line;
    for( int i = 0; i < count_1-1; i++) {

        String dn = literal_value("dn", list[i]);
        String oc = literal_value("oc", list[i]);
        String os = literal_value("os", list[i]);

        // Serial.printf("\n[%d]\n", i);
        // Serial.printf("\t dn: %s\n", dn.c_str());
        // Serial.printf("\t oc: %s\n", oc.c_str());
        // Serial.printf("\t os: %s\n", os.c_str());

            
            line += "\noutput ["+String(i)+"]\n";
            line += "\tdn: "+dn+"\n";
            line += "\toc: "+oc+"\n";
            line += "\tos: "+os+"\n";

            count_2 = explode(os, '*', list_o);
            for( int j = 0; j < count_2-1; j++) {

                // Serial.printf("\t\t output [%d]: %s\n", j, list_o[j].c_str());

                line += "\t\toutput ["+String(j)+"]: "+list_o[j]+"\n";

                count_3 = explode(list_o[j], ';', list_oi);
                for( int k = 0; k < count_3-1; k++) {
                    // Serial.printf("\t\t\t state [%d]: %s\n", k, list_oi[k].c_str());

                    if (dn == device_name) line += "\t\t\tstate ["+String(k)+"]: "+list_oi[k]+"\n";


                }  

                if (dn == device_name) {
                    int pin                     = string_to_int( list_oi[0]);;
                    toggleOnOff[pin]            = string_to_bool( list_oi[1]);
                    strip_color[pin]            = list_oi[2];
                    strip_bri[pin]              = string_to_int( list_oi[3]);
                    output_selected[pin]        = string_to_int( list_oi[4]);
                    strip_hue[pin]              = string_to_int( list_oi[5]);
                    strip_sat[pin]              = string_to_int( list_oi[6]);
                    strip_hBri[pin]             = string_to_int( list_oi[7]);
                    trueWhite[pin]              = string_to_bool( list_oi[8]);
                

                    #if CLIENT_SELECT==CLIENT_LED
                        setBriFromString(pin, strip_bri[pin] );

                        if (output_isRGBStrip(pin)){
                            if(toggleOnOff[pin])    setColorFromString(pin, true);
                            else                    setColorFromString("0.0.0", pin, true);                    
                        }

                        if (output_isRGBWStrip(pin)){
                            if (toggleOnOff[pin]) {
                                if (trueWhite[pin]) setWhite(pin, strip_hBri[pin], true);
                                else setColorFromString(pin, true);
                            } else {
                                if (trueWhite[pin]) setWhite(pin, 0, true);
                                else setColorFromString("0.0.0", pin, true);
                            }                  
                        }
                    #endif
                }     
            }
        
    }
    debug(&line);
}


void strip_selected_onOff(int strip, boolean show) {
    #if CLIENT_SELECT==CLIENT_LED
        if (output_isStrip(strip)) {
            if (output_isRGBStrip(strip))   setColorFromString(strip, show);
            if (output_isRGBWStrip(strip))  {
                if (trueWhite[strip]) setWhite(strip, strip_hBri[strip], show);
                else setColorFromString(strip, show);
            }
        }
    #endif    
}
void strip_selected_onOff(int strip, String c, boolean show) {
    #if CLIENT_SELECT==CLIENT_LED
        if (output_isStrip(strip)) {
            if (output_isRGBStrip(strip))   setColorFromString(c, strip, show);
            if (output_isRGBWStrip(strip))  {
                if (trueWhite[strip]) setWhite(strip, 0, show);
                else setColorFromString(c, strip, show);
            }
        }
    #endif    
}
int output_selected_onOff(String v, boolean select){ // output_selected_onOff : output pos|output stat 0 = on 1=off
#ifdef DEBUG
   if (command_debug) fsprintf("\n[output_selected_onOff] : %s\n", v.c_str());
#endif


    String Exploed[2];

    explode(v, '|', Exploed);

    int strip =  Exploed[0].toInt();

    if (strip > output_count) {
        #ifdef DEBUG
        if (command_debug){
        Serial.printf("[output_selected_onOff] pos %d > stripcount: %d", 
        strip, 
        output_count);}
        #endif
        return -1;
    }

    if ( select){
        patternStrip_selected[strip] = 0;
        output_selected[strip] = 1;
    }
    else {
        if (output_selected[strip] == 0) {
            return -1;
        } else {
            patternStrip_selected[strip] = 0;
        }
    }  

    int state = Exploed[1].toInt();
    if ( state == 0) {
        // if (output_isStrip(strip)) {
        //     if (output_isRGBStrip(strip))   setColorFromString(strip, true);
        //     if (output_isRGBWStrip(strip))  {
        //         if (trueWhite[strip]) setWhite(strip, strip_hBri[strip], true);
        //         else setColorFromString(strip, true);
        //     }
        // }
        strip_selected_onOff(strip, true);
        if (output_isRelay(strip)) relay_set_on(strip);
        toggleOnOff[strip] = true;
    }
    else {
        // if (output_isStrip(strip)) {
        //     if (output_isRGBStrip(strip))   setColorFromString("0.0.0", strip, true);
        //     if (output_isRGBWStrip(strip))  {
        //         if (trueWhite[strip]) setWhite(strip, 0, true);
        //         else setColorFromString("0.0.0", strip, true);
        //     }
        // }
        strip_selected_onOff(strip, "0.0.0", true);
        if (output_isRelay(strip)) relay_set_off(strip);
        toggleOnOff[strip] = false;
    } 
}

int output_selected_toggleOnOff(String v, boolean select){
#ifdef DEBUG
   if (command_debug) fsprintf("\n[output_selected_toggleOnOff] : %s\n", v.c_str());
#endif

    String Exploed[2];

    explode(v, '|', Exploed);

    int strip =  Exploed[0].toInt();

    if (strip > output_count) {
        #ifdef DEBUG
        if (command_debug){
            Serial.printf("[output_selected_toggleOnOff] pos %d > stripcount: %d", 
            strip, 
            output_count);}
        #endif
    return -1;
    }

    if ( select){
        patternStrip_selected[strip] = 0;
        output_selected[strip] = 1;
    }
    else {
        if (output_selected[strip] == 0) {
            return -1;
        } else {
            patternStrip_selected[strip] = 0;
        }
    }  

    if (!toggleOnOff[strip]) {
        // if (output_isStrip(strip)) {
        //     if (output_isRGBStrip(strip))   setColorFromString(strip, true);
        //     if (output_isRGBWStrip(strip))  {
        //         if (trueWhite[strip]) setWhite(strip, strip_hBri[strip], true);
        //         else setColorFromString(strip, true);
        //     }
        // }
        strip_selected_onOff(strip, true);
        if (output_isRelay(strip)) relay_set_on(strip);
        toggleOnOff[strip] = true;
    } else {
        // if (output_isStrip(strip)) {
        //     if (output_isRGBStrip(strip))   setColorFromString("0.0.0", strip, true);
        //     if (output_isRGBWStrip(strip))  {
        //         if (trueWhite[strip]) setWhite(strip, 0, true);
        //         else setColorFromString("0.0.0", strip, true);
        //     }
        // }
        strip_selected_onOff(strip, "0.0.0", true);
        if (output_isRelay(strip)) relay_set_off(strip);
        toggleOnOff[strip] = false;
    } 

   return strip;

}

int output_all_toggleOnOff(){
#ifdef DEBUG
   if (command_debug) fsprintln("[output_all_toggleOnOff]");
#endif
    for( int i = 0; i < output_count; i++) { 
        if (output_selected[i] == 1) {
            int numLed = strip_size[i]; 
            if (!toggleOnOff[i]) {
                // if (output_isStrip(i)) {
                //     if (output_isRGBStrip(i))   setColorFromString(i, true);
                //     if (output_isRGBWStrip(i))  {
                //         if (trueWhite[i]) setWhite(i, strip_hBri[i], true);
                //         else setColorFromString(i, true);
                //     }
                // }
                strip_selected_onOff(i, true);
                if (output_isRelay(i)) relay_set_on(i);
                toggleOnOff[i] = true;
            } else {
                // if (output_isStrip(i)) {
                //     if (output_isRGBStrip(i))   setColorFromString("0.0.0", i, true);
                //     if (output_isRGBWStrip(i))  {
                //         if (trueWhite[i]) setWhite(i, 0, true);
                //         else setColorFromString("0.0.0", i, true);
                //     }
                // }
                strip_selected_onOff(i, "0.0.0", true);
                if (output_isRelay(i)) relay_set_off(i);
                toggleOnOff[i] = false;
            }         
        }         
    }    
}

int output_all_onOff(){
#ifdef DEBUG
   if (command_debug) fsprintln("[output_all_on]");
#endif
    for( int i = 0; i < output_count; i++) { 
        if (output_selected[i] == 1) {
            if (toggleOnOff[i]){
                // if (output_isStrip(i)) {
                //     if (output_isRGBStrip(i))   setColorFromString(i, true);
                //     if (output_isRGBWStrip(i))  {
                //         if (trueWhite[i]) setWhite(i, strip_hBri[i], true);
                //         else setColorFromString(i, true);
                //     }
                // }
                strip_selected_onOff(i, true);
                if (output_isRelay(i)) relay_set_on(i);
            } else  {
                // if (output_isStrip(i)) {
                //     if (output_isRGBStrip(i))   setColorFromString("0.0.0", i, true);
                //     if (output_isRGBWStrip(i))  {
                //         if (trueWhite[i]) setWhite(i, 0, true);
                //         else setColorFromString("0.0.0", i, true);
                //     }
                // }
                strip_selected_onOff(i, "0.0.0", true);
                if (output_isRelay(i)) relay_set_off(i);                
            }
        }
    }    
}
int output_all_onOff(boolean relay){
#ifdef DEBUG
   if (command_debug) fsprintln("[output_all_on]");
#endif
    for( int i = 0; i < output_count; i++) { 
        if (output_selected[i] == 1) {
            if (toggleOnOff[i]){
                // if (output_isStrip(i)) {
                //     if (output_isRGBStrip(i))   setColorFromString(i, true);
                //     if (output_isRGBWStrip(i))  {
                //         if (trueWhite[i]) setWhite(i, strip_hBri[i], true);
                //         else setColorFromString(i, true);
                //     }
                // }
                strip_selected_onOff(i, true);
                if (output_isRelay(i)) relay_set_on(i);
            } else  {
                // if (output_isStrip(i)) {
                //     if (output_isRGBStrip(i))   setColorFromString("0.0.0", i, true);
                //     if (output_isRGBWStrip(i))  {
                //         if (trueWhite[i]) setWhite(i, 0, true);
                //         else setColorFromString("0.0.0", i, true);
                //     }
                // }
                strip_selected_onOff(i, "0.0.0", true);
                if (relay) {
                    if (output_isRelay(i)) relay_set_off(i);
                }               
            }
        }
    }    
}

int output_all_on(){
#ifdef DEBUG
   if (command_debug) fsprintln("[output_all_on]");
#endif
    for( int i = 0; i < output_count; i++) { 
        if (output_selected[i] == 1) {
            // if (output_isStrip(i)) {
            //     if (output_isRGBStrip(i)) setColorFromString(i, true);
            //     if (output_isRGBWStrip(i))  {
            //         if (trueWhite[i]) setWhite(i, strip_hBri[i], true);
            //         else setColorFromString(i, true);
            //     }
            // }
            strip_selected_onOff(i, true);
            if (output_isRelay(i)) relay_set_on(i);
            toggleOnOff[i] = true;
        }
    }    
}

int output_all_off(){
#ifdef DEBUG
   if (command_debug) fsprintln("[output_all_off]");
#endif
    for( int i = 0; i < output_count; i++) { 
        if (output_selected[i] == 1) {
            // if (output_isStrip(i)) {
            //     if (output_isRGBStrip(i))   setColorFromString("0.0.0", i, true);
            //     if (output_isRGBWStrip(i))  {
            //         if (trueWhite[i]) setWhite(i, 0, true);
            //         else setColorFromString("0.0.0", i, true);
            //     }
            // }
            strip_selected_onOff(i, "0.0.0", true);
            if (output_isRelay(i)) relay_set_off(i);
            toggleOnOff[i] = false;
        }
    }    
}

int output_all_bri(){
#ifdef DEBUG
   if (command_debug) fsprintln("[output_all_bri]");
#endif
    for( int i = 0; i < output_count; i++) { 
        if (output_selected[i] == 1) {
            #if CLIENT_SELECT==CLIENT_LED
                if (output_isStrip(i)) setBriFromString(i, strip_bri[i]);
            #endif
        }
    }    
}

int output_dDefault(){
#ifdef DEBUG
   if (command_debug) fsprintln("[output_all_stripDefault]");
#endif

    for( int i = 0; i < output_count; i++) { 
        output_selected[i]  = 1;
        toggleOnOff[i]      = true;
        strip_bri[i]        = 255;
        strip_color[i]      = "255.255.255";
        strip_hue[i]        = 80;
        strip_sat[i]        = 255;
        strip_hBri[i]       = 255;
        if(output_isRGBWStrip(i)) trueWhite[i] = true;
    }    

}


void output_preset_load(boolean relay) {

    // String disp;

    for( int i = 0; i < output_count; i++) { 

        // disp += "pin: " + String(i) + " - ";
        // disp += "os: " + String(output_selected[i]) + " - ";
        // disp += "\n";

        int state = output_selected[i] ;
        if (state == 1) {
            if (toggleOnOff[i]){
                #if CLIENT_SELECT==CLIENT_LED
                    if (output_isStrip(i)) setBriFromString(i, strip_bri[i]);
                #endif
                strip_selected_onOff(i, true);
                if (relay) {if (output_isRelay(i)) relay_set_on(i);}
            } else  {
                strip_selected_onOff(i, "0.0.0", true);
                if (relay) {if (output_isRelay(i)) relay_set_off(i);}                
            }
        }
        if (state == 0) {
            toggleOnOff[i] = false;
            strip_selected_onOff(i, "0.0.0", false);
            if (relay) {if (output_isRelay(i)) relay_set_off(i); }
            #if CLIENT_SELECT==CLIENT_LED
            led_show_force(i);    
            #endif        
        }
    } 

    String aVal = value_sav_toStringt(false);
    value_savToSPIFF(false, "/" + FILENAME_SAVVALUE + ".txt", aVal); 

    // debug(&disp);
}