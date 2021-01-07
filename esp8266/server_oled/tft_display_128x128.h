	#include <TimeLib.h>
	#include <calendar.h>
    #include <FS.h>   


	#include <Adafruit_GFX.h>    // Core graphics library
	#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
	#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
	#include <SPI.h>
	#include "tft_display.h"
	#include "devices.h"
	#include "fonts/SansSerif_bold_15.h"
	#include "fonts/SansSerif_plain_10.h"
	#ifdef OW_METEO
		#include "meteo.h"
		// #include "weathericons.h"
	#endif
	#include "adriwifi.h"
	#define TFT_CS         D2
	#define TFT_RST        D3                                            
	#define TFT_DC         D4

	Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


	tft_disp_mods tft_disp_mod;

#ifdef OLED_UPD_WEBSOCKET
	extern oled_requests oled_request;
#endif

void tft_setup() {
	

	tft.initR(INITR_144GREENTAB); 
	tft.setRotation(0);
	// tft.fillScreen(ST77XX_BLACK);
	// bmpDraw( getMeteoconIconFromProgmem("10d") , 0, 0);
	// delay(3000);

	tft_display_tdm();                           	
}

String setDigit(int val) {
	if (val < 10) {
		return "0"+String(val);
	}
	return String(val);
}
void tft_update_time(int x, int y) { // 11,34
	tft.setTextSize(2); 
	tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);  
	tft.setCursor(x, y);                          
	tft.print( setDigit(hour()) + ":" + setDigit(minute())  + ":" + setDigit(second()));  
}
void tft_update_temp(int x, int y){// 11,114
	String temp = String(currentWeather.temp, 1) ;
	String hum = String(currentWeather.humidity) ;
	String desc = currentWeather.description ;
	tft.setTextSize(2);
	tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);  		
	tft.setCursor(x, y);                          
	tft.print( temp + " - " + hum);  

	// tft.print( temp );  
	// String iconName = currentWeather.icon;
	// iconName.replace("n","d");
	// String picPath = "/bmp/" + iconName + ".bmp";
	// char buff[100];
	// sprintf(buff, "%s", picPath.c_str());
	// bmpDraw(buff, x + 55, y-20);	
}
void tft_update_tempIco(int x, int y){// 11,114
	String temp = String(currentWeather.temp, 1) ;
	String hum = String(currentWeather.humidity) ;
	String desc = currentWeather.description ;
	tft.setTextSize(2);
	tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);  		
	tft.setCursor(x, y);                          
	tft.print( temp);  
	String iconName = currentWeather.icon;
	iconName.replace("n","d");
	String picPath = "/bmp/" + iconName + ".bmp";
	char buff[100];
	sprintf(buff, "%s", picPath.c_str());
	bmpDraw(buff, x + 55, y+5);	
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

	tft_disp_mod = tdm_tm;

	tft.fillScreen(ST77XX_BLACK);
	// bmpDraw("/horloge.bmp", 0, 0) ;
	// bmpDraw("/meteo.bmp", 0, 80) ;	
	// ui.setTextColor(ST77XX_ORANGE, ST77XX_BLACK);
	// ui.setTextAlignment(CENTER);
	// ui.drawString(75, 80,  "Temperature");	

	tft.drawFastHLine(0, 15 ,  tft.width(), ST77XX_CYAN);
	tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);        
	tft.setTextSize(1);                                  
	tft.setCursor(25, 5);                                
	tft.print("ADRI DOMOTIQUE");

	tft.drawFastHLine(0, 55,  tft.width(), ST77XX_CYAN);  
	tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);           
	tft.setCursor(40, 20);                                
	tft.print("HORLOGE ");     


	tft.setTextColor(ST77XX_CYAN, ST7735_BLACK);        
	tft.setCursor(40, 60);                               
	tft.print("TEMPERATURE ");  
	tft.setTextSize(2);   

}
void tft_display_tdm(){

	tft_disp_mod = tdm_tdm;

	tft.fillScreen(ST77XX_BLACK);
	// bmpDraw("/horloge.bmp", 0, 0) ;
	// bmpDraw("/meteo.bmp", 0, 80) ;	
	// ui.setTextColor(ST77XX_ORANGE, ST77XX_BLACK);
	// ui.setTextAlignment(CENTER);
	// ui.drawString(75, 80,  "Temperature");	

	tft.drawFastHLine(0, 15 ,  tft.width(), ST77XX_CYAN);
	tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);        
	tft.setTextSize(1);                                  
	tft.setCursor(25, 5);                                
	tft.print("ADRI DOMOTIQUE");

	tft.drawFastHLine(0, 55,  tft.width(), ST77XX_CYAN);  
	tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);           
	tft.setCursor(40, 20);                                
	tft.print("HORLOGE ");     

	tft.drawFastHLine(0, 95,  tft.width(), ST77XX_CYAN);  
	tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);          
	tft.setCursor(40, 60);                               
	tft.print("DATE "); 
	tft.setTextColor(ST77XX_CYAN, ST7735_BLACK);        
	tft.setCursor(40, 100);                               
	tft.print("TEMPERATURE ");  
	tft.setTextSize(2);   


	char* cDay = dayStr(weekday());
	int search_day = calendar_date_get_dow("en", String(cDay)); 
	String frDay = "";
	if (search_day>=0) frDay = ce_char_to_string(calendar_dates[search_day].day_fr);
	String frShortMon = "";
	char* cMon = monthStr(month());
	int search_mon = calendar_date_get_mon("en", String(cMon)); 
	if (search_mon>=0) {
		frShortMon = ce_char_to_string(calendar_months[search_mon].mon_short);
	}
	
	tft.setTextSize(1);   
	tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK); 
	tft.setCursor(10, 77); 
	tft.setFont(&SansSerif_bold_15);
	tft.print(frDay + " " + String(day()) + frShortMon );
	tft.setFont();
	tft.setTextSize(2);	

	// tft_update_time();
	// tft_update_temp();

}

