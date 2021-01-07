#include "config_base.h"
#include "device_def.h"

#if CLIENT_SELECT==CLIENT_LED

#include "fastled_func.h"
#include "output_func.h"
#include "colors.h"
#include "tools.h"

#if LED_LIB==LEDLIB_NO

void strip_setup(){
         
}

#endif

#if LED_LIB==FASTLED

void strip_setup(){

    #ifdef LED_PIN_S1
        FastLED.addLeds<CHIPSET, LED_PIN_S1, COLOR_ORDER>(leds_s1, NUMS_LED_S1);
    #endif
    #ifdef LED_PIN_S2
        FastLED.addLeds<CHIPSET, LED_PIN_S2, COLOR_ORDER>(leds_s2, NUMS_LED_S2);
    #endif
    #ifdef LED_PIN_S3
        FastLED.addLeds<CHIPSET, LED_PIN_S3, COLOR_ORDER>(leds_s3, NUMS_LED_S3);
    #endif
    #ifdef LED_PIN_S4
        FastLED.addLeds<CHIPSET, LED_PIN_S4, COLOR_ORDER>(leds_s4, NUMS_LED_S4);
    #endif
    #ifdef LED_PIN_S5
        FastLED.addLeds<CHIPSET, LED_PIN_S5, COLOR_ORDER>(leds_s5, NUMS_LED_S5);
    #endif
    #ifdef LED_PIN_S6
        FastLED.addLeds<CHIPSET, LED_PIN_S6, COLOR_ORDER>(leds_s6, NUMS_LED_S6);
    #endif 
    FastLED.clear();
    led_show();
}

void led_show() {
   for( int i = 0; i < output_count; i++) { 
      if ((output_selected[i] == 1) && (output_isStrip(i))) {
         FastLED[i].showLeds(  strip_bri[i] )   ;    
      }
   }     
}
void led_show_force(int pos) {
      if (output_isStrip(pos)) {
         FastLED[pos].showLeds(  strip_bri[pos] )   ; 
     }    
}

void led_showPattern(){
   for( int i = 0; i < output_count; i++) { 
      if ((patternStrip_selected[i] == 1) && (output_isStrip(i))) {
         FastLED[i].showLeds(  strip_bri[i] )   ;    
      }
   }     
}

void FastLED_show(int nbr, uint8_t val){FastLED[nbr].showLeds(  val );  }

void setPixelColor(CRGB* ledarray, int Pixel, RgbColor c, bool save = true) {
   ledarray[Pixel].r = c.R;
   ledarray[Pixel].g = c.G;
   ledarray[Pixel].b = c.B;
}

void setPixelColor(CRGB* ledarray, int Pixel, CHSV c, bool save = true) {ledarray[Pixel] = c;}

void setPixelColor(CRGB* ledarray, int Pixel, CRGB c, bool save = true) {
   ledarray[Pixel].r = c.r;
   ledarray[Pixel].g = c.g;
   ledarray[Pixel].b = c.b;
}

void setPixelColor_ByStrip(CRGB* ledarray, int numleds, RgbColor c, bool show) {
   for(uint8_t i=0; i<numleds; i++) { 
      setPixelColor(ledarray, i, c);
   }
   if (show) led_show();
}

void setPixelColor_ByStrip(CRGB* ledarray, int numleds, CHSV c_chsv, bool show) {
   for(uint8_t i=0; i<numleds; i++) { 
      ledarray[i] = c_chsv;     
   }
   if (show) led_show();
}

void setPixelColor_ByStrip( CRGB* ledarray, uint8_t numleds, CRGB c, bool show) {
   for(uint8_t i=0; i<numleds; i++) { 
      ledarray[i] = c;       
   }
   if (show) led_show();
}

void fill_solid_ByStrip(CRGB* ledarray, int numleds, CRGB c, bool show) {
   fill_solid(ledarray, numleds, c); 
   if (show) led_show();
}

