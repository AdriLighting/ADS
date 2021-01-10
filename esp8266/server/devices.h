#ifndef DEVICES_H
#define DEVICES_H 
#include "include.h"
#include "serverudp.h"
#include <ArduinoJson.h>
#include <adri_timer.h>



        struct oled_requests {
            int cnt;
            boolean send;
            String device[MAX_DEVICES][2];

        };  
        class oledRequest
        {
        public:
        	oledRequest();
        	~oledRequest();

        	oled_requests oled_request;
	        int     search_device(String search);
	        void    upd_device(String search);
	        boolean canSend();
	        void    start();
	        void    clear();
	        String  sendString();
	        void    end();
	        int     cnt();        	
        	
        };
        oledRequest * oledRequestPtrGet();

class appiUdp
{

	oledRequest * _oledRequest;
public:
	

	appiUdp();
	~appiUdp();
	boolean     udp_wait_for_hsvLoop    = false;
	boolean     udp_appi_rep            = false;	

	String 	device_setOp(String dn);
	char * 	device_set_req(char * cmd);
	String 	device_setVal(String dn, char * cmd, String val, int pos);
	String 	device_setVal(char * cmd, String val);
	String 	c_to_string(char * name);
	String 	request_set_format(char * cmd, String value, String e, String s);
	String 	request_set_format(char * cmd, String e, String s);
	void 	appi_send_udpRequest_dn(String dn, char * req, String val, String upd);
	void 	appi_send_udpRequest_mdn(String val, char * req, String dg, String upd);
	boolean	request_isTrueWhite(char * req);
	char *	request_getTrueWhite(char * req);

};
appiUdp * appiUdpPtrGet();

class devicesManage
{
	output_edit 	* _output_edit;
	devices_edit 	* _devices_edit;
	devicesspiff 	* _devices_spiff;
	devices 		* _devices;
	oledRequest 	* _oledRequest;
	appiUdp 		* _appiUdp;
	adri_timer 		* _timer_upd;
	
	public:
		devicesManage();
		~devicesManage();

		void devices_print();
		void outputEdit_print();

		void udpMultiParse(String req);
		void udpParse(String req);

		void loop();

		void list_devicesOutput_jsonFile();
		void list_devicesOutput_jsonFile(String & object);
		void list_devicesOutput(JsonObject & object);
		String list_devicesOutput();
		String list_devices();
		String list_group();

		String appi_rep_loop(String op);
	
};
devicesManage * devicesManagePtr_get();
void devicesManage_udpMultiParse(String req);
void devicesManage_udpParse(String udp_msg);
String _serial_deviceDisplay(String cmd, String value);
String _serial_outputEdit_display(String cmd, String value);


void webserver_reponse_setup();
#endif