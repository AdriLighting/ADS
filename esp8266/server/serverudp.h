#ifndef SERVERUDP_H
#define SERVERUDP_H
#include <adri_udp.h>

class udp_server
{
		adri_udp		* udpServer			= nullptr;
		adri_udpMulti	* udpMultiServer	= nullptr;
		uint32_t 		_udp_port			= 9100;
		uint32_t 		_udpMulti_port		= 9200;	
public:
	udp_server();
	~udp_server();

	void udp_setup(uint32_t port);
	void udp_loop();
	void udp_send(String msg);

	void udpMulti_setup(uint32_t port);
	void udpMulti_loop();
	void udpMulti_send(String msg);
};
udp_server * udp_serverPtr_get();



#endif