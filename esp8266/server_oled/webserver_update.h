#include "config_base.h"


#ifndef WEBSERVER_UPDATE_H
	#define WEBSERVER_UPDATE_H
    #include <arduino.h>
    #include <FS.h>   

    String  appi_rep_loop(String op);
    String  appi_rep_2_loop(String op);

    String 	webserverSend_string(String str, String name);
    String 	webserverSend_list_devices();
    String 	webserverSend_list_devicesOutput();
    void 	webserverSend_list_devicesOutput_toSocket();
    void    webserverSend_list_group_toSocket();
    void    webserverSend_selected_devicesOutput_toSocket();
    String  webserverSend_list_devicesOn();
    
    class webSocketUpdate {
    public: 
        String name;
        boolean isConnected;
        webSocketUpdate(String add);
    };
    extern int webSocketUpdate_array_nbr;
    extern webSocketUpdate * webSocketUpdate_array[];   
     
    void 	webSocketUpdate_loop();


      enum request_reponse_fileTypes { rft_number, rft_string}; 
   
      struct requestReponseLists {
            char *                        id;
            request_reponse_fileTypes     type;
            String                        (* getValue )     ();   

      } ;
      String setReponseJson(int mod, String transmit_request, boolean display);
          
#endif