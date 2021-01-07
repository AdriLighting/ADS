// #include <ESP8266WiFi.h>

#ifndef ADRIWIFI_H
    #define ADRIWIFI_H

#ifdef ESP8266
   #include <ESP8266WiFi.h>
    #include <ESP8266mDNS.h>
#endif 

void _wfifi_connect(int mod,
    char* ssid, char* password,
    IPAddress staticIP, String hostName, 
    boolean savId, boolean cfgIp);

String network_info(String hostName);
String network_info_short(String hostName);

int wfifi_getID_fromSPIFF() ;
int wfifi_getID_toSPIFF() ;
struct wifi_id { // 1AP
    String  subnet;
    String  gateway;
    String  ip;
    String  pswd;
    String  ssid;
    String  ap_ip;
    String  ap_psk;
    String  ap_ssid;
    int     mod;
};

struct wifi_id_getValue {
    char * name;
    String (*func_1)();
    int mod;
} ;
extern PROGMEM wifi_id_getValue wifi_id_getValue_list [];
extern int wifi_id_getValue_count;

boolean telnet_put  (char *     output_string);
boolean telnet_put  (String *   output_string);
boolean telnet_get  (char *     input_string);
void    telnet_setup();
void    telnet_loop ();
void    remote_print(String * s);
void    remote_print(char * s);

enum    dm {none, local, remote, both};
extern  dm debug_mode;

void debug(const String * s);
void debug(String * s);
void debug(char * buf);

void local_print(String * s);
void local_print(char * s);
#endif

