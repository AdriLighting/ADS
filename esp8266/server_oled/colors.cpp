
#include "colors.h"

#define HSV_SECTION_3 (86)

RgbColor hsv2rgb (uint8_t h, uint8_t s, uint8_t v, int pix_nbr) {
	//long start=micros();
	RgbColor rgb;

	uint8_t value = v;
	uint8_t sat = s;

	// The brightness floor is minimum number that all of R, G, and B will be set to.
	uint8_t brightness_floor=(value*(255-sat))/256;

	// The color amplitude is the maximum amount of R, G, and B that will be added on top of the brightness_floor
	uint8_t color_amplitude = value-brightness_floor;

	// Figure out which section of the hue wheel we're in, and how far offset we are within that section
	uint8_t section = h / HSV_SECTION_3;              // 0..2
	uint8_t offset  = h % HSV_SECTION_3;              // 0..63

	uint8_t rampup = offset;                    // 0..63
	uint8_t rampdown = (HSV_SECTION_3 - 1) - offset;  // 63..0

	// compute color amplitude scaled down versions of rampup and rampdown
	uint8_t rampup_amp_adj   = (rampup   * color_amplitude) / (86);
	uint8_t rampdown_amp_adj = (rampdown * color_amplitude) / (86);

	// add brightness_floor offset to everything
	uint8_t rampup_adj_with_floor   = rampup_amp_adj   + brightness_floor;
	uint8_t rampdown_adj_with_floor = rampdown_amp_adj + brightness_floor;

	switch (section) {
		case 0:
			rgb.R = rampdown_adj_with_floor;
			rgb.G = rampup_adj_with_floor;
			rgb.B = brightness_floor;
		break;
		case 1:
			rgb.R = brightness_floor;
			rgb.G = rampdown_adj_with_floor;
			rgb.B = rampup_adj_with_floor;
		break;
		case 2:
			rgb.R = rampup_adj_with_floor;
			rgb.G = brightness_floor;
			rgb.B = rampdown_adj_with_floor;
		break;
	}

   	return rgb;
}
HsvColor rgb2hsv(unsigned char r, unsigned char  g, unsigned char  b) {
	HsvColor hsv;
	unsigned char rgb_min, rgb_max;

	rgb_min = r<g ? (r<b?r:b) : (g<b?g:b);
	rgb_max = r>g ? (r>b?r:b) : (g>b?g:b);

	hsv.v = rgb_max;
	if (hsv.v==0)  { hsv.h=0; hsv.s=0; return hsv; }
	hsv.s = 255 * long(rgb_max - rgb_min) / hsv.v;

	if (hsv.s==0)   { hsv.h=0;  return hsv; }

	if (rgb_max==r) 		hsv.h =   	0+43*(g-b)/(rgb_max-rgb_min);
	else if (rgb_max==g)   	hsv.h =  	85+43*(b-r)/(rgb_max-rgb_min);
	else               		hsv.h = 	171+43*(r-g)/(rgb_max-rgb_min);

	return hsv;
}

int* splitColor(String Val) {
    String      aVal            = Val;
    byte        firstIndex      = aVal.indexOf('.');
    byte        secondIndex     = aVal.indexOf('.', firstIndex + 1);
    String      red             = aVal.substring(0, firstIndex);
    String      green           = aVal.substring(firstIndex + 1, secondIndex);
    String      blue            = aVal.substring(secondIndex + 1);
    int         r               = red.toInt();
    int         g               = green.toInt();
    int         b               = blue.toInt();
                r               = map(r, 0, 255, 0, 255);
                g               = map(g, 0, 255, 0, 255);
                b               = map(b, 0, 255, 0, 255);

    int     *array          = new int[3];
            array[0]        = r;
            array[1]        = g;
            array[2]        = b;
    return array;    
}

