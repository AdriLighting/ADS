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
#include "device_def.h"


AsyncWebServer             server(80);
AsyncWebSocket             ws("/ws");
AsyncWebSocketClient    *  websocket_client           = NULL;
AsyncWebServerResponse  *  AsyncWebServer_response    = NULL;
AsyncWebServerRequest   *  AsyncWebServer_request     = NULL;
AsyncEventSource           events("/events");

boolean           webSocketServer_isConnected   = false;
// unsigned long     webSocketServer_lastUpd;
// unsigned long     webSocketServer_Upd;

boolean              debug_webServer               = false;
boolean              debug_httpServer              = false;
boolean              debug_webSocketServer         = false;

boolean           ota_error;
int               ota_progress;
int               loaded                        =  0;
boolean           restart_required              =  false;


void udp_send(String transmit_buffer);

// **********************************************************************  WEB SOCKET SERVER  **************************************
// void socket_send_msg(String message, int maxTime) {
//    if ((webSocketServer_isConnected) && (websocket_client!=NULL)) {
//       static unsigned long lastTime = millis();
//       char t[255];
//       unsigned long now = millis();
//       if ( now - webSocketServer_lastUpd > maxTime) {
//          if (debug_webSocketServer) {
//             unsigned long ms = millis() - lastTime;
//             seconds2time(ms, t);
//             fsprintf("\nsocket_send_msg: [%s | %s]\n", on_time().c_str(), String(t).c_str());
//             lastTime = millis();
//          }
//          // websocket_client->text(message);
//          ws.textAll(message);
//          webSocketServer_lastUpd = now;
//          delay(20);
//       }
//    }
// }
void socket_send_msg(String message, int maxTime) {
   if ((webSocketServer_isConnected) && (websocket_client!=NULL)) {
      // ws.textAll(message);
      websocket_client->text(message);
      // static unsigned long lastTime = millis();
      // unsigned long now = millis();  
      // if ( now - webSocketServer_lastUpd > maxTime) {
      //    if (debug_webSocketServer) {
      //       char t[80];
                      
      //       unsigned long ms = millis() - lastTime;
      //       seconds2time(ms, t);
      //       fsprintf("\nsocket_send_msg: [%s | %s]\n", on_time().c_str(), String(t).c_str());
      //       lastTime = millis();
      //    }
      //    // websocket_client->text(message);
      //    ws.textAll(message);
      //    webSocketServer_lastUpd = now;
      // }

   }
   else
   {
      if (debug_webSocketServer) {
         if (!webSocketServer_isConnected)   {fsprint("\nSocket msg not sent, no socket connected\n");}
         if (websocket_client==NULL) {fsprint("\nSocket msg not sent, no socket client available\n");}
      }
   }
}

String web_socket_server_msg = "";

void web_socket_server (AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
   if(type==WS_EVT_CONNECT) {
      websocket_client = client;
      if (debug_webSocketServer) {
         fsprintf("Socket client %u connected\n", client->id());
         client->printf("Socket client %u connected\n", client->id());
      }
      client->ping();
      webSocketServer_isConnected=true;
      #ifdef NO_PERIODICS
            // if necessary we ask the client to stop sending keep alive messages (helps debugging)
            socket_stop_periodics();
      #endif
      // String rep = setReponseJson(0, "", false);      
      // socket_send_msg(rep, 0);  

   }
   else if (type==WS_EVT_DISCONNECT) {
      if (debug_webSocketServer) {
         fsprintf("Socket client %u disconnected\n", client->id());
      }
      webSocketServer_isConnected=false;
   }
   else if (type==WS_EVT_DATA) {
      AwsFrameInfo * info = (AwsFrameInfo*)arg;
      String msg = "";
      if(info->final && info->index == 0 && info->len == len) {
         //the whole message is in a single frame and we got all of it's data
         if (info->opcode == WS_TEXT) {
            for(size_t i=0; i < info->len; i++)  msg += (char) data[i];
            // char socket_resp[3000];

            if (debug_webSocketServer) fsprintf("\n[ web_socket_server ] chaineCommand: %s\n", msg.c_str());
            web_socket_server_msg = String(msg);
         }
      }
   }
}



// *********************************************************************************************************************************



