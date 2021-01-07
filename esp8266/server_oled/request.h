
#include "config_base.h"

#ifndef REQUEST_H
	#define REQUEST_H
    #include <arduino.h>

    struct cmdApp_req_lists {
        char	* name;
        int  	(* cmd ) (String msg);
        // void    (* setValue ) (String msg);
    } ;
    extern PROGMEM cmdApp_req_lists cmdApp_req_list [] ;  

    int getCommand(String A_readString, String request_mod, boolean display);
    
#endif