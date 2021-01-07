
#include "config_base.h"

#ifndef CS_GETVALUE_H
	#define CS_GETVALUE_H
    #include <arduino.h>

    struct cmdApp_cs_lists {
        char    * id;
        String  (* getValue ) ();
        // void    (* setValue ) (String msg);
    } ;
    extern PROGMEM cmdApp_cs_lists cmdApp_cs_list [] ;  
#endif