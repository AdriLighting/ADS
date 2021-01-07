
#include "config_base.h"

#ifndef DEVICES_H
	#define DEVICES_H
    #include <arduino.h>
    #include <ESP8266WiFi.h>
    #include "devices_mod.h"
    #include "device_def.h"

    #define FILENAME_SAVVALUE String("savValue")

    
    String  device_display(int mod);

    #if LED_LIB==FASTLED
        #include <FastLED.h>
        extern CRGB leds_s1[];
        extern CRGB leds_s2[];
        extern CRGB leds_s3[];
        extern CRGB leds_s4[];
        extern CRGB leds_s5[];
        extern CRGB leds_s6[];
        extern CRGB* Strip_array[];

    #endif
    #if LED_LIB==NEOPIX
        #include <Adafruit_NeoPixel.h>
        extern Adafruit_NeoPixel strip_1;
        extern Adafruit_NeoPixel strip_2;
        extern Adafruit_NeoPixel strip_3;
        extern Adafruit_NeoPixel strip_4;
        extern Adafruit_NeoPixel strip_5;
        extern Adafruit_NeoPixel strip_6;
    #endif


    extern String       device_name;
    extern IPAddress    device_ip;

    extern int                 output_count;
    extern int                 output_pin[];
    extern mod_outputs_type    output_type[];

    extern mod_devices_room    device_room[];
    extern mod_devices_grp     output_grp[];
    extern String              output_location[];

    extern boolean toggleOnOff[]; 
    extern int     output_state[]; 

    extern int     strip_size[]; 
    extern String  strip_color[];
    extern String  strip_colorOff[];
    extern uint8_t strip_hue[];
    extern uint8_t strip_sat[];    
    extern uint8_t strip_bri[];
    extern uint8_t strip_hBri[];
    extern boolean trueWhite[];

    extern int     output_selected[];
    extern int     patternStrip_selected[];
    // #ifndef DDSERVER



    struct device_values {
        char    * id;
        int     position;
        int     type;
        int     load;
        String  (* getValue ) ();
        void    (* setValue ) (String value);
        void    (* set ) (String value);
    } ;

    String t_cd(int mod);
    String send_device();
    String send_rep();
    
    String get_output_info(String buffer, mod_outputs_type *pos, int count);
    String get_output_info(String buffer, mod_devices_room *pos, int count);
    String get_output_info(String buffer, mod_devices_grp *pos, int count);
    String get_output_info(String buffer, String *pos, int count);
    String get_output_info(String buffer, boolean *pos, int count);
    String get_output_info(String buffer, int *pos, int count);
    String get_output_info(String buffer, uint8_t *pos, int count);




    void set_output_bool(String s, boolean *pos);
    void set_output_int8(String s, uint8_t *pos);
    void set_output_int(String s, int *pos);
    void set_output_string(String s, String *pos);
    void set_output_type(String s, mod_outputs_type *pos);
    void set_output_room(String s, mod_devices_room *pos);
    void set_output_devices_grp(String s, mod_devices_grp *pos);

    String value_sav_toStringt(boolean Debug);
    boolean value_savToSPIFF(boolean Debug, String filename, String temp);
    String value_loadFromSPIFF(boolean Debug, String filename);
    String value_load_fromString(boolean Debug, String str);
#endif

