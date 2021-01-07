

	#if DEVICE_SELECT==BUREAU_ECRAN
		#define CLIENT_SELECT		CLIENT_LED

	    #define BOARD_NAME          String("LOLIN(WEMOS) D1 mini")
	    #define CHIPSET_NAME        String("WS2812B")
	    #define COLOR_ORDER_NAME    String("GRB")
		#define DEVICENAME 			String("BUREAU_ECRAN")  
		#define HOST_NAME_PREFIX 	"BUREAU_ECRAN_"	
	    #define COLOR_ORDER         GRB
	    #define CHIPSET             WS2812B

	    #define NUMS_LED_S1         22
	    #define NUMS_LED_S2         80
	    #define NUMS_LED_S3         33
	    #define NUMS_LED_S4         22
	    #define NUMS_LED_S5         13
	    #define NUMS_LED_S6         10

	    #define LED_PIN_S1          5
	    #define LED_PIN_S2          4
	    #define LED_PIN_S3          2
	    #define LED_PIN_S4          D5
	    #define LED_PIN_S5          D6
	    #define LED_PIN_S6          D7

	    #define STRIP_COUNT         6

	    #define LED_LIB             NEOPIX

		#define OUTPUT_ON 			HIGH
		#define OUTPUT_OFF 			LOW
	    #define DIGIT_PIN_S1 		15
    	#define DIGIT_MOD       	DIGIT_MOD_1

		#define LOAD_STARTVALUE
	#endif

	#if DEVICE_SELECT==BUREAU
		#define CLIENT_SELECT		CLIENT_LED

	    #define BOARD_NAME          String("LOLIN(WEMOS) D1 mini")
	    #define CHIPSET_NAME        String("WS2812B")
	    #define COLOR_ORDER_NAME    String("GRB")
		#define DEVICENAME 			String("BUREAU_HAUT")  
		#define HOST_NAME_PREFIX 	"BUREAU_HAUT_"	
	    #define COLOR_ORDER         GRB
	    #define CHIPSET             WS2812B
	    #define LED_LIB             NEOPIX

	    #define NUMS_LED_S1         54
		
		#define LED_PIN_S1 			D1
		#define LED_PIN_S1_RGBW
	    #define STRIP_COUNT         1

		#define OUTPUT_ON 			LOW
		#define OUTPUT_OFF 			HIGH
	    #define DIGIT_PIN_S1 		D5
    	#define DIGIT_MOD       	DIGIT_MOD_1

		#define LOAD_STARTVALUE
	#endif

	#if DEVICE_SELECT==TDN
		#define CLIENT_SELECT		CLIENT_LED

	    #define BOARD_NAME          String("LOLIN(WEMOS) D1 mini")
	    #define CHIPSET_NAME        String("WS2812B")
	    #define COLOR_ORDER_NAME    String("GRB")
		#define DEVICENAME 			String("TDN")  
		#define HOST_NAME_PREFIX 	"TDN_"	
	    #define COLOR_ORDER         GRB
	    #define CHIPSET             WS2812B
	    #define LED_LIB             NEOPIX

	    #define NUMS_LED_S1         33
	    #define NUMS_LED_S2         47


	    #define LED_PIN_S1          D1
	    #define LED_PIN_S2          D2

	    #define STRIP_COUNT         2

		#define OUTPUT_ON 			LOW
		#define OUTPUT_OFF 			HIGH
	    #define DIGIT_PIN_S1 		D4
    	#define DIGIT_MOD       	DIGIT_MOD_1

		#define LOAD_STARTVALUE
	#endif



	#if DEVICE_SELECT==PLAFOND_LIT
		#define CLIENT_SELECT		CLIENT_LED

	    #define BOARD_NAME          String("LOLIN(WEMOS) D1 mini")
	    #define CHIPSET_NAME        String("WS2812B")
	    #define COLOR_ORDER_NAME    String("GRB")
		#define DEVICENAME 			String("PLAFOND_LIT")  
		#define HOST_NAME_PREFIX 	"PLAFOND_LIT_"	
	    #define COLOR_ORDER         GRB
	    #define CHIPSET             WS2812B
	    #define LED_LIB             NEOPIX

	    #define NUMS_LED_S1         38
		
		#define LED_PIN_S1 			D1
		#define LED_PIN_S1_RGBW
	    #define STRIP_COUNT         1

		#define OUTPUT_ON 			LOW
		#define OUTPUT_OFF 			HIGH
    	#define DIGIT_MOD       	DIGIT_MOD_0
		
		#define LOAD_STARTVALUE
	#endif


	#if DEVICE_SELECT==PLAFOND_ENTER
		#define CLIENT_SELECT		CLIENT_LED

	    #define BOARD_NAME          String("LOLIN(WEMOS) D1 mini")
	    #define CHIPSET_NAME        String("WS2812B")
	    #define COLOR_ORDER_NAME    String("GRB")
		#define DEVICENAME 			String("PLAFOND_ENTER")  
		#define HOST_NAME_PREFIX 	"PLAFOND_ENTER_"	
	    #define COLOR_ORDER         GRB
	    #define CHIPSET             WS2812B
	    #define LED_LIB             NEOPIX

	    #define NUMS_LED_S1         20
		#define LED_PIN_S1 			D1
		#define LED_PIN_S1_RGBW

	    #define STRIP_COUNT         1

		#define OUTPUT_ON 			LOW
		#define OUTPUT_OFF 			HIGH
    	#define DIGIT_MOD       	DIGIT_MOD_0

		#define LOAD_STARTVALUE
	#endif

	#if DEVICE_SELECT==PLAFOND_RELAY
		#define CLIENT_SELECT		CLIENT_RELAY

	    #define BOARD_NAME          String("MODEMCU12e(amica)")
	    #define CHIPSET_NAME        String("WS2812B")
	    #define COLOR_ORDER_NAME    String("GRB")
		#define DEVICENAME 			String("PLAFOND_RELAY")  
		#define HOST_NAME_PREFIX 	"PLAFOND_RELAY_"	

	    #define RELAY_PIN_S1 		D1

	    #define STRIP_COUNT         1

		#define LED_LIB             LEDLIB_NO

		#define OUTPUT_ON 			HIGH
		#define OUTPUT_OFF 			LOW
    	#define DIGIT_MOD       	DIGIT_MOD_0

	#endif


	#if DEVICE_SELECT==FLEUR
		#define CLIENT_SELECT		CLIENT_LED

	    #define BOARD_NAME          String("LOLIN(WEMOS) D1 mini")
	    #define CHIPSET_NAME        String("WS2812B")
	    #define COLOR_ORDER_NAME    String("GRB")
		#define DEVICENAME 			String("FLEUR")  
		#define HOST_NAME_PREFIX 	"FLEUR_"	
	    #define COLOR_ORDER         GRB
	    #define CHIPSET             WS2812B
	    #define LED_LIB             NEOPIX

	    #define NUMS_LED_S1         15
	    #define NUMS_LED_S2         20
		
		#define LED_PIN_S1 			D2
		#define LED_PIN_S2 			D1
		#define LED_PIN_S1_RGBW
	    #define STRIP_COUNT         2

		#define OUTPUT_ON 			LOW
		#define OUTPUT_OFF 			HIGH
    	#define DIGIT_MOD       	DIGIT_MOD_0
		
		#define LOAD_STARTVALUE
	#endif

	#if DEVICE_SELECT==AMPLI
		#define CLIENT_SELECT		CLIENT_LED

	    #define BOARD_NAME          String("LOLIN(WEMOS) D1 mini")
	    #define CHIPSET_NAME        String("WS2812B")
	    #define COLOR_ORDER_NAME    String("GRB")
		#define DEVICENAME 			String("AMPLI")  
		#define HOST_NAME_PREFIX 	"AMPLI_"	
	    #define COLOR_ORDER         GRB
	    #define CHIPSET             WS2812B
	    #define LED_LIB             NEOPIX

	    #define NUMS_LED_S1         60
		#define LED_PIN_S1_RGBW
		
		#define LED_PIN_S1 		D1

	    #define STRIP_COUNT         1

		#define OUTPUT_ON 			LOW
		#define OUTPUT_OFF 			HIGH
    	#define DIGIT_MOD       	DIGIT_MOD_0

		#define LOAD_STARTVALUE
	#endif

	#if DEVICE_SELECT==SUSPENDU
		#define CLIENT_SELECT		CLIENT_LED

	    #define BOARD_NAME          String("LOLIN(WEMOS) D1 mini")
	    #define CHIPSET_NAME        String("WS2812B")
	    #define COLOR_ORDER_NAME    String("GRB")
		#define DEVICENAME 			String("SUSPENDU")  
		#define HOST_NAME_PREFIX 	"SUSPENDU_"	
	    #define COLOR_ORDER         GRB
	    #define CHIPSET             WS2812B
	    #define LED_LIB             NEOPIX

	    #define NUMS_LED_S1         144
		
		#define LED_PIN_S1 		D1

	    #define STRIP_COUNT         1

		#define OUTPUT_ON 			LOW
		#define OUTPUT_OFF 			HIGH
    	#define DIGIT_MOD       	DIGIT_MOD_0

		#define LOAD_STARTVALUE
	#endif


	#if DEVICE_SELECT==ENTER_RIGHT
		#define CLIENT_SELECT		CLIENT_LED

	    #define BOARD_NAME          String("MODEMCU12e(amica)")
	    #define CHIPSET_NAME        String("WS2812B")
	    #define COLOR_ORDER_NAME    String("GRB")
		#define DEVICENAME 			String("ENTER_RIGHT")  
		#define HOST_NAME_PREFIX 	"ENTER_RIGHT_"	
	    #define COLOR_ORDER         GRB
	    #define CHIPSET             WS2812B
	    #define LED_LIB             NEOPIX

	    #define NUMS_LED_S1         61
	    #define NUMS_LED_S2         35
		
		#if LED_LIB==FASTLED
	    	#define LED_PIN_S1 		D1
		#endif
		#if LED_LIB==NEOPIX
	    	#define LED_PIN_S1 		D1
		#endif
		#define LED_PIN_S2 			D4

	    #define STRIP_COUNT         2

		#define OUTPUT_ON 			LOW
		#define OUTPUT_OFF 			HIGH
	    #define DIGIT_PIN_S1 		D2
	    #define DIGIT_PIN_S2 		D5
    	#define DIGIT_MOD       	DIGIT_MOD_2

		#define LOAD_STARTVALUE
	#endif

	#if DEVICE_SELECT==DOUBLEAPPLIQUE
		#define CLIENT_SELECT		CLIENT_LED

	    #define BOARD_NAME          String("LOLIN(WEMOS) D1 mini")
	    #define CHIPSET_NAME        String("WS2812B")
	    #define COLOR_ORDER_NAME    String("GRB")
		#define DEVICENAME 			String("DOUBLEAPPLIQUE")  
		#define HOST_NAME_PREFIX 	"DOUBLEAPPLIQUE_"	
	    #define COLOR_ORDER         GRB
	    #define CHIPSET             WS2812B
	    #define LED_LIB             NEOPIX

	    #define NUMS_LED_S1         34
	    #define NUMS_LED_S2         44


	    #define LED_PIN_S1          D1
	    #define LED_PIN_S2          D2


	    #define STRIP_COUNT         2

		#define OUTPUT_ON 			LOW
		#define OUTPUT_OFF 			HIGH
	    #define DIGIT_PIN_S1 		D4
    	#define DIGIT_MOD       	DIGIT_MOD_1

		#define LOAD_STARTVALUE
	#endif

	#if DEVICE_SELECT==SALON_CANAPE_RELAY
		#define CLIENT_SELECT		CLIENT_RELAY

	    #define BOARD_NAME          String("MODEMCU12e(amica)")
	    #define CHIPSET_NAME        String("WS2812B")
	    #define COLOR_ORDER_NAME    String("GRB")
		#define DEVICENAME 			String("SALON_CANAPE_RELAY")  
		#define HOST_NAME_PREFIX 	"SALON_CANAPE_RELAY_"	

	    #define RELAY_PIN_S1 		12

	    #define STRIP_COUNT         1

		#define LED_LIB             LEDLIB_NO

		#define OUTPUT_ON 			HIGH
		#define OUTPUT_OFF 			LOW
		#define DIGIT_PIN_S1 		0
    	#define DIGIT_MOD       	DIGIT_MOD_3

	#endif