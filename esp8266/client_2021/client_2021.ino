#include "ADS_lib.h"


void setup()
{


	Serial.begin(115200);

            WiFi.disconnect(true);
            WiFi.softAPdisconnect(true);
            WiFi.setPhyMode(WIFI_PHY_MODE_11B);

	delay(3000);


	Serial.println("");
	Serial.println("");

	set_def();
	print_output();
	// delay(3000);	
}

void loop()
{

}
