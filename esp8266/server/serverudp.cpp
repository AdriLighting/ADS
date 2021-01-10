#include "serverudp.h"


#include <adri_tools.h>
#include <adri_webserver_reponselist.h>
#include <adri_webserver_reponse.h>
#include <adri_webserver_parsecommand.h>

#include "devices.h"

udp_server * udp_serverPtr;
udp_server * udp_serverPtr_get(){return udp_serverPtr;}

udp_server::udp_server(){
	udp_serverPtr = this;
	udpServer 		= new adri_udp();
	udp_setup(9100);

	udpMultiServer 	= new adri_udpMulti();
	udpMulti_setup(9200);
}

void udp_server::udp_setup(uint32_t port){
	if (udpServer==nullptr) return;
	fsprintf("\n[udp_setup]\n");
	_udp_port = port;
	udpServer->set_port(port);
	udpServer->parse_set_func(devicesManage_udpParse);
	udpServer->start();
}
void udp_server::udp_loop(){
	if (udpServer==nullptr) return;
	udpServer->loop();
}
void udp_server::udp_send(String msg){
	if (udpServer==nullptr) return;
	udpServer->send(msg);
	unsigned int msg_length = msg.length();
	Serial.printf("\n[udp_send] size: %d\n%s\n",msg_length, msg.c_str());

}

void udp_server::udpMulti_setup(uint32_t port){
	if (udpMultiServer==nullptr) return;
	fsprintf("\n[udpMulti_setup]\n");
	_udpMulti_port = port;
	udpMultiServer->set_port(port);
	udpMultiServer->parse_set_func(devicesManage_udpMultiParse);
	udpMultiServer->start();
}
void udp_server::udpMulti_loop(){
	if (udpMultiServer==nullptr) return;
	udpMultiServer->loop();
}
void udp_server::udpMulti_send(String msg){
	if (udpMultiServer==nullptr) return;
    msg = literal_item("server_send", "1") + msg;
	udpMultiServer->send(msg);
	unsigned int msg_length = msg.length();
	fsprintf("\n[udpMulti_send] size: %d\n%s\n",msg_length, msg.c_str());
}




void parse_screen_initialize(String url){
	String cmd 		= "";
	String value 	= "";	
	String rep 		= "";	

	// fsprintf("\n[parse_screen_initialize]\n\t%s\n",url.c_str());
	// adriTools_serialReadPtr_get()->splitText(url, "&", cmd,  value) ;
	// int pos = value.toInt();
	// if (cmd == "colorpicker") {
	// 		ALS_lampUpdateClientPtr_get()->android_generalSetting(rep, "");
	// 		ALS_udpServer_get()->udp_send(rep);
	// 		ALS_udpServer_get()->udpMulti_send(rep);
	// 		return;		
	// }
	// switch(pos) {
	// 	case 0:
	// 		ALS_lampUpdateClientPtr_get()->android_generalSetting(rep, "");
	// 		ALS_udpServer_get()->udp_send(rep);
	// 		ALS_udpServer_get()->udpMulti_send(rep);
	// 	break;

	// 	case 1: 
	// 		ALS_lampUpdateClientPtr_get()->android_generalSetting(rep, "");
	// 		ALS_udpServer_get()->udp_send(rep);
	// 		ALS_udpServer_get()->udpMulti_send(rep);
	// 	break;
	// }
}
