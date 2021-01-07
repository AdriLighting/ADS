	// #define OLED_ENABLE
	#define WEBSERVER_ENABLE
// 
	// #define DEVICEFROMSPIFF
	#ifdef WEBSERVER_ENABLE
		#define OW_METEO
		#define TFT_ENABLE
		#define TFT_128x128
		#define TFT_240x320
		#define OLED_UPD_WEBSOCKET
		#define DEVICEFROMSPIFF
		// #define DHT_ENABLE
		#define NTP_TIME
		#define EVENT_TIMER
		#define ESP_WEBSOCKETSERVER
		#define DEVICENAME String("DADRI_SERVER")  
		#define HOST_NAME_PREFIX "DADRI_SERVER_"	
		#define DHTPIN D5
		#define DHTTYPE DHT22 
	#endif
	#define OTA_ENABLE

	#ifdef OLED_ENABLE
		#define OLED_UPD_WEBSOCKET
		#define OLED_DEVICE_1 0
		#define OLED_DEVICE_2 1
		#define OLED_DEVICE_3 2
		#define OLED_DEVICE_SELECT OLED_DEVICE_3
		#if OLED_DEVICE_SELECT==OLED_DEVICE_1
			#define DEVICENAME String("DABOX_SERVER")  
			#define HOST_NAME_PREFIX "DABOX_SERVER_"	
		#endif
		#if OLED_DEVICE_SELECT==OLED_DEVICE_2
			#define DEVICENAME String("DAPLEX_SERVER")  
			#define HOST_NAME_PREFIX "DAPLEX_SERVER_"	
		#endif
		#if OLED_DEVICE_SELECT==OLED_DEVICE_3
			#define DEVICENAME String("DADESK_SERVER")  
			#define HOST_NAME_PREFIX "DADESK_SERVER_"	
		#endif
	#endif

	#define MAX_OUTPUTS 6
	#define MAX_DEVICES 20
	
// if (om_arborescence_item[1] == c_to_string(om_act_rommGrp)) 
// if (om_arborescence_item[2] == room) 
// 	0 : color - 1 : room_group - 2 : salon - 3 : bureau