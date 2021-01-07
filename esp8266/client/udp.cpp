#include <ESP8266WiFi.h>
#include <udp.h>
#include <WiFiUdp.h>

#include "config_base.h"
#include "udp.h"
#include "tools.h"
#include "adriwifi.h"
#include "devices.h"
#include "output_func.h"
#include "adriwifi.h"

#include "request.h"


int 	udp_port 			= 9100;
bool 	debug_udp			= true;
WiFiUDP udp_server;
String 	udp_receive_buffer;

IPAddress   udp_multicast_ip    	= {239, 0, 0, 57};
int         udp_multicast_port 		= 9200;
WiFiUDP 	udp_multi;
String 		udpMulti_receive_buffer;

unsigned long 	timer_udp_lastchange;
int 			timer_udp_max = 1500;

void request_preset_sav(String msg){
	String 	preset = literal_value( "preset",  msg);
	String aVal = value_sav_toStringt(true);
	value_savToSPIFF(true, "/preset_" + preset + ".txt", aVal);		
}
void request_preset_load(String msg){
		String 	preset = literal_value( "preset",  msg);
		String aVal = value_loadFromSPIFF(false,"/preset_" + preset + ".txt");
		if (aVal != "") {
			value_load_fromString(true, aVal);
			output_preset_load(true);
		}	
}

int request_cmd(String msg){

	String 	op;
	String 	dn;
	String 	uMsg;

	String 	buff[120];

	int 	cnt;
	int 	cmdCnt = 0;


	if(msg.indexOf("#") >= 0) {

		cnt = explode(msg, '#', buff);

		String s = "";

		for( int i = 0; i < cnt ; i++) {
			dn = 	literal_value("dn", buff[i]);
			if (dn == device_name) {
				s = buff[i];
				break;
			}
		}

		if (s != ""){

						op 				= literal_value("op", s);
						uMsg 			= literal_value("msg", s);			
		    byte        firstIndex      = uMsg.indexOf('=');
		    byte        secondIndex     = uMsg.indexOf('=', firstIndex + 1);
		    String      cmd             = uMsg.substring(0, firstIndex);
		    String 		val 			= uMsg;

		    val.replace(cmd+"=", "");

			if(val.indexOf("*") >= 0) {

				cnt = explode(val, '*', buff);

				for( int i = 0; i < cnt ; i++) {

					if ( buff[i] == "" ) continue;

					String send = cmd + "=" + buff[i];
					int c 		= getCommand(send, "request_pattern", debug_udp);
					cmdCnt 		+= c; 
				}
			}
		}
	} else {

		dn = 	literal_value("dn", msg);
		op = 	literal_value("op", msg);
		uMsg = 	literal_value("msg", msg);

		if (dn == device_name) {
			if (op == "lp")	{
				int c = getCommand(uMsg, "request_pattern", debug_udp);
				cmdCnt += c;
			}
		}				
	}

	return cmdCnt;
}

void udpMulti_parse(String * udp_msg) { 

	String 	msg 	= *udp_msg;	

	String 	server = literal_value( "server_send",  msg);
	if (server != "1") return;

	// Serial.printf("\n%s\n", msg.c_str());
	// debug(&msg);

	String 	op 		= literal_value( "op",  msg);
	String 	dn 		= literal_value( "dn",  msg);

	if (op == "preset_sav")	{
		request_preset_sav(msg); 	
		return;
	}
	if (op == "preset_load") {
		request_preset_load(msg); 	
		return;
	}
	if (op == "get_rep") {
		if (dn == device_name) {
	    	String rep = send_rep();
	    	udpMulti_send(rep);
			return;			
		}
	}

	int cmdCnt = request_cmd(msg);
	if (cmdCnt > 0) {

	    String aVal = value_sav_toStringt(true);
	    value_savToSPIFF(true, "/" + FILENAME_SAVVALUE + ".txt", aVal);		

	    String rep = send_rep();
	    udpMulti_send(rep);

	    timer_udp_lastchange = millis();
	}

}

bool udpMulti_receive(String * receive_buffer) {
   int packet_size = udp_multi.parsePacket();
   if (!packet_size) return false;
   char buf[1024];
   int length = udp_multi.read(buf, 1024);
   if (length>0) buf[length]=0;
   * receive_buffer = String(buf);
   return true;
}	
void udpMulti_send(String transmit_buffer) {
    int size = transmit_buffer.length();
	char buf[size+1];
	sprintf(buf, "%s", 
		transmit_buffer.c_str()
	);	
	udp_multi.beginPacketMulticast(udp_multicast_ip, udp_multicast_port, WiFi.localIP());
	udp_multi.write(buf);
	udp_multi.endPacket();	
}		
bool udp_receive(String * receive_buffer) {
   int packet_size = udp_server.parsePacket();
   if (!packet_size) return false;
   char buf[1024];
   int length = udp_server.read(buf, 1024);
   if (length>0) buf[length]=0;
   * receive_buffer = String(buf);
   return true;
}
void udp_send(String transmit_buffer) {
    int size = transmit_buffer.length();
	char buf[size+1];
	sprintf(buf, "%s", 
		transmit_buffer.c_str()
	);	
	udp_server.beginPacket(udp_server.remoteIP(), udp_server.remotePort());
	udp_server.write(buf);
	udp_server.endPacket();	
}
void udp_parse(String * udp_msg) { 
	String msg = *udp_msg;	
}


bool udp_loop() {
	// if (udp_receive(&udp_receive_buffer)) 			{udp_parse(&udp_receive_buffer);}
	if (udpMulti_receive(&udpMulti_receive_buffer))	{udpMulti_parse(&udpMulti_receive_buffer);}

	long now = millis();
	if ( (now - timer_udp_lastchange) > (timer_udp_max) ) {  
		timer_udp_lastchange = now;
		String s = send_device();
		udpMulti_send(s);
	}		
	
}

void udp_start(){
	udp_server.begin(udp_port);
	udp_multi.beginMulticast(WiFi.localIP(), udp_multicast_ip, udp_multicast_port);
}


