#include "config_base.h"

#ifdef WEBSERVER_ENABLE

#include "webserver_request.h"
#include "webserver_update.h"
#include "devices.h"
#include "tools.h"

    String device_setOp(String dn);
    char * device_set_req(char * cmd);
    String device_setVal(String dn, char * cmd, String val, int pos);
    String device_setVal(char * cmd, String val);
    String c_to_string(char * name);
    String request_set_format(char * cmd, String value, String e, String s);
    String request_set_format(char * cmd, String e, String s);




#endif