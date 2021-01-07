#include "config_base.h"

#ifdef ESP8266
	#include <ESP8266WiFi.h>
	#include <ESP8266mDNS.h>
#endif 

#include <ArduinoOTA.h>

#include <FS.h>   
#ifdef ESP32
	#include <SPIFFS.h>
#endif  

#include <TimeLib.h>

#include "Hash.h"

// #include <ESPAsyncTCP.h>   
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>

#include "serverota.h"

#include "tools.h"

#include "webserver_update.h"

#include "adriwifi.h"

#include "udp.h"

#include "colors.h"

#include "request.h"


#ifdef OLED_UPD_WEBSOCKET
	extern oled_requests oled_request;
#endif
	
AsyncWebServer             server(80);


#ifdef ESP_WEBSOCKETSERVER
	AsyncWebSocket             ws("/ws");
	AsyncWebSocketClient    *  websocket_client           = NULL;
#endif
AsyncEventSource           events("/events");
boolean 	webSocketServer_isConnected   = false;
boolean 	debug_webSocketServer         = false;
int 		loaded                        =  0;
boolean 	restart_required              =  false;
boolean 	ota_error;
int 		ota_progress;

void 	udp_send(String transmit_buffer);
String 	device_setOp(String dn) ;

// **********************************************************************  WEB SOCKET SERVER  **************************************
#ifdef ESP_WEBSOCKETSERVER

	void socket_send_msg(String message, int maxTime) {
		if ((webSocketServer_isConnected) && (websocket_client!=NULL)) {
			websocket_client->text(message);
		}
		else
		{
			#ifdef WEBSOCKET_DEBUG
				if (debug_webSocketServer) {
					if (!webSocketServer_isConnected)   {fsprint("\nSocket msg not sent, no socket connected\n");}
					if (websocket_client==NULL) {fsprint("\nSocket msg not sent, no socket client available\n");}
				}
			#endif
		}
	}
	boolean socket_isConnected() {
		if ((webSocketServer_isConnected) && (websocket_client!=NULL)) {
			return true;
		}
		return false;
	}
	
	String web_socket_server_msg = "";


	void web_socket_server (AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
		if(type==WS_EVT_CONNECT) {
			websocket_client = client;
			#ifdef WEBSOCKET_DEBUG
				if (debug_webSocketServer) {
					fsprintf("Socket client %u connected\n", client->id());
				}
			#endif
			client->ping();
			webSocketServer_isConnected=true;
			web_socket_server_msg = "init_webpage";		
		}
		else if (type==WS_EVT_DISCONNECT) {
			#ifdef WEBSOCKET_DEBUG
				if (debug_webSocketServer) {
					fsprintf("Socket client %u disconnected\n", client->id());
				}
			#endif
			webSocketServer_isConnected=false;
		}
		else if (type==WS_EVT_DATA) {
			AwsFrameInfo * info = (AwsFrameInfo*)arg;
			String msg = "";
			if(info->final && info->index == 0 && info->len == len) {
				//the whole message is in a single frame and we got all of it's data
				if (info->opcode == WS_TEXT) {
					for(size_t i=0; i < info->len; i++)  msg += (char) data[i];
					#ifdef WEBSOCKET_DEBUG	
						if (debug_webSocketServer) fsprintf("\n[ web_socket_server ] chaineCommand: %s\n", msg.c_str());
					#endif
					web_socket_server_msg = String(msg);
				}
			}
		}
	}

#endif
// *********************************************************************************************************************************


// **********************************************************************  OTA SETUP   

void serve_ota_progress       (char *   result) {
	sprintf(result, "Mise à jour en cours, %d ko téléchargés\n", ota_progress);
	Serial.println(ota_progress);
}

