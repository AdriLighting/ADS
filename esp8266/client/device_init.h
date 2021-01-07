// #include <arduino.h>
// #ifdef ESP8266
//    #include <ESP8266WiFi.h>
//     #include <ESP8266mDNS.h>
// #endif 
// #include "devices_mod.h"

#if DEVICE_SELECT==BUREAU_ECRAN
    String      device_name  = DEVICENAME;
    IPAddress   device_ip    = {192, 168, 0, 21};

    int                 output_count    = STRIP_COUNT;//
    mod_outputs_type    output_type[]   = { OT_RGBSTRIP, OT_RGBSTRIP, OT_RGBSTRIP, OT_RGBSTRIP, OT_RGBSTRIP, OT_RGBSTRIP};
    int                 output_pin[]        = { LED_PIN_S1, LED_PIN_S2, LED_PIN_S3, LED_PIN_S4, LED_PIN_S5, LED_PIN_S6};
    mod_devices_room    device_room[]       = { DR_CH,      DR_SALON,      DR_CH,      DR_CH,      DR_CH,      DR_CH};
    mod_devices_grp     output_grp[]        = { DG_ECARN,   DG_ECARN,   DG_BUREAU,  DG_ECARN,   DG_BUREAU,  DG_BUREAU};
    String              output_location[]   = { "right",    "center",  "center",   "left",      "left",     "right"};
    uint8_t             strip_bri[]         = { 60,         60,         60,         60,         60,         60};
    int                 strip_size[]        = { NUMS_LED_S1,NUMS_LED_S2,NUMS_LED_S3,NUMS_LED_S4,NUMS_LED_S5,NUMS_LED_S6}; 

    boolean             toggleOnOff[]   = {true, true, true, true, true, true}; 
    int                 output_state[]  = {0, 0, 0, 0, 0, 0}; 
    boolean             trueWhite[]     = {false, false, false, false, false, false}; 

    uint8_t strip_hue[]     = {0, 0, 0, 0, 0, 0}; ;
    uint8_t strip_sat[]     = {255, 255, 255, 255, 255, 255};
    uint8_t strip_hBri[]    = {255, 255, 255, 255, 255, 255};
    // uint8_t strip_val[output_count]
    String  strip_color[]   = {"0.0.255", "255.0.0", "0.0.255", "0.255.0", "0.0.255", "0.255.0"};
    String  strip_colorOff[]= {"0.0.0", "0.0.0", "0.0.0", "0.0.0", "0.0.0", "0.0.0"};;

    int     output_selected[]       = {1, 1, 1, 1, 1, 1};
    int     patternStrip_selected[] = {1, 1, 1, 1, 1, 1};

    String device_lib = DEVICE_LIB;
#endif

#if DEVICE_SELECT==BUREAU
    String      device_name  = DEVICENAME;
    IPAddress   device_ip    = {192, 168, 0, 21};

    int                 output_count    = STRIP_COUNT;//
    mod_outputs_type    output_type[]   = { OT_RGBWSTRIP};
    int                 output_pin[]    = { LED_PIN_S1};
    mod_devices_room    device_room[]       = { DR_CH};
    mod_devices_grp     output_grp[]        = { DG_BUREAU};
    String              output_location[]   = { "top"};
    uint8_t             strip_bri[]         = { 60};
    int                 strip_size[]        = { NUMS_LED_S1}; 

    boolean             toggleOnOff[]   = {true}; 
    int                 output_state[]  = {0}; 
    boolean             trueWhite[]     = {false}; 

    uint8_t strip_hue[]     = {0}; ;
    uint8_t strip_sat[]     = {255};
    uint8_t strip_hBri[]    = {255};

    String  strip_color[]   = {"0.0.255"};
    String  strip_colorOff[]= {"0.0.0"};;

    int     output_selected[]       = {1};
    int     patternStrip_selected[] = {0};

    String device_lib = DEVICE_LIB;
#endif

