
#include "device_def.h"
#include "devices.h"

#if CLIENT_SELECT==CLIENT_LED

#include "fastled_func.h"

#include "colors.h"
#include "tools.h"

#if LED_LIB==NEOPIXz
	#include <FastLED.h>

#endif
#if LED_LIB==NEOPIX
	#include <Adafruit_NeoPixel.h>

#endif
#if LED_LIB==FASTLED
void rainbowSolid(uint8_t sHue) {
   for( int i = 0; i < output_count; i++) { 
      // Serial.printf("%d - %d - %d\n", i, devices[DEVICE_POS].strip_size[i], devices[DEVICE_POS].patternStrip_selected[i]);
      if (patternStrip_selected[i] == 1) {
        fill_solid(Strip_array[i], strip_size[i], CHSV(sHue, 255, 255));        
      }
   }  
}
void rainbow(uint8_t sHue) 
{
   for( int i = 0; i < output_count; i++) { 
      // Serial.printf("%d - %d - %d\n", i, devices[DEVICE_POS].strip_size[i], devices[DEVICE_POS].patternStrip_selected[i]);
      if (patternStrip_selected[i] == 1) {
        fill_rainbow( Strip_array[i], strip_size[i], sHue, 7);      
      }
   } 
}
void rainbowSolid( ) {
	for( int i = 0; i < output_count; i++) { 
		patternStrip_selected[i] = 1;
	} 	
	static uint8_t sHue;
	static long last_change;
	long now = millis();
	if ((now - last_change) > 50) {
		last_change = now;
		sHue++;
	}
 	rainbow(sHue);
 	led_show();
}

#endif

#if LED_LIB==NEOPIX
// Fill the dots one after the other with a color
// void colorWipe(uint32_t c, uint8_t wait) {
//   for(uint16_t i=0; i<strip.numPixels(); i++) {
//       strip.setPixelColor(i, c);
//       strip.show();
//       delay(wait);
//   }
// }
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos, int pos) {
	if(WheelPos < 85) {
		#ifdef LED_PIN_S1
			if (pos == 1) {
				return strip_1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
			}
		#endif
		#ifdef LED_PIN_S2
			if (pos == 2) {
				return strip_2.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
			}
		#endif
		#ifdef LED_PIN_S3
			if (pos == 3) {
				return strip_3.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
			}
		#endif
		#ifdef LED_PIN_S4
			if (pos == 4) {
				return strip_4.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
			}
		#endif
		#ifdef LED_PIN_S5
			if (pos == 5) {
				return strip_5.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
			}
		#endif
		#ifdef LED_PIN_S6
			if (pos == 6) {
				return strip_6.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
			}
		#endif									

	} else if(WheelPos < 170) {
		WheelPos -= 85;
		#ifdef LED_PIN_S1
			if (pos == 1) {
				return strip_1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
			}
		#endif
		#ifdef LED_PIN_S2
			if (pos == 2) {
				return strip_2.Color(255 - WheelPos * 3, 0, WheelPos * 3);
			}
		#endif
		#ifdef LED_PIN_S3
			if (pos == 3) {
				return strip_3.Color(255 - WheelPos * 3, 0, WheelPos * 3);
			}
		#endif
		#ifdef LED_PIN_S4
			if (pos == 4) {
				return strip_4.Color(255 - WheelPos * 3, 0, WheelPos * 3);
			}
		#endif
		#ifdef LED_PIN_S5
			if (pos == 5) {
				return strip_5.Color(255 - WheelPos * 3, 0, WheelPos * 3);
			}
		#endif
		#ifdef LED_PIN_S6
			if (pos == 6) {
				return strip_6.Color(255 - WheelPos * 3, 0, WheelPos * 3);
			}
		#endif											
	} else {
		WheelPos -= 170;
		#ifdef LED_PIN_S1
			if (pos == 1) {
				return strip_1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
			}
		#endif
		#ifdef LED_PIN_S2
			if (pos == 2) {
				return strip_2.Color(0, WheelPos * 3, 255 - WheelPos * 3);
			}
		#endif
		#ifdef LED_PIN_S3
			if (pos == 3) {
				return strip_3.Color(0, WheelPos * 3, 255 - WheelPos * 3);
			}
		#endif
		#ifdef LED_PIN_S4
			if (pos == 4) {
				return strip_4.Color(0, WheelPos * 3, 255 - WheelPos * 3);
			}
		#endif
		#ifdef LED_PIN_S5
			if (pos == 5) {
				return strip_5.Color(0, WheelPos * 3, 255 - WheelPos * 3);
			}
		#endif
		#ifdef LED_PIN_S6
			if (pos == 6) {
				return strip_6.Color(0, WheelPos * 3, 255 - WheelPos * 3);
			}
		#endif											
	}
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
  	#ifdef LED_PIN_S1
  		if (patternStrip_selected[0] == 1) {
		    for(i=0; i<strip_1.numPixels(); i++) {
		      strip_1.setPixelColor(i, Wheel((i+j) & 255, 1));
		      // delay(wait);
		      // strip_1.show();
		    }
	    }
    #endif
  	#ifdef LED_PIN_S2
  		if (patternStrip_selected[1] == 1) {
		    for(i=0; i<strip_2.numPixels(); i++) {
		      strip_2.setPixelColor(i, Wheel((i+j) & 255, 2));
		      // delay(wait);
		      // strip_2.show();
		    }
	    }
    #endif
  	#ifdef LED_PIN_S3
  		if (patternStrip_selected[2] == 1) {
		    for(i=0; i<strip_3.numPixels(); i++) {
		      strip_3.setPixelColor(i, Wheel((i+j) & 255, 3));
		      // delay(wait);
		      // strip_3.show();
		    }
	    }
    #endif
  	#ifdef LED_PIN_S4
  		if (patternStrip_selected[3] == 1) {
		    for(i=0; i<strip_4.numPixels(); i++) {
		      strip_4.setPixelColor(i, Wheel((i+j) & 255, 4));
		      // delay(wait);
		      // strip_4.show();
		    }
	    }
    #endif
  	#ifdef LED_PIN_S5
  		if (patternStrip_selected[4] == 1) {
		    for(i=0; i<strip_5.numPixels(); i++) {
		      strip_5.setPixelColor(i, Wheel((i+j) & 255, 5));
		      // delay(wait);
		      // strip_5.show();
		    }
	    }
    #endif
  	#ifdef LED_PIN_S6
  		if (patternStrip_selected[5] == 1) {
		    for(i=0; i<strip_6.numPixels(); i++) {
		      strip_6.setPixelColor(i, Wheel((i+j) & 255, 6));
		      // delay(wait);
		      // strip_6.show();		      
		    }
	    }
    #endif
      delay(wait);
      led_show();      	    

  }
}
void rainbowSolid( ){
	for( int i = 0; i < output_count; i++) { 
		patternStrip_selected[i] = 1;
	} 		
	rainbow(20);
	
	
};

// Slightly different, this makes the rainbow equally distributed throughout
// void rainbowCycle(uint8_t wait) {
//   uint16_t i, j;

//   for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
//     for(i=0; i< strip.numPixels(); i++) {
//       strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
//     }
//     strip.show();
//     delay(wait);
//   }
// }


#endif
#endif