#ifdef OTA_DEBUG
void user_message(String s) {Serial.print(s);}
#endif
void ota_server(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, boolean final) {
		static int nbd=0;
		uint32_t max_sketch_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000; // retrieve free flash size
		if (!index) {                             // if index == 0 this is the first frame of data
			#ifdef OTA_DEBUG
				Serial.print("Demarrage de la mise a jour: "+filename+" ");
			#endif
			if (filename.indexOf(".bin")==-1) {          // we check that a valid filename was entered
				#ifdef OTA_DEBUG
					Serial.printf("\nErreur %s n'est pas un fichier téléchargeable\n", filename.c_str());
				#endif
				events.send("OTA_FILENAME_ERROR", "add");
				Update.end(false);                     // false means that we want to abort OTA
				ota_error=true;
				return;
			}
			events.send("OTA_START", "add");
			ota_error=!Update.begin(max_sketch_space);      // open the update object
			Update.runAsync(true);                    // tell the updaterClass to run in async mode
		}
		if (!ota_error) {                         // if everything goes well..
			if ((index+len)<max_sketch_space) {          // first check if we have enough space..
				Update.write(data, len);               // ..yes, write the data packet received
				loaded = Update.progress();
				//Serial.printf("filename:%s loaded:%d final:%d\n", filename.c_str(), loaded, final);
				if (nbd++%10==0) Serial.print(".");
				if (final) {                        // ..and, if it is the last frame of data..
					if((Update.end(true))&&(!ota_error)){  // ..close the update (true) to perform flash copy at reboot
						restart_required=true;           // do not forget signalling we will need a reboot
						#ifdef OTA_DEBUG
							user_message("\nMise a jour effectuee\n");
						#endif
						events.send("OTA_SUCCESS", "add");
					} else {
						#ifdef OTA_DEBUG
							user_message("\nMise à jour erreur");
						#endif
						events.send("OTA_ERROR", "add");
					}
				}
			}
			else
			{                                   // no more flash left, stop everything or we will lose SPIFFS content
				Update.end(false);                     // false means that we want to abort OTA
				ota_error=true;
			}
		}
	}

void ota_end(AsyncWebServerRequest * request) {
		// the request handler is triggered after the upload has finished...
		// create the response, add header, and send response
		fsprintln("Fermeture de la connexion");
		AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", (ota_error)?"OTA_UPDATE FAIL":"OTA UPDATE FINISHED");
		response->addHeader("Connection", "close");
		response->addHeader("Access-Control-Allow-Origin", "*");
		request->send(response);
}

void not_found_server(AsyncWebServerRequest * request){
		fsprint("\n[not_found_server] : ");
		if(request->method() == HTTP_GET)            fsprint("GET");
		else if(request->method() == HTTP_POST)         fsprint("POST");
		else if(request->method() == HTTP_DELETE)       fsprint("DELETE");
		else if(request->method() == HTTP_PUT)          fsprint("PUT");
		else if(request->method() == HTTP_PATCH)        fsprint("PATCH");
		else if(request->method() == HTTP_HEAD)         fsprint("HEAD");
		else if(request->method() == HTTP_OPTIONS)      fsprint("OPTIONS");
		else                                fsprint("UNKNOWN");
		fsprintf("http://%s%s\n", request->host().c_str(), request->url().c_str());
		char err[100];
		fssprintf(err, "Error: 'http://%s%s' was not found\n", request->host().c_str(), request->url().c_str());
		request->send(404, "text/html",  String(err));
}

struct parms {
   uint8_t item_nbr;
   String names   [20];
   String values  [20];
};
void request_command(AsyncWebServerRequest *request) {

   // if (debug_httpServer) fsprintf("\n[ client_request_server ] chaineCommand: %s\n", http_server_msg.c_str());

   parms data;
   char buf[1024];
   int params = request->params();
   data.item_nbr=params;
   String s= "";
   for (int i=0;i<params; i++) {
      data.names[i]  = request->getParam(i)->name();
      data.values[i] = request->getParam(i)->value();
      if (data.names[i] != ""){
         sprintf(buf," &%s=%s ", data.names[i].c_str(), data.values[i].c_str());
         s+=String(buf);
      }
   }
   int c = getCommand(s, "request_pattern", true); 


}