#if DEVICE_SELECT==TDN
    String      device_name  = DEVICENAME;
    IPAddress   device_ip    = {192, 168, 0, 21};

    int                 output_count    = STRIP_COUNT;//
    mod_outputs_type    output_type[]   = { OT_RGBSTRIP, OT_RGBSTRIP};
    int                 output_pin[]    = { LED_PIN_S1, LED_PIN_S2};
    mod_devices_room    device_room[]       = { DR_CH, DR_CH};
    mod_devices_grp     output_grp[]        = { DG_LIT, DG_LIT};
    String              output_location[]   = { "nightstand_bot" , "nightstand_top"};
    uint8_t             strip_bri[]         = { 60, 60};
    int                 strip_size[]        = { NUMS_LED_S1, NUMS_LED_S2}; 

    boolean             toggleOnOff[]   = {true, true}; 
    int                 output_state[]  = {0, 0}; 
    boolean             trueWhite[]     = {false, false};

    uint8_t strip_hue[]     = {0, 0}; ;
    uint8_t strip_sat[]     = {255, 255};
    uint8_t strip_hBri[]    = {255, 255};

    String  strip_color[]   = {"0.0.255", "0.0.255"};
    String  strip_colorOff[]= {"0.0.0", "0.0.255"};;

    int     output_selected[]       = {1, 1};
    int     patternStrip_selected[] = {0, 0};

    String device_lib = DEVICE_LIB;
#endif


#if DEVICE_SELECT==PLAFOND_LIT
    String      device_name  = DEVICENAME;
    IPAddress   device_ip    = {192, 168, 0, 21};

    int                 output_count    = STRIP_COUNT;//
    mod_outputs_type    output_type[]   = { OT_RGBWSTRIP};
    int                 output_pin[]    = { LED_PIN_S1};
    mod_devices_room    device_room[]       = { DR_CH};
    mod_devices_grp     output_grp[]        = { DG_CENTRE};
    String              output_location[]   = { "celling_2"};
    uint8_t             strip_bri[]         = { 60};
    int                 strip_size[]        = { NUMS_LED_S1}; 

    boolean             toggleOnOff[]   = {true}; 
    int                 output_state[]  = {0}; 
    boolean             trueWhite[]     = {false}; 

    uint8_t strip_hue[]     = {0}; ;
    uint8_t strip_sat[]     = {255};
    uint8_t strip_hBri[]    = {255};

    String  strip_color[]   = {"0.0.255"};
    String  strip_colorOff[]= {"0.0.0"};;

    int     output_selected[]       = {1};
    int     patternStrip_selected[] = {0};

    String device_lib = DEVICE_LIB;
#endif   

#if DEVICE_SELECT==PLAFOND_ENTER
    String      device_name  = DEVICENAME;
    IPAddress   device_ip    = {192, 168, 0, 21};

    int                 output_count    = STRIP_COUNT;//
    mod_outputs_type    output_type[]   = { OT_RGBWSTRIP};
    int                 output_pin[]    = { LED_PIN_S1};
    mod_devices_room    device_room[]       = { DR_CH};
    mod_devices_grp     output_grp[]        = { DG_CENTRE};
    String              output_location[]   = { "celling_e"};
    uint8_t             strip_bri[]         = { 60};
    int                 strip_size[]        = { NUMS_LED_S1}; 

    boolean             toggleOnOff[]   = {true}; 
    int                 output_state[]  = {0}; 
    boolean             trueWhite[]     = {false}; 

    uint8_t strip_hue[]     = {0}; ;
    uint8_t strip_sat[]     = {255};
    uint8_t strip_hBri[]    = {255};

    String  strip_color[]   = {"0.0.255"};
    String  strip_colorOff[]= {"0.0.0"};;

    int     output_selected[]       = {1};
    int     patternStrip_selected[] = {0};

    String device_lib = DEVICE_LIB;
#endif    