void sendNetworkStatus(AsyncWebServerRequest *request)
{
   // if (isAdmin(request) == false) return;
   uint8_t mac[6];
   char macStr[18] = { 0 };
   WiFi.macAddress(mac);
   sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

   String state = "N/A";
   String Networks = "";
   if (WiFi.status() == 0) state = "Idle";
   else if (WiFi.status() == 1) state = "NO SSID AVAILBLE";
   else if (WiFi.status() == 2) state = "SCAN COMPLETED";
   else if (WiFi.status() == 3) state = "CONNECTED";
   else if (WiFi.status() == 4) state = "CONNECT FAILED";
   else if (WiFi.status() == 5) state = "CONNECTION LOST";
   else if (WiFi.status() == 6) state = "DISCONNECTED";

   Networks = "";  //future to scan and show networks async

   String wifilist = "";
   wifilist += "WiFi State: " + state + "<br>";
   wifilist += "Scanned Networks <br>" + Networks + "<br>";

   String values = "<body> ";
   values += "SSID            :  " + (String)WiFi.SSID()          + "<br>";
   values += "IP Address      :  " + (String)WiFi.localIP()[0]    + "." + (String)WiFi.localIP()[1]      + "." + (String)WiFi.localIP()[2]      + "." + (String)WiFi.localIP()[3]      + "<br>";
   values += "Wifi Gateway    :  " + (String)WiFi.gatewayIP()[0]  + "." + (String)WiFi.gatewayIP()[1]    + "." + (String)WiFi.gatewayIP()[2]    + "." + (String)WiFi.gatewayIP()[3]    + "<br>";
   values += "NetMask         :  " + (String)WiFi.subnetMask()[0] + "." + (String)WiFi.subnetMask()[1]   + "." + (String)WiFi.subnetMask()[2]   + "." + (String)WiFi.subnetMask()[3]   + "<br>";
   values += "Mac Address     >  " + String(macStr) + "<br>";
   // values += "NTP Time       :   " + String(hour()) + ":" + String(minute()) + ":" + String(second()) + " " + String(year()) + "-" + String(month()) + "-" + String(day()) + "<br>";
   values += "Server Uptime   :  " + String(millis() / 60000)     + " minutes" + "<br>";
   values += "Server Heap     :  " + String(ESP.getFreeHeap())    + "<br>";
   values += "APP Test        :  TEST2 <br>";
   values += wifilist;
   values += " <input action=\"action\" type=\"button\" value=\"Back\" onclick=\"history.go(-1);\" style=\"width: 100px; height: 50px;\" /> </body> ";
   request->send(200, "text/html", values);
}


struct parms {
   uint8_t item_nbr;
   String names   [20];
   String values  [20];
};
String parms_get_arg_value(parms * data, String arg_name) {
   for (int i=0; i<data->item_nbr; i++) if (data->names[i]==arg_name) return data->values[i];
   return "";
}
// //List all parameters
// int params = request->params();
// for(int i=0;i<params;i++){
//   AsyncWebParameter* p = request->getParam(i);
//   if(p->isFile()){ //p->isPost() is also true
//     Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
//   } else if(p->isPost()){
//     Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
//   } else {
//     Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
//   }
// }


String http_server_msg = "";
unsigned long http_server_msg_start;


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

// server.addHandler(new SPIFFSEditor("",""));
      
   server.serveStatic("/", SPIFFS, "/");
   
   server.serveStatic("/ota", SPIFFS, "/ota.htm");



   // server.on("/browse",  HTTP_ANY,  [](AsyncWebServerRequest *request)  { 
   //    AsyncWebServer_request = request;
   //    http_server_msg = "handle_files_browser";      
   // });
   
   // server.on("/list",  HTTP_GET,  [](AsyncWebServerRequest *request)  { 
   //    // handle_files_list(request);
   //    AsyncWebServer_request = request;
   //    http_server_msg = "handle_files_list";
   // });

   // server.on("/getpatternsList",  HTTP_GET,  [](AsyncWebServerRequest *request)  {   
   //    String response = patternListToJSon();
   //    request->send(200, "application/json", response );
   // });



   server.onNotFound([](AsyncWebServerRequest *request) {
      not_found_server(request);          
   });

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
   
   if (http_server_msg != "") {
      // if ( http_server_msg == "handle_files_list" ) {
      //    handle_files_list(AsyncWebServer_request);
      //    http_server_msg = "";
      // }
      // else if ( http_server_msg == "handle_files_browser" ) {
      //    handle_files_browser(AsyncWebServer_request);
      //    http_server_msg = "";
      // }  
      // else if ( http_server_msg == "request_upd" ) {
      //    String rep = setReponseJson(0, "", false);
      //    AsyncWebServer_request->send(200, "application/json", rep );
      //    http_server_msg = "";
      // }          
      // else {   
      //    String *result;
      //    getCommand(http_server_msg, "request_pattern", debug_httpServer, result); 
      //    String rep = setReponseJson(0, "", false);
      //    AsyncWebServer_request->send(200, "application/json", rep );
      //    if (debug_httpServer) fsprintf("[ client_request_server END ] DURATION: %d\n", millis()-http_server_msg_start);
      //    http_server_msg = "";

      // }
   }

   if (web_socket_server_msg != "") {
      // String *result;
      // getCommand(web_socket_server_msg, "", debug_webSocketServer, result); 
      // if (debug_webSocketServer) fsprintln("[ web_socket_server END ]");
      // String rep = setReponseJson(0, "", debug_webSocketServer);
      // socket_send_msg(rep, 0);
      // rep = setReponseJson( 1,    "", false);     
      // udp_send(rep);  
      // rep = displayPatternInfo(pattern.getName(), false, 1);     
      // udp_send(rep);                   
      // web_socket_server_msg= "";
   } 
         
   if (restart_required){
      fsprint("Restarting ESP\n\r");
      restart_required = false;
      delay(1000);
      ESP.reset();
   } 

}
void ota_loop(){ArduinoOTA.handle();}


