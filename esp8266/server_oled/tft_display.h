#ifndef TFT_DISPLAY_H
   #define TFT_DISPLAY_H
   #include <arduino.h>
   #include <FS.h>   

   // uint16_t read16(File f);
   // uint32_t read32(File f);
	void bmpDraw(char *filename, uint8_t x, uint16_t y) ;
   void tft_update_tdm();
	void tft_setup();
   enum tft_disp_mods { 
      tdm_tdm,
      tdm_tm,
      tdm_s
   };
   void tft_display_server_statu(boolean show);
   void tft_update_time();
   void tft_update_temp();
   void tft_display_tdm();
   void tft_display_tm();
   void tft_bmp_fileList_init(String dirPath);
   void tft_bmp_fileList_display();
#endif

      // LampeColor_string = "rgb(" + red + ", " + green + ", " + blue + ")";