#if DEVICE_SELECT==PLAFOND_RELAY
    String      device_name  = DEVICENAME;
    IPAddress   device_ip    = {192, 168, 0, 21};

    int                 output_count    = STRIP_COUNT;//
    mod_outputs_type    output_type[]   = {OT_RELAY_AC};
    int                 output_pin[]    = {RELAY_PIN_S1};
    mod_devices_room    device_room[]       = { DR_CH};
    mod_devices_grp     output_grp[]        = { DG_CENTRE};
    String              output_location[]   = { "ceiling"};
    uint8_t             strip_bri[]         = { 0};
    int                 strip_size[]        = { 0}; 

    boolean             toggleOnOff[]   = {true}; 
    int                 output_state[]  = {0}; 
    boolean             trueWhite[]     = {false};

    uint8_t strip_hue[]     = {0}; ;
    uint8_t strip_sat[]     = {0};
    uint8_t strip_hBri[]    = {0};

    String  strip_color[]   = {"0.0.0"};
    String  strip_colorOff[]= {"0.0.0"};;

    int     output_selected[]       = {1};
    int     patternStrip_selected[] = {0};

    String device_lib = DEVICE_LIB;
#endif


#if DEVICE_SELECT==FLEUR
    String      device_name  = DEVICENAME;
    IPAddress   device_ip    = {192, 168, 0, 21};

    int                 output_count    = STRIP_COUNT;//
    mod_outputs_type    output_type[]   = { OT_RGBWSTRIP, OT_RGBSTRIP};
    int                 output_pin[]    = { LED_PIN_S1, LED_PIN_S2};
    mod_devices_room    device_room[]       = { DR_CH, DR_CH};
    mod_devices_grp     output_grp[]        = { DG_CENTRE, DG_CENTRE};
    String              output_location[]   = { "fleur" , "coffee_stand"};
    uint8_t             strip_bri[]         = { 60, 60};
    int                 strip_size[]        = { NUMS_LED_S1, NUMS_LED_S2}; 

    boolean             toggleOnOff[]   = {true, true}; 
    int                 output_state[]  = {0, 0}; 
    boolean             trueWhite[]     = {false, false};

    uint8_t strip_hue[]     = {0, 0}; ;
    uint8_t strip_sat[]     = {255, 255};
    uint8_t strip_hBri[]    = {255, 255};

    String  strip_color[]   = {"0.0.255", "0.0.255"};
    String  strip_colorOff[]= {"0.0.0", "0.0.255"};;

    int     output_selected[]       = {1, 1};
    int     patternStrip_selected[] = {0, 0};

    String device_lib = DEVICE_LIB;
#endif

#if DEVICE_SELECT==AMPLI
    String      device_name  = DEVICENAME;
    IPAddress   device_ip    = {192, 168, 0, 21};

    int                 output_count    = STRIP_COUNT;//
    mod_outputs_type    output_type[]   = { OT_RGBWSTRIP};
    int                 output_pin[]    = { LED_PIN_S1};
    mod_devices_room    device_room[]       = { DR_CH};
    mod_devices_grp     output_grp[]        = { DG_BUREAU};
    String              output_location[]   = { "ampli"};
    uint8_t             strip_bri[]         = { 60};
    int                 strip_size[]        = { NUMS_LED_S1}; 

    boolean             toggleOnOff[]   = {true}; 
    int                 output_state[]  = {0}; 
    boolean             trueWhite[]     = {false}; 

    uint8_t strip_hue[]     = {0}; ;
    uint8_t strip_sat[]     = {255};
    uint8_t strip_hBri[]    = {255};

    String  strip_color[]   = {"0.0.255"};
    String  strip_colorOff[]= {"0.0.0"};;

    int     output_selected[]       = {1};
    int     patternStrip_selected[] = {0};

    String device_lib = DEVICE_LIB;
#endif