void tft_display_server_statu(boolean show){
	if (show) tft_disp_mod = tdm_s;
	else {
		if (tft_disp_mod != tdm_s) return;
	}
	

	tft.fillScreen(ST77XX_BLACK);
	tft.setTextSize(2);
	tft.drawFastHLine(0, 15 ,  tft.width(), ST77XX_CYAN);
	tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);        
	tft.setTextSize(1);                                  
	tft.setCursor(25, 5);                                
	tft.print("SERVER_STATU");
	tft.setTextSize(1);
	tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);  
	int y =20;      
	#ifdef OLED_UPD_WEBSOCKET
		int cnt = oled_request_cnt();
		tft.setCursor(5, y); 
		y+=10;
		tft.print("request cnt: " + String(oled_request.cnt) + "/" + String(cnt));
		tft.setCursor(5, y);
		y+=10;
		tft.print("request state: " + String(oled_request.send));
		tft.drawFastHLine(0, y ,  tft.width(), ST77XX_CYAN);
		y+=10;
	#endif
	tft.setCursor(5, y); 
	y+=10;
	tft.print("ip: " + wifi_id_get_value("ip"));
	tft.setCursor(5, y);	
	y+=10;
	tft.print("gateway: " + wifi_id_get_value("gateway"));	
	tft.drawFastHLine(0, y ,  tft.width(), ST77XX_CYAN);

}

int tft_bmp_fileList_max = 30;
String tft_bmp_fileList[30];