// **********************************************************************  SERVER SETUP   
void server_setup(){

	#ifdef OTA_ENABLE
		ArduinoOTA.onStart([]() { Serial.println("Update Start"); });
		ArduinoOTA.onEnd([]() { Serial.println("Update End"); });
		ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
			char p[32];
			sprintf(p, "Progress: %u%%\n", (progress/(total/100)));
			Serial.println(p);
		});
		ArduinoOTA.onError([](ota_error_t error) {
			if(error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
			else if(error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
			else if(error == OTA_CONNECT_ERROR) Serial.println("onnect Failed");
			else if(error == OTA_RECEIVE_ERROR) Serial.println("Recieve Failed");
			else if(error == OTA_END_ERROR) Serial.println("End Failed");
		});
		ArduinoOTA.setHostname(DEVICENAME.c_str());
		ArduinoOTA.begin();
	#endif

	#ifdef ESP_WEBSOCKETSERVER
		ws.onEvent(web_socket_server);
		server.addHandler(&ws);     
	#endif

	events.onConnect([](AsyncEventSourceClient *client){
		// client->send("hello!",NULL,millis(),1000);
	});
	server.addHandler(&events);

	server.addHandler(new SPIFFSEditor("",""));
		
	server.serveStatic("/", SPIFFS, "/");
	server.serveStatic("js/jscolor.js", SPIFFS, "js//jscolor.js");
	server.serveStatic("js//core.js", SPIFFS, "js//core.js");

	server.on("/request_pattern",  HTTP_GET,  [](AsyncWebServerRequest *request)  { 
		request_command(request);
		String rep = setReponseJson(0, "", false);
		request->send(200, "application/json", rep );
	});
   	server.on("/request_upd",  HTTP_GET,  [](AsyncWebServerRequest *request)  {   
      	String rep = setReponseJson(0, "", false);
      	request->send(200, "application/json", rep );

   	});	
	// server.onNotFound([](AsyncWebServerRequest *request) {
	//    not_found_server(request);          
	// });

	//******************************** // POST request 
	server.on("/start_ota", HTTP_POST, 
		[](AsyncWebServerRequest *request) {
		  ota_end(request);
		},
		[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, boolean final){
		  ota_server(request, filename, index, data, len, final);
		}
	);

	// ********************************* UPLOAD
	server.onFileUpload([](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, boolean final) {
		if(!index)  fsprintf("UploadStart: %s\n", filename.c_str());
		fsprintf("%s", (const char*)data);
		if(final)   fsprintf("UploadEnd: %s (%u)\n", filename.c_str(), index+len);
	});

	server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		if(!index)                 fsprintf("BodyStart: %u\n", total);
		fsprintf("%s", (const char*)data);
		if(index + len == total)   fsprintf("BodyEnd: %u\n", total);
	});

	server.begin();
}
// *********************************************************************************************************************************

void server_loop() {
	
	#ifdef ESP_WEBSOCKETSERVER
		if ((web_socket_server_msg != "") && (!oled_request.send)) {

			if (web_socket_server_msg == "init_webpage"){
				webserverSend_list_devicesOutput_toSocket();
				webserverSend_list_group_toSocket();
				web_socket_server_msg= "";
				return;			
			}		

						
				String cmd;
				String dn;
				String op;
					cmd = literal_value("msg", 	web_socket_server_msg);
					dn 	= literal_value("dn", 	web_socket_server_msg);
					op 	= device_setOp(dn);
					if (op == "") return;
					String 	s2 = "";
							s2 = 	literal_item("dn", 	dn);
							s2 += 	literal_item("op", 	op);
							s2 += 	literal_item("msg",	cmd);	
							oled_request_clear();	
							oled_request.device[oled_request.cnt][0] = dn;
							oled_request.device[oled_request.cnt][1] = "";
							oled_request.cnt++;	
							oled_request_start();										
							udpMulti_send(s2) ;  

				web_socket_server_msg= "";
		} 
	#endif
			
	if (restart_required){
		fsprint("Restarting ESP\n\r");
		restart_required = false;
		delay(1000);
		ESP.reset();
	} 

}
void ota_loop(){ArduinoOTA.handle();}