#if DEVICE_SELECT==SUSPENDU
    String      device_name  = DEVICENAME;
    IPAddress   device_ip    = {192, 168, 0, 21};

    int                 output_count    = STRIP_COUNT;//
    mod_outputs_type    output_type[]   = { OT_RGBSTRIP};
    int                 output_pin[]    = { LED_PIN_S1};
    mod_devices_room    device_room[]       = { DR_CH};
    mod_devices_grp     output_grp[]        = { DG_CENTRE};
    String              output_location[]   = { "suspendu"};
    uint8_t             strip_bri[]         = { 60};
    int                 strip_size[]        = { NUMS_LED_S1}; 

    boolean             toggleOnOff[]   = {true}; 
    int                 output_state[]  = {0}; 
    boolean             trueWhite[]     = {false}; 

    uint8_t strip_hue[]     = {0}; ;
    uint8_t strip_sat[]     = {255};
    uint8_t strip_hBri[]    = {255};

    String  strip_color[]   = {"0.0.255"};
    String  strip_colorOff[]= {"0.0.0"};;

    int     output_selected[]       = {1};
    int     patternStrip_selected[] = {0};

    String device_lib = DEVICE_LIB;
#endif


#if DEVICE_SELECT==ENTER_RIGHT
    String      device_name  = DEVICENAME;
    IPAddress   device_ip    = {192, 168, 0, 21};

    int                 output_count    = STRIP_COUNT;//
    mod_outputs_type    output_type[]   = { OT_RGBSTRIP, OT_RGBSTRIP};
    int                 output_pin[]    = { LED_PIN_S1, LED_PIN_S2};
    mod_devices_room    device_room[]       = { DR_CH, DR_CH};
    mod_devices_grp     output_grp[]        = { DG_CENTRE, DG_CENTRE};
    String              output_location[]   = { "enter_right", "enter_table"};
    uint8_t             strip_bri[]         = { 60, 60};
    int                 strip_size[]        = { NUMS_LED_S1, NUMS_LED_S2}; 

    boolean             toggleOnOff[]   = {true, true}; 
    int                 output_state[]  = {0, 0}; 
    boolean             trueWhite[]     = {false, false}; 

    uint8_t strip_hue[]     = {0, 0}; ;
    uint8_t strip_sat[]     = {255, 255};
    uint8_t strip_hBri[]    = {255, 255};

    String  strip_color[]   = {"0.0.255", "0.0.255"};
    String  strip_colorOff[]= {"0.0.0", "0.0.0"};

    int     output_selected[]       = {1, 1};
    int     patternStrip_selected[] = {0, 0};

    String device_lib = DEVICE_LIB;
#endif

#if DEVICE_SELECT==DOUBLEAPPLIQUE
    String      device_name  = DEVICENAME;
    IPAddress   device_ip    = {192, 168, 0, 21};

    int                 output_count    = STRIP_COUNT;//
    mod_outputs_type    output_type[]   = { OT_RGBSTRIP, OT_RGBSTRIP};
    int                 output_pin[]    = { LED_PIN_S1, LED_PIN_S2};
    mod_devices_room    device_room[]       = { DR_CH, DR_CH};
    mod_devices_grp     output_grp[]        = { DG_CENTRE, DG_CENTRE};
    String              output_location[]   = { "enter_left_bot" , "enter_left_top"};
    uint8_t             strip_bri[]         = { 60, 60};
    int                 strip_size[]        = { NUMS_LED_S1, NUMS_LED_S2}; 

    boolean             toggleOnOff[]   = {true, true}; 
    int                 output_state[]  = {0, 0}; 
    boolean             trueWhite[]     = {false, false};

    uint8_t strip_hue[]     = {0, 0}; ;
    uint8_t strip_sat[]     = {255, 255};
    uint8_t strip_hBri[]    = {255, 255};

    String  strip_color[]   = {"0.0.255", "0.0.255"};
    String  strip_colorOff[]= {"0.0.0", "0.0.255"};;

    int     output_selected[]       = {1, 1};
    int     patternStrip_selected[] = {0, 0};

    String device_lib = DEVICE_LIB;
#endif