void tft_bmp_fileList_init(String dirPath) { 


	for (int x=0; x < tft_bmp_fileList_max; x++) tft_bmp_fileList[x] = "";

	Dir dir = SPIFFS.openDir(dirPath);
	int i=0;
	while (dir.next()) {
		File f = dir.openFile("r");
		tft_bmp_fileList[i] = dir.fileName();
		i++;
	}
}
void tft_bmp_fileList_display() { 
	tft_disp_mod = tdm_s;

	char picPath[80];

	for (int x=0; x < tft_bmp_fileList_max; x++) { 
		if (tft_bmp_fileList[x] != "") {
			sprintf(picPath, "%s", tft_bmp_fileList[x].c_str());
			Serial.printf("%s\n",  tft_bmp_fileList[x].c_str());
			tft.fillScreen(ST77XX_BLACK);
			bmpDraw(picPath, 0, 0); 
			delay(1000);
		}
	}
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
 
	File     bmpFile;
	int      bmpWidth, bmpHeight;   // W+H in pixels
	uint8_t  bmpDepth;              // Bit depth (currently must be 24)
	uint32_t bmpImageoffset;        // Start of image data in file
	uint32_t rowSize;               // Not always = bmpWidth; may have padding
	uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
	uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
	boolean  goodBmp = false;       // Set to true on valid header parse
	boolean  flip    = true;        // BMP is stored bottom-to-top
	int      w, h, row, col;
	uint8_t  r, g, b;
	uint32_t pos = 0, startTime = millis();
 
	if((x >= tft.width()) || (y >= tft.height())) return;
 
	Serial.println();
	Serial.print(F("Loading image '"));
	Serial.print(filename);
	Serial.println('\'');
 
	// Open requested file on SD card
	if ((bmpFile = SPIFFS.open(filename, "r")) == NULL) {
		Serial.print(F("File not found"));
		return;
	}
 
	// Parse BMP header
	if(read16(bmpFile) == 0x4D42) { // BMP signature
		Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
		(void)read32(bmpFile); // Read & ignore creator bytes
		bmpImageoffset = read32(bmpFile); // Start of image data
		Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
		// Read DIB header
		Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
		bmpWidth  = read32(bmpFile);
		bmpHeight = read32(bmpFile);
		if(read16(bmpFile) == 1) { // # planes -- must be '1'
			bmpDepth = read16(bmpFile); // bits per pixel
			Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
			if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed
 
				goodBmp = true; // Supported BMP format -- proceed!
				Serial.print(F("Image size: "));
				Serial.print(bmpWidth);
				Serial.print('x');
				Serial.println(bmpHeight);
 
				// BMP rows are padded (if needed) to 4-byte boundary
				rowSize = (bmpWidth * 3 + 3) & ~3;
 
				// If bmpHeight is negative, image is in top-down order.
				// This is not canon but has been observed in the wild.
				if(bmpHeight < 0) {
					bmpHeight = -bmpHeight;
					flip      = false;
				}
 
				// Crop area to be loaded
				w = bmpWidth;
				h = bmpHeight;
				if((x+w-1) >= tft.width())  w = tft.width()  - x;
				if((y+h-1) >= tft.height()) h = tft.height() - y;
 
				// Set TFT address window to clipped image bounds
				tft.startWrite();
				tft.setAddrWindow(x, y, w, h);
 
				for (row=0; row<h; row++) { // For each scanline...
 
					// Seek to start of scan line.  It might seem labor-
					// intensive to be doing this on every line, but this
					// method covers a lot of gritty details like cropping
					// and scanline padding.  Also, the seek only takes
					// place if the file position actually needs to change
					// (avoids a lot of cluster math in SD library).
					if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
						pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
					else     // Bitmap is stored top-to-bottom
						pos = bmpImageoffset + row * rowSize;
					if(bmpFile.position() != pos) { // Need seek?
						tft.endWrite();
						bmpFile.seek(pos);
						buffidx = sizeof(sdbuffer); // Force buffer reload
					}
 
					for (col=0; col<w; col++) { // For each pixel...
						// Time to read more pixel data?
						if (buffidx >= sizeof(sdbuffer)) { // Indeed
							bmpFile.read(sdbuffer, sizeof(sdbuffer));
							buffidx = 0; // Set index to beginning
							tft.startWrite();
						}
 
						// Convert pixel from BMP to TFT format, push to display
						b = sdbuffer[buffidx++];
						g = sdbuffer[buffidx++];
						r = sdbuffer[buffidx++];
						tft.pushColor(tft.color565(r,g,b));
					} // end pixel
				} // end scanline
				tft.endWrite();
				Serial.print(F("Loaded in "));
				Serial.print(millis() - startTime);
				Serial.println(" ms");
			} // end goodBmp
		}
	}
 
	bmpFile.close();
	if(!goodBmp) Serial.println(F("BMP format not recognized."));
}