void setColor(CRGB c) {
   for( int i = 0; i < output_count; i++) { 
         if (output_selected[i] == 1) {
         patternStrip_selected[i] = 0;
         setPixelColor_ByStrip(Strip_array[i], strip_size[i], c, false);
         strip_color[i] = String(c.r) + "." + String(c.g) + "." + String(c.b);
      }
   }
   led_show();
}

#endif

#if LED_LIB==NEOPIX

boolean led_driver_debug = false;

void strip_setup(){
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
        clock_prescale_set(clock_div_1);
    #endif
    #ifdef LED_PIN_S1
        strip_1.begin();
        strip_1.setBrightness(50);
    #endif
    #ifdef LED_PIN_S2
        strip_2.begin();
        strip_2.setBrightness(50);
    #endif
    #ifdef LED_PIN_S3
        strip_3.begin();
        strip_3.setBrightness(50);
    #endif
    #ifdef LED_PIN_S4
        strip_4.begin();
        strip_4.setBrightness(50);
    #endif
    #ifdef LED_PIN_S5
        strip_5.begin();
        strip_5.setBrightness(50);
    #endif
    #ifdef LED_PIN_S6
        strip_6.begin();
        strip_6.setBrightness(50);
    #endif 
    led_set_red() ;           
}

void led_show() {
        #ifdef LED_PIN_S1
            if (output_selected[0] == 1) strip_1.show();
        #endif
        #ifdef LED_PIN_S2
            if (output_selected[1] == 1) strip_2.show();
        #endif
        #ifdef LED_PIN_S3
            if (output_selected[2] == 1) strip_3.show();
        #endif
        #ifdef LED_PIN_S4
            if (output_selected[3] == 1) strip_4.show();
        #endif
        #ifdef LED_PIN_S5
            if (output_selected[4] == 1) strip_5.show();
        #endif
        #ifdef LED_PIN_S6
            if (output_selected[5] == 1) strip_6.show();
        #endif  
}

void led_show(int pos) {
        #ifdef LED_PIN_S1
            if (output_selected[0] == 1) {if (pos == 0) strip_1.show();}
        #endif
        #ifdef LED_PIN_S2
            if (output_selected[1] == 1) {if (pos == 1) strip_2.show();}
        #endif
        #ifdef LED_PIN_S3
            if (output_selected[2] == 1) {if (pos == 2) strip_3.show();}
        #endif
        #ifdef LED_PIN_S4
            if (output_selected[3] == 1) {if (pos == 3) strip_4.show();}
        #endif
        #ifdef LED_PIN_S5
            if (output_selected[4] == 1) {if (pos == 4) strip_5.show();}
        #endif
        #ifdef LED_PIN_S6
            if (output_selected[5] == 1) {if (pos == 5) strip_6.show();}
        #endif  
}
void led_show_force(int pos)  {
        #ifdef LED_PIN_S1
            if (pos == 0) strip_1.show();
        #endif
        #ifdef LED_PIN_S2
            if (pos == 1) strip_2.show();
        #endif
        #ifdef LED_PIN_S3
            if (pos == 2) strip_3.show();
        #endif
        #ifdef LED_PIN_S4
            if (pos == 3) strip_4.show();
        #endif
        #ifdef LED_PIN_S5
            if (pos == 4) strip_5.show();
        #endif
        #ifdef LED_PIN_S6
            if (pos == 5) strip_6.show();
        #endif  
}

void setBri_ByStrip(int pos,  uint8_t v, boolean show) {
        #ifdef LED_PIN_S1
            if (pos == 0) {
                strip_1.setBrightness(v);
            }
        #endif
        #ifdef LED_PIN_S2
            if (pos == 1) {
                strip_2.setBrightness(v);
            }
        #endif
        #ifdef LED_PIN_S3
            if (pos == 2) {
                strip_3.setBrightness(v);
            }
        #endif
        #ifdef LED_PIN_S4
            if (pos == 3) {
                strip_4.setBrightness(v);
            }
        #endif
        #ifdef LED_PIN_S5
            if (pos == 4) {
                strip_5.setBrightness(v);
            }
        #endif
        #ifdef LED_PIN_S6
            if (pos == 5) {
                strip_6.setBrightness(v);
            }
        #endif     
   if (show) led_show(pos);
}

void setBri_allSelectedStrip( uint8_t v, boolean show ) {
        #ifdef LED_PIN_S1
            if (output_selected[0] == 1) setBri_ByStrip(0,  v, show);
        #endif
        #ifdef LED_PIN_S2
            if (output_selected[1] == 1) setBri_ByStrip(0,  v, show);
        #endif
        #ifdef LED_PIN_S3
            if (output_selected[2] == 1) setBri_ByStrip(0,  v, show);
        #endif
        #ifdef LED_PIN_S4
            if (output_selected[3] == 1) setBri_ByStrip(0,  v, show);
        #endif
        #ifdef LED_PIN_S5
            if (output_selected[4] == 1) setBri_ByStrip(0,  v, show);
        #endif
        #ifdef LED_PIN_S6
            if (output_selected[5] == 1) setBri_ByStrip(0,  v, show);
        #endif  
}

void setPixelColor_ByStrip(int numleds, RgbColor c, bool show) {
   // for(uint8_t i=0; i<numleds; i++) { 
   //    setPixelColor(ledarray, i, c);
   // }
   // if (show) led_show();
}

void setPixelColor_ByStrip(int pos, int numleds, uint8_t h, uint8_t s, uint8_t v, bool show) {
        #ifdef LED_PIN_S1
            if (pos == 0) {
                for(uint8_t i=0; i<numleds; i++) strip_1.setPixelColor(i, strip_1.gamma32(strip_1.ColorHSV( h * 65536L / 255 , s,v)));}
        #endif
        #ifdef LED_PIN_S2
            if (pos == 1) {
                for(uint8_t i=0; i<numleds; i++) strip_2.setPixelColor(i, strip_2.gamma32(strip_2.ColorHSV( h * 65536L / 255 , s,v)));}
        #endif
        #ifdef LED_PIN_S3
            if (pos == 2) {
                for(uint8_t i=0; i<numleds; i++) strip_3.setPixelColor(i, strip_3.gamma32(strip_3.ColorHSV( h * 65536L / 255 , s,v)));}
        #endif
        #ifdef LED_PIN_S4
            if (pos == 3) {
                for(uint8_t i=0; i<numleds; i++) strip_4.setPixelColor(i, strip_4.gamma32(strip_4.ColorHSV( h * 65536L / 255 , s,v)));}
        #endif
        #ifdef LED_PIN_S5
            if (pos == 4) {
                for(uint8_t i=0; i<numleds; i++) strip_5.setPixelColor(i, strip_5.gamma32(strip_5.ColorHSV( h * 65536L / 255 , s,v)));}
        #endif
        #ifdef LED_PIN_S6
            if (pos == 5) {
                for(uint8_t i=0; i<numleds; i++) strip_6.setPixelColor(i, strip_6.gamma32(strip_6.ColorHSV( h * 65536L / 255 , s,v)));}
        #endif     
   if (show) led_show();
}

void setPixelColor_ByStrip(int pos, int numleds, uint8_t r, uint8_t g, uint8_t b,  uint8_t w, bool show) {
        #ifdef LED_PIN_S1
            if (pos == 0) {
                for(uint8_t i=0; i<numleds; i++) strip_1.setPixelColor(i, r, g, b);}
        #endif
        #ifdef LED_PIN_S2
            if (pos == 1) {
                for(uint8_t i=0; i<numleds; i++) strip_2.setPixelColor(i, r, g, b);}
        #endif
        #ifdef LED_PIN_S3
            if (pos == 2) {
                for(uint8_t i=0; i<numleds; i++) strip_3.setPixelColor(i, r, g, b);}
        #endif
        #ifdef LED_PIN_S4
            if (pos == 3) {
                for(uint8_t i=0; i<numleds; i++)strip_4.setPixelColor(i, r, g, b);}
        #endif
        #ifdef LED_PIN_S5
            if (pos == 4) {
                for(uint8_t i=0; i<numleds; i++)strip_5.setPixelColor(i, r, g, b);}
        #endif
        #ifdef LED_PIN_S6
            if (pos == 5) {
                for(uint8_t i=0; i<numleds; i++) strip_6.setPixelColor(i, r, g, b);}
        #endif              
   if (show) led_show();
}

void setPixelColor_ByStrip(int pos, int numleds, uint32_t c, bool show) {
        #ifdef LED_PIN_S1
            if (pos == 0) {
                for(uint8_t i=0; i<numleds; i++) strip_1.setPixelColor(i, c);}
        #endif
        #ifdef LED_PIN_S2
            if (pos == 1) {
                for(uint8_t i=0; i<numleds; i++) strip_2.setPixelColor(i, c);}
        #endif
        #ifdef LED_PIN_S3
            if (pos == 2) {
                for(uint8_t i=0; i<numleds; i++) strip_3.setPixelColor(i, c);}
        #endif
        #ifdef LED_PIN_S4
            if (pos == 3) {
                for(uint8_t i=0; i<numleds; i++)strip_4.setPixelColor(i, c);}
        #endif
        #ifdef LED_PIN_S5
            if (pos == 4) {
                for(uint8_t i=0; i<numleds; i++)strip_5.setPixelColor(i, c);}
        #endif
        #ifdef LED_PIN_S6
            if (pos == 5) {
                for(uint8_t i=0; i<numleds; i++) strip_6.setPixelColor(i, c);}
        #endif        
   if (show) led_show();
}

void setPixelWhite_ByStrip(int pos, uint8_t w, bool show) {
        #ifdef LED_PIN_S1
            if (pos == 0) {
                strip_1.fill(strip_1.Color(0, 0, 0, strip_1.gamma8(w)));}
        #endif
        #ifdef LED_PIN_S2
            if (pos == 1) {
                strip_2.fill(strip_2.Color(0, 0, 0, strip_2.gamma8(w)));}
        #endif
        #ifdef LED_PIN_S3
            if (pos == 2) {
                strip_3.fill(strip_3.Color(0, 0, 0, strip_3.gamma8(w)));}
        #endif
        #ifdef LED_PIN_S4
            if (pos == 3) {
                strip_4.fill(strip_4.Color(0, 0, 0, strip_4.gamma8(w)));}
        #endif
        #ifdef LED_PIN_S5
            if (pos == 4) {
                strip_5.fill(strip_5.Color(0, 0, 0, strip_5.gamma8(w)));}
        #endif
        #ifdef LED_PIN_S6
            if (pos == 5) {
                strip_6.fill(strip_6.Color(0, 0, 0, strip_6.gamma8(w)));}
        #endif              
   if (show) led_show();
}

void led_set_red() {setColor(0, 0, 0, 'r') ;}

void setColor(uint32_t c) {
    for( int i = 0; i < output_count; i++) { 
        if (output_selected[i] == 1) {
            patternStrip_selected[i] = 0;
            setPixelColor_ByStrip(i, strip_size[i], c, false);
            // strip_color[i] = String(c.r) + "." + String(c.g) + "." + String(c.b);
        }
    }
    led_show();
}
void setHsvColor(uint8_t h, uint8_t s, uint8_t v) {
    for( int i = 0; i < output_count; i++) { 
        if (output_selected[i] == 1) {
            patternStrip_selected[i] = 0;
            setPixelColor_ByStrip(i, strip_size[i], h, s, v, false);
            // strip_color[i] = String(c.r) + "." + String(c.g) + "." + String(c.b);
        }
    }
    led_show();
}

void setRgbColor(uint8_t r, uint8_t g, uint8_t b) {
    for( int i = 0; i < output_count; i++) { 
        if (output_selected[i] == 1) {
            patternStrip_selected[i] = 0;
            setPixelColor_ByStrip(i, strip_size[i], r, g, b, 0, false);
            strip_color[i] = String(r) + "." + String(g) + "." + String(b);
        }
    }
    led_show();
}

void setRgbWhite() {
    for( int i = 0; i < output_count; i++) { 
        if (output_selected[i] == 1) {
            patternStrip_selected[i] = 0;
            setPixelWhite_ByStrip(i, strip_hBri[i], false);
            // strip_color[i] = String(r) + "." + String(g) + "." + String(b);
        }
    }
    led_show();
}

void setColor(uint8_t v1, uint8_t v2, uint8_t v3, char mod) {
    RgbColor c = hsv2rgb (v1, v2, v3, 0);
    
    switch (mod) {
        case 'r' :  setRgbColor(v1, v2, v3);    break;
        case 'h' :  setRgbColor(c.R, c.G, c.B); break;
    }

    led_show();
}

#endif

void selectedStrip_setColor(String color){
    #if LED_LIB==FASTLED
        CRGB c = parsecolorEx(color);
        setColor(c);
    #endif
    #if LED_LIB==NEOPIX
        int *c = splitColor(color);
        setColor( c[0], c[1], c[2], 'r');  
    #endif        
}

void setBriFromString(int pos, int val) {
    #if LED_LIB==FASTLED
        FastLED_show(pos, val);
    #endif
    #if LED_LIB==NEOPIX
        setBri_ByStrip(pos,  val, true);
    #endif    
}

void setColorFromString(int pos, boolean show) {
    #if LED_LIB==FASTLED
        CRGB c = parsecolor(strip_color[pos]);
        setPixelColor_ByStrip(Strip_array[pos], strip_size[pos], c,                     show);
        trueWhite[pos] = false;
    #endif
    #if LED_LIB==NEOPIX
        int *c = splitColor(strip_color[pos]);
        setPixelColor_ByStrip(pos,              strip_size[pos], c[0], c[1], c[2], 0,   show);  
        trueWhite[pos] = false;
    #endif    
}
void setColorFromString(String color, int pos, boolean show) {
    #if LED_LIB==FASTLED
        CRGB c = parsecolorEx(color);
        setPixelColor_ByStrip(Strip_array[pos], strip_size[pos], c,                     show);
        trueWhite[pos] = false;
    #endif
    #if LED_LIB==NEOPIX
        int *c = splitColor(color);
        setPixelColor_ByStrip(pos,              strip_size[pos], c[0], c[1], c[2], 0,   show);  
        trueWhite[pos] = false;
    #endif    
}
void setWhite(int pos, uint8_t v, boolean show) {
    #if LED_LIB==FASTLED

    #endif
    #if LED_LIB==NEOPIX
        setPixelWhite_ByStrip(pos, v, show);  
    #endif    
}

boolean command_debug = false;
// #define DEBUG 

int allStrip_hue(uint8_t val){
#ifdef DEBUG
   if (command_debug) fsprintln("[allStrip_hue]");
#endif
    RgbColor hsv;
    for( int i = 0; i < output_count; i++) { 
        if ((output_selected[i] == 1) && (output_isStrip(i))) {
            hsv = hsv2rgb (val, strip_sat[i], strip_hBri[i], 0);
            strip_color[i]  = String(hsv.R) + "." + String(hsv.G) + "." + String(hsv.B);
            strip_hue[i] = val;
            setColorFromString(i, true);
            toggleOnOff[i] = true;
        }
    }    
}
int selectedStrip_hue(String v, boolean select){ 
#ifdef DEBUG
   if (command_debug) fsprintf("\n[selectedStrip_hue] : %s\n", v.c_str());
#endif


    String Exploed[2];

    explode(v, '|', Exploed);

    int strip =  Exploed[0].toInt();

    if (strip > output_count) {
        #ifdef DEBUG
        if (command_debug){
        Serial.printf("[selectedStrip_hue] pos %d > stripcount: %d", 
        strip, 
        output_count);}
        #endif
        return -1;
    }

    if (!output_isStrip(strip)) return -1;
    
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

    #ifdef DEBUG
       if (command_debug) fsprintf("strip: %d - val: %d\n[selectedStrip_hue] done\n", strip, state);
    #endif

    RgbColor hsv = hsv2rgb (state, strip_sat[strip], strip_hBri[strip], 0);
    strip_color[strip]  = String(hsv.R) + "." + String(hsv.G) + "." + String(hsv.B);
    strip_hue[strip] = state;
    setColorFromString(strip, true);
    toggleOnOff[strip] = true;
}