#if DEVICE_SELECT==SALON_CANAPE_RELAY
    String      device_name  = DEVICENAME;
    IPAddress   device_ip    = {192, 168, 0, 21};

    int                 output_count    = STRIP_COUNT;//
    mod_outputs_type    output_type[]   = {OT_RELAY_AC};
    int                 output_pin[]    = {RELAY_PIN_S1};
    mod_devices_room    device_room[]       = { DR_CH};
    mod_devices_grp     output_grp[]        = { DG_CENTRE};
    String              output_location[]   = { "canape"};
    uint8_t             strip_bri[]         = { 0};
    int                 strip_size[]        = { 0}; 

    boolean             toggleOnOff[]   = {true}; 
    int                 output_state[]  = {0}; 
    boolean             trueWhite[]     = {false};

    uint8_t strip_hue[]     = {0}; ;
    uint8_t strip_sat[]     = {0};
    uint8_t strip_hBri[]    = {0};

    String  strip_color[]   = {"0.0.0"};
    String  strip_colorOff[]= {"0.0.0"};;

    int     output_selected[]       = {1};
    int     patternStrip_selected[] = {0};

    String device_lib = DEVICE_LIB;
#endif

#if LED_LIB==FASTLED
    #include <FastLED.h>

    #if DEVICE_SELECT==BUREAU_ECRAN
    CRGB leds_s1[NUMS_LED_S1];
    CRGB leds_s2[NUMS_LED_S2];
    CRGB leds_s3[NUMS_LED_S3];
    CRGB leds_s4[NUMS_LED_S4];
    CRGB leds_s5[NUMS_LED_S5];
    CRGB leds_s6[NUMS_LED_S6];
    CRGB* Strip_array[STRIP_COUNT] = {leds_s1, leds_s2, leds_s3, leds_s4, leds_s5, leds_s6};
    #endif
    #if DEVICE_SELECT==BUREAU
    CRGB leds_s1[NUMS_LED_S1];
    CRGB* Strip_array[STRIP_COUNT] = {leds_s1};
    #endif
    #if DEVICE_SELECT==AMPLI
    CRGB leds_s1[NUMS_LED_S1];
    CRGB* Strip_array[STRIP_COUNT] = {leds_s1};
    #endif    
#endif

    
#if LED_LIB==NEOPIX
    #include <Adafruit_NeoPixel.h>
    #ifdef __AVR__
        #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
    #endif

    #ifdef LED_PIN_S1
        
        #ifdef LED_PIN_S1_RGBW
            Adafruit_NeoPixel strip_1(NUMS_LED_S1, LED_PIN_S1, NEO_GRBW + NEO_KHZ800);
        #else 
             Adafruit_NeoPixel strip_1(NUMS_LED_S1, LED_PIN_S1, NEO_GRB + NEO_KHZ800);
        #endif
       
    #endif
    #ifdef LED_PIN_S2
        
        #ifdef LED_PIN_S2_RGBW
            Adafruit_NeoPixel strip_2(NUMS_LED_S2, LED_PIN_S2, NEO_GRBW + NEO_KHZ800);
        #else 
             Adafruit_NeoPixel strip_2(NUMS_LED_S2, LED_PIN_S2, NEO_GRB + NEO_KHZ800);
        #endif        
    #endif
    #ifdef LED_PIN_S3
        Adafruit_NeoPixel strip_3(NUMS_LED_S3, LED_PIN_S3, NEO_GRB + NEO_KHZ800);
    #endif
    #ifdef LED_PIN_S4
        Adafruit_NeoPixel strip_4(NUMS_LED_S4, LED_PIN_S4, NEO_GRB + NEO_KHZ800);
    #endif
    #ifdef LED_PIN_S5
        Adafruit_NeoPixel strip_5(NUMS_LED_S5, LED_PIN_S5, NEO_GRB + NEO_KHZ800);
    #endif
    #ifdef LED_PIN_S6
        Adafruit_NeoPixel strip_6(NUMS_LED_S6, LED_PIN_S6, NEO_GRB + NEO_KHZ800);
    #endif                            
#endif