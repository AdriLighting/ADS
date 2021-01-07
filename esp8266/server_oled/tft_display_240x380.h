	#include <MiniGrafx.h>
	#include <Carousel.h>
	#include <ILI9341_SPI.h> 

	#include "tft_display.h"
	#include "devices.h"

	#include "ArialRounded.h"
	#include "moonphases.h"
	#include "weathericons.h"

#define TFT_DC D2
#define TFT_CS D1
#define TFT_LED D8

	// defines the colors usable in the paletted 16 color frame buffer
	uint16_t palette[] = {ILI9341_BLACK, // 0
	                      ILI9341_WHITE, // 1
	                      ILI9341_YELLOW, // 2
	                      0x7E3C
	                      }; //3

	int SCREEN_WIDTH = 240;
	int SCREEN_HEIGHT = 320;
	// Limited to 4 colors due to memory constraints
	int BITS_PER_PIXEL = 2; // 2^2 =  4 colors

	ILI9341_SPI tft = ILI9341_SPI(TFT_CS, TFT_DC);
	MiniGrafx gfx = MiniGrafx(&tft, BITS_PER_PIXEL, palette);
	Carousel carousel(&gfx, 0, 0, 240, 100);

	// FrameCallback frames[] = { drawForecast1, drawForecast2, drawForecast3 };
	// int frameCount = 3;

	// how many different screens do we have?
	int screenCount = 5;
	long lastDownloadUpdate = millis();

	uint16_t screen = 0;
	long timerPress;
	bool canBtnPress;
	time_t dstOffset = 0;	





	tft_disp_mods tft_disp_mod;

#ifdef OLED_UPD_WEBSOCKET
	extern oled_requests oled_request;
#endif

void tft_setup() {
	
                        	
}

String setDigit(int val) {
	if (val < 10) {
		return "0"+String(val);
	}
	return String(val);
}
void tft_update_time(int x, int y) { // 11,34

}
void tft_update_temp(int x, int y){// 11,114

}
void tft_update_tempIco(int x, int y){// 11,114

}
boolean tft_can_update(){
	if (tft_disp_mod == tdm_tdm) return true;
	if (tft_disp_mod == tdm_tm) return true;
	return false;
}
void tft_update_time(){
	if (!tft_can_update())  return;
	tft_update_time(11, 34);
}
void tft_update_temp(){
	if (!tft_can_update()) return;
	if (tft_disp_mod == tdm_tdm) 	tft_update_temp(11, 114);	
	if (tft_disp_mod == tdm_tm) 	tft_update_tempIco(11, 80);	
}
void tft_display_tm(){



}
void tft_display_tdm(){



}

void tft_display_server_statu(boolean show){


}

int tft_bmp_fileList_max = 30;
String tft_bmp_fileList[30];

void tft_bmp_fileList_init(String dirPath) { 


}
void tft_bmp_fileList_display() { 

}

#define BUFFPIXEL 20


// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.
 
uint16_t read16(File f) {
	uint16_t result;
	((uint8_t *)&result)[0] = f.read(); // LSB
	((uint8_t *)&result)[1] = f.read(); // MSB
	return result;
}
 
uint32_t read32(File f) {
	uint32_t result;
	((uint8_t *)&result)[0] = f.read(); // LSB
	((uint8_t *)&result)[1] = f.read();
	((uint8_t *)&result)[2] = f.read();
	((uint8_t *)&result)[3] = f.read(); // MSB
	return result;
}
void bmpDraw(char *filename, uint8_t x, uint16_t y) {

}