#ifndef SERVEROTA_H
	#define SERVEROTA_H
	#include <ESPAsyncWebServer.h>

	extern boolean webSocketServer_isConnected;
	extern boolean debug_webServer;
	extern boolean debug_httpServer;
	extern boolean debug_webSocketServer ;   
	extern String web_socket_server_msg ; 
	extern AsyncEventSource events;  
	extern boolean restart_required;
	void server_setup();
	void server_loop();
	void ota_loop();
	void socket_send_msg(String message, int maxTime);  
	void not_found_server(AsyncWebServerRequest * request);
#endif