int allStrip_sat(uint8_t val){
#ifdef DEBUG
   if (command_debug) fsprintln("[allStrip_sat]");
#endif
    RgbColor hsv;
    for( int i = 0; i < output_count; i++) { 
        if ((output_selected[i] == 1) && (output_isStrip(i))) {
            hsv = hsv2rgb (strip_hue[i], val, strip_hBri[i], 0);
            strip_color[i]  = String(hsv.R) + "." + String(hsv.G) + "." + String(hsv.B);
            strip_sat[i] = val;
            setColorFromString(i, true);
            toggleOnOff[i] = true;
        }
    }    
}
int selectedStrip_sat(String v, boolean select){ 
#ifdef DEBUG
   if (command_debug) fsprintf("\n[selectedStrip_sat] : %s\n", v.c_str());
#endif


    String Exploed[2];

    explode(v, '|', Exploed);

    int strip =  Exploed[0].toInt();

    if (strip > output_count) {
        #ifdef DEBUG
        if (command_debug){
        Serial.printf("[selectedStrip_sat] pos %d > stripcount: %d", 
        strip, 
        output_count);}
        #endif
        return -1;
    }

    if (!output_isStrip(strip)) return -1;

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
    };

    int state = Exploed[1].toInt();

    #ifdef DEBUG
       if (command_debug) fsprintf("strip: %d - val: %d\n[selectedStrip_sat] done\n", strip, state);
    #endif

    RgbColor hsv = hsv2rgb (strip_hue[strip], state, strip_hBri[strip], 0);
    strip_color[strip]  = String(hsv.R) + "." + String(hsv.G) + "." + String(hsv.B);
    strip_sat[strip] = state;
    setColorFromString(strip, true);
    toggleOnOff[strip] = true;
}

int allStrip_bri(uint8_t val){
#ifdef DEBUG
   if (command_debug) fsprintln("[allStrip_bri]");
#endif

    RgbColor hsv;
    val = map(val, 0, 255, 2, 255);
    for( int i = 0; i < output_count; i++) { 
        if ((output_selected[i] == 1) && (output_isStrip(i))) {
            hsv = hsv2rgb (strip_hue[i], strip_sat[i], val, 0);
            strip_color[i]  = String(hsv.R) + "." + String(hsv.G) + "." + String(hsv.B);
            strip_hBri[i] = val;
            setColorFromString(i, true);
            toggleOnOff[i] = true;
        }
    }    
}
int selectedStrip_bri(String v, boolean select){ 
#ifdef DEBUG
   if (command_debug) fsprintf("\n[selectedStrip_bri] : %s\n", v.c_str());
#endif


    String Exploed[2];

    explode(v, '|', Exploed);

    int strip =  Exploed[0].toInt();

    if (strip > output_count) {
        #ifdef DEBUG
        if (command_debug){
        Serial.printf("[selectedStrip_bri] pos %d > stripcount: %d", 
        strip, 
        output_count);}
        #endif
        return -1;
    }

    if (!output_isStrip(strip)) return -1;

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
    state = map(state, 0, 255, 2, 255);
    RgbColor hsv = hsv2rgb (strip_hue[strip], strip_sat[strip], state, 0);
    strip_color[strip]  = String(hsv.R) + "." + String(hsv.G) + "." + String(hsv.B);
    strip_hBri[strip] = state;
    setColorFromString(strip, true);
    toggleOnOff[strip] = true;
}



