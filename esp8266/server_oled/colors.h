#ifndef COLORS_H
   #define COLORS_H
   #include <arduino.h>
   #include "devices.h"

   typedef struct RgbColor {uint8_t R; uint8_t G; uint8_t B; };
   typedef struct HsvColor {uint8_t h; uint8_t s; uint8_t v; };

   RgbColor hsv2rgb (uint8_t h, uint8_t s, uint8_t v, int pix_nbr);
   HsvColor rgb2hsv(unsigned char r, unsigned char  g, unsigned char  b);

   int* splitColor(String Val);
#endif

      // LampeColor_string = "rgb(" + red + ", " + green + ", " + blue + ")";
