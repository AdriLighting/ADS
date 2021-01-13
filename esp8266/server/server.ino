
#include <adri_wifiConnect.h>
#include <adri_tools.h>
#include <adri_timeNtp.h>
#include <adri_timer.h>

#include "devices.h"
#include "serverudp.h"

#define DEBUG


wifiConnect 			* myWifi;	// PTR pour unr instance statique "wifiConnect"
wifi_credential_ap		* myWifiAp;	// PTR pour unr instance statique "wifi_credential_ap"

const char 				* myWifiHostname = "MY_WIFI"; 	// AP AND DNS HOSTNAME 
													
										// 	AWC_LOOP; 		WIFI CONNECT STARTUP WITH STATIC 
										// 	AWC_SETUP; 		WIFI CONNECT STARTUP WITH STATIC 
WIFICONNECT_MOD			myWifiConnectMod 	= 	AWC_SETUP;	

										// 	AWCS_MULTI;		STA CONNECT WITH MULTIPLE SSID
										// 	AWCS_NORMAL;	STA CONNECT WITH THE SELECTED SSID
WIFICONNECTSSID_MOD 	myWifiSSIDMod 		= 	AWCS_NORMAL;	

boolean 				myWifiOTA 			= false; 	// ENABLED OTA
	
int 					myWifiConnectDone	= 0;		// WIFI CONNECT LOOP POSTION 

adri_timeNtp 			* _ntpTime;
		
adri_timer 				* _timer_disp;

adriTools_serialRead    * _serial;

		#include <ALS_espwebserver.h>
		#include <adri_espwebserver.h>
		adri_webserver	clientServer(80); 
		#ifdef ALS_USE_SOCKETSERVER
		adri_socket		socketServer(81); 
		#endif	
		ALS_espwebserver	* _webserver;



void setup()
{
	Serial.begin(115200);
	delay(1000);
	fsprintf("\n");

	SPIFFS.begin();

	String  path = "/devcices_save.txt";
	SPIFFS.remove(path);
    path = "/devcices_output.txt";
    SPIFFS.remove(path);
    

	new devicesManage();


    _serial = new adriTools_serialRead();
    _serial->cmd_array(1, 5);
    _serial->cmd_item_add(1, "menu",					"a", "", _serial_menu);
    _serial->cmd_item_add(1, "_serial_ESPreset",		"z", "", _serial_ESPreset);
    _serial->cmd_item_add(1, "outputEdit_display",		"e", "", _serial_outputEdit_display);
    _serial->cmd_item_add(1, "_serial_freeHeap",		"r", "", _serial_freeHeap);
    _serial->cmd_item_add(1, "_serial_deviceDisplay",	"t", "", _serial_deviceDisplay);
	_serial->menu();	


	_timer_disp = new adri_timer(1000,"",true);


	myWifi 		= new wifiConnect();
	myWifiAp 	= new wifi_credential_ap("");

	myWifiAp->hostname_set(ch_toString(myWifiHostname));
	wifi_credential_ap_register(myWifiAp);

	wifi_credential_sta_fromSPIFF();
	wifi_credential_set(
		0, 						
		"ssid", 		
		"pswd", 			
		"",						
		"",						
		""						
	);	
	wifi_credential_sta_toSpiff();	
	wifi_credential_sta_print();

	myWifi->load_fromSpiif 				();
	myWifi->credential_sta_pos_set 		(0);

	// configuration du lancement de la coonection
	myWifi->connect_set 				(myWifiConnectMod);
	if (myWifiConnectMod == AWC_LOOP) myWifiSSIDMod = AWCS_NORMAL; 	// wifi connect loop ne supporte pas encor de multiple ssid
	myWifi->connectSSID_set 			(myWifiSSIDMod);


	myWifi->station_set 				(WIFI_STA);
	myWifi->hostName_set 				(myWifiHostname); 			// initialisation dns si ota desactiver
	myWifi->setup_id					();							// initialize les id STA

	//
	myWifiAp->psk_set 					("mywifiappsk");						// pswd AP
	myWifiAp->ip_set 					(myWifi->_credential_sta->ip_get());	// ip 	AP
	myWifiAp->print 					();	

	if (myWifiConnectMod == AWC_SETUP) {
		myWifi->setup 						();
		if(!myWifiOTA) 	myWifi->MDSN_begin	();
		wifi_connect_statu 					();
		fsprintf("\n[myWifiConnectDone] : %s\n", on_time().c_str());
		myWifiConnectDone = 1;		
	}


	_ntpTime = new adri_timeNtp();
	_ntpTime->setup(true);

	webserver_reponse_setup();
	_webserver = new ALS_espwebserver();
	_webserver->serverFS(true);
	// initialize_webserver();
	_webserver->serverInitialize();
	// begin_webserver();
	_webserver->serverBegin();

	new udp_server();

}


void loop()
{
	myWifi->MDSN_loop();
	_ntpTime->loop();
	_serial->loop();
	udp_serverPtr_get()->udpMulti_loop();
	udp_serverPtr_get()->udp_loop();
	devicesManagePtr_get()->loop();
	_webserver->serverLoop();
}

String _serial_menu(String cmd, String value) {
    adriTools_serialReadPtr_get()->menu();
    return "";
}

String _serial_ESPreset(String cmd, String value){
    ESP.restart();
    return "";
}	

String _serial_freeHeap(String cmd, String value){
	int freeHeap = ESP.getFreeHeap();
	fsprintf("\n[freeHeap] %d", freeHeap);
	return "";
}		





