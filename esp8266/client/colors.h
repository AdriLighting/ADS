#ifndef COLORS_H
   #define COLORS_H
   #include <arduino.h>
   #include "devices.h"

   #if LED_LIB==FASTLED
      #include <FastLED.h>
      FASTLED_USING_NAMESPACE 
      String   RGBtoString(CRGB c);
      CRGB     parsecolor(String Val);
      CRGB     parsecolorEx(String Val);
   #endif

   #if LED_LIB==NEOPIX

      
      uint32_t parsecolor(String Val);
   #endif

   typedef struct RgbColor {uint8_t R; uint8_t G; uint8_t B; };
   typedef struct HsvColor {uint8_t h; uint8_t s; uint8_t v; };

   extern String LampeColor_string;

   RgbColor hsv2rgb (uint8_t h, uint8_t s, uint8_t v, int pix_nbr);
   HsvColor rgb2hsv(unsigned char r, unsigned char  g, unsigned char  b);

   int* splitColor(String Val);
#endif

      // LampeColor_string = "rgb(" + red + ", " + green + ", " + blue + ")";