int allStrip_setBri(uint8_t val){
#ifdef DEBUG
   if (command_debug) fsprintln("[allStrip_setBri]");
#endif
    for( int i = 0; i < output_count; i++) { 
        if ((output_selected[i] == 1) && (output_isStrip(i))) {
           strip_bri[i] = val;      
           setBriFromString(i, val);
        }
    }    
}
int selectedStrip_setBri(String v, boolean select){
#ifdef DEBUG
   if (command_debug) fsprintf("\n[selectedStrip_setBri] : %s\n", v.c_str());
#endif

    String a[2];
    explode(v, '|', a);
    int strip =  a[0].toInt();
    if (strip > output_count) {
        Serial.printf("[selectedStrip_setBri] pos %d > stripcount: %d", 
        strip, 
        output_count);
        return -1;
    }

    if (!output_isStrip(strip)) return -1;

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
    int val =  a[1].toInt();
    strip_bri[strip] = val;      
    setBriFromString(strip, val);
    return strip_bri[strip];  
}





int allStrip_white(uint8_t val){
#ifdef DEBUG
   if (command_debug) fsprintln("[allStrip_bri]");
#endif

    RgbColor hsv;
    val = map(val, 0, 255, 2, 255);
    for( int i = 0; i < output_count; i++) { 
        if ((output_selected[i] == 1)  && (output_type[i] == OT_RGBWSTRIP))  {
            hsv = hsv2rgb (strip_hue[i], strip_sat[i], val, 0);
            strip_color[i]  = String(hsv.R) + "." + String(hsv.G) + "." + String(hsv.B);
            strip_hBri[i] = val;
            setWhite(i, val, true);
            toggleOnOff[i] = true;
            trueWhite[i] = true;
        }
    }    
}
int selectedStrip_white(String v, boolean select){ 
#ifdef DEBUG
   if (command_debug) fsprintf("\n[selectedStrip_bri] : %s\n", v.c_str());
#endif


    String Exploed[2];

    explode(v, '|', Exploed);

    int strip =  Exploed[0].toInt();

    if (strip > output_count) {
        #ifdef DEBUG
        if (command_debug){
        Serial.printf("[selectedStrip_bri] pos %d > stripcount: %d", 
        strip, 
        output_count);}
        #endif
        return -1;
    }

    if (output_type[strip] != OT_RGBWSTRIP) return -1;

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
    state = map(state, 0, 255, 2, 255);
    RgbColor hsv = hsv2rgb (strip_hue[strip], strip_sat[strip], state, 0);
    strip_color[strip]  = String(hsv.R) + "." + String(hsv.G) + "." + String(hsv.B);
    strip_hBri[strip] = state;
    setWhite(strip, state, true);
    toggleOnOff[strip] = true;
    trueWhite[strip] = true;
}




int selectedStrip_setcolorFromRGB(String v, boolean select){ 
#ifdef DEBUG
   if (command_debug) fsprintf("\n[selectedStrip_setcolorShow] : %s\n", v.c_str());
#endif


    String Exploed[2];

    explode(v, '|', Exploed);

    int strip =  Exploed[0].toInt();

    if (strip > output_count) {
        #ifdef DEBUG
        if (command_debug){
        Serial.printf("[selectedStrip_setcolorShow] pos %d > stripcount: %d", 
        strip, 
        output_count);}
        #endif
        return -1;
    }

    if (!output_isStrip(strip)) return -1;

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

    String  state       = Exploed[1];
    int     *c          = splitColor(state);
    HsvColor hsv        = rgb2hsv(c[0], c[1], c[2]);
    strip_color[strip]  = String(c[0]) + "." + String(c[1]) + "." + String(c[2]);
    strip_hBri[strip]   = hsv.v;
    strip_hue[strip]    = hsv.h;
    strip_sat[strip]    = hsv.s;
    setColorFromString(strip, true);
    toggleOnOff[strip] = true;
}

#endif