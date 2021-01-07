#ifdef ESP8266
   #include <ESP8266WiFi.h>
    #include <ESP8266mDNS.h>
#endif 

#include <arduino.h>
#include "config_base.h" 
#include "device_def.h" //
#include "tools.h"
#include "adriwifi.h"
#include "serverota.h"

#define DEBUG
    
wifi_id wifi_id;




#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


String wifi_id_get_subnet()     {return wifi_id.subnet;}
String wifi_id_get_gateway()    {return wifi_id.gateway;}
String wifi_id_get_ip()         {return wifi_id.ip;}
String wifi_id_get_pswd()       {return wifi_id.pswd;}
String wifi_id_get_ssid()       {return wifi_id.ssid;}
String wifi_id_get_ap_ip()      {return wifi_id.ap_ip;}
String wifi_id_get_ap_psk()     {return wifi_id.ap_psk;}
String wifi_id_get_ap_ssid()    {return wifi_id.ap_ssid;}
String wifi_id_get_mod()        {return String(wifi_id.mod);}
PROGMEM wifi_id_getValue wifi_id_getValue_list [] = {           
    {"ssid",    &wifi_id_get_ssid,      0},
    {"pswd",    &wifi_id_get_pswd,      0},
    {"ip",      &wifi_id_get_ip,        0},
    {"ap_ssid", &wifi_id_get_ap_ssid,   1},
    {"ap_psk",  &wifi_id_get_ap_psk,    1},
    {"ap_ip",   &wifi_id_get_ap_ip,     1},
    {"subnet",  &wifi_id_get_subnet,    2},
    {"gateway", &wifi_id_get_gateway,   2},
    {"mod",     &wifi_id_get_mod,       2},

};
int wifi_id_getValue_count = ARRAY_SIZE(wifi_id_getValue_list);

char b_host_name[40];
void build_host_name() {
   unsigned char mac[6];
   WiFi.macAddress(mac);
   strcpy(b_host_name, HOST_NAME_PREFIX);
   for (int i=4; i<6; i++) {char b[2]; sprintf(b,"%02x",mac[i]); strcat(b_host_name, b); }
}


String network_info(String hostName) {
    String s;
    s+="\nRESEAUX\n";
    fs(sys_mode,            "mode");
    fs(sys_connecter,       "connecter a");
    fs(sys_ip,              "ip");
    fs(sys_mdp,             "mdp");
    fs(sys_dns,             "nom dns");
    fs(sys_webserver,       "webserver");
    fs(sys_webserverName,   "ESPAsyncWebServer");
    fs(sys_ota,             "ota");
    fs(sys_websocket,       "websocket");
    fs(sys_activer,         "activer");
    fs(sys_desactiver,      "desactiver");
    fs(sys_subnet,          "gateway");
    fs(sys_gateway,         "subnet");

    if (wifi_id.mod == 0) {
        s+= info_parm(fsget(sys_mode),      "station"); 
        s+= info_parm(fsget(sys_connecter), wifi_id.ssid);
        s+= info_parm(fsget(sys_ip),        wifi_id.ip);
        s+= info_parm(fsget(sys_mdp),       wifi_id.pswd);
        s+= info_parm(fsget(sys_subnet),    wifi_id.subnet);
        s+= info_parm(fsget(sys_gateway),   wifi_id.gateway);
        s+= info_parm(fsget(sys_dns),       hostName+".local");
    }
    if (wifi_id.mod == 1) {
        s+= info_parm(fsget(sys_mode),      "AP"); 
        s+= info_parm(fsget(sys_connecter), wifi_id.ap_ssid);
        s+= info_parm(fsget(sys_ip),        wifi_id.ap_ip);
        s+= info_parm(fsget(sys_mdp),       wifi_id.ap_psk);
        s+= info_parm(fsget(sys_dns),       hostName+".local");
    }   

    s+=info_parm(fsget(sys_webserver),  fsget(sys_webserverName));
    #ifdef OTA_ENABLE
    s+=info_parm(fsget(sys_ota),        fsget(sys_activer));
    #else
    s+=info_parm(fsget(sys_ota),        fsget(sys_desactiver));
    #endif        
    #ifdef ESP_WEBSOCKETSERVER
    s+=info_parm(fsget(sys_websocket),  fsget(sys_activer));
    #else
    s+=info_parm(fsget(sys_websocket),  fsget(sys_desactiver));
    #endif      
    #ifdef ESP8266
    // available_networks_text(&s);
    #endif
    // s+="\n"; registered_networks_text(&s);
    return s;
}
String network_info_short(String hostName) {
    String s;
    s+="\nRESEAUX\n";
    fs(sys_mode,            "mode");
    fs(sys_connecter,       "connecter a");
    fs(sys_ip,              "ip");
    fs(sys_mdp,             "mdp");
    fs(sys_dns,             "nom dns");
    fs(sys_subnet,          "gateway");
    fs(sys_gateway,         "subnet");

    if (wifi_id.mod == 0) {
        s+= info_print(fsget(sys_mode),      "station"); 
        s+= info_print(fsget(sys_connecter), wifi_id.ssid);
        s+= info_print(fsget(sys_ip),        wifi_id.ip);
        s+= info_print(fsget(sys_mdp),       wifi_id.pswd);
        s+= info_print(fsget(sys_subnet),    wifi_id.subnet);
        s+= info_print(fsget(sys_gateway),   wifi_id.gateway);
        s+= info_print(fsget(sys_dns),       hostName+".local");
    }
    if (wifi_id.mod == 1) {
        s+= info_print(fsget(sys_mode),      "AP"); 
        s+= info_print(fsget(sys_connecter), wifi_id.ap_ssid);
        s+= info_print(fsget(sys_ip),        wifi_id.ap_ip);
        s+= info_print(fsget(sys_mdp),       wifi_id.ap_psk);
        s+= info_print(fsget(sys_dns),       hostName+".local");
    }   

    return s;
}

int wfifi_getID_fromSPIFF() { 
    String filename = "/wifi_id.txt";
    File f = SPIFFS.open(filename,"r");

    #ifdef DEBUG
    Serial.printf("\n[wfifi_getID_fromSPIFF]");
    #endif

    if (!f) {
        #ifdef DEBUG
         Serial.printf(" ERROR read file:/wifi_id.txt\n"); 
        #endif
        wifi_id.mod = 1;
    } else {

        String line;

        while (f.available()) {line += char(f.read());}
        f.close();

        wifi_id.ip        = literal_value("wifi_ip",       line );
        wifi_id.pswd      = literal_value("wifi_psk",      line );
        wifi_id.ssid      = literal_value("wifi_ssid",     line );
        wifi_id.ap_ip     = literal_value("wifiAp_ip",     line );
        wifi_id.subnet    = literal_value("wifi_subnet",   line );
        wifi_id.gateway   = literal_value("wifi_gateway",  line );

        if (literal_value("wifi_mod", line ) == "") wifi_id.mod == 1;
        else wifi_id.mod    = literal_value("wifi_mod", line ).toInt();

        #ifdef DEBUG
        Serial.printf(" SUCCES read file:/wifi_id.txt\n"); 
        #endif        
    }
     
}
int wfifi_getID_toSPIFF() { 

    String filename = "/wifi_id.txt";

    File f = SPIFFS.open(filename,"w");
    if (!f) {  
        #ifdef DEBUG
        Serial.printf(" ERROR read file:/pswd.txt\n"); 
        #endif

        f.close(); 
    }
    else {

        String line;
        line += literal_item(   "wifi_ip",        wifi_id.ip        );
        line += literal_item(   "wifi_psk",       wifi_id.pswd      );
        line += literal_item(   "wifi_ssid",      wifi_id.ssid      );
        line += literal_item(   "wifiAp_ip",      wifi_id.ap_ip     );
        line += literal_item(   "wifi_subnet",    wifi_id.subnet    );
        line += literal_item(   "wifi_gateway",   wifi_id.gateway   );
        line += literal_item(   "wifi_mod",       String(wifi_id.mod)   );

        f.print(line);
        f.close();     

        #ifdef DEBUG
        Serial.printf(" ERROR write file:/wifi_id.txt\n"); 
        #endif

    } 

}

int wfifi_getID(char* ssid, char* password) {
    #ifdef DEBUG
    Serial.printf("\n[wfifi_connect_getSSID]");
    #endif
    
    String line;      
    uint8_t error = 0;

    if ((wifi_id.pswd == "") || (wifi_id.ssid == "")) {
        #ifdef DEBUG
        Serial.printf(" ERROR reading WIFIID\n");
        #endif
        error=1;
    }
    else {

        String sPSWD = wifi_id.pswd;
        String sSSID = wifi_id.ssid;
        if (sPSWD == "") error=2;
        if (sSSID == "") error=3;

        if ( error == 0) {
            sprintf(ssid,     "%s",    sSSID.c_str());
            sprintf(password, "%s",    sPSWD.c_str());
            Serial.printf("\tssid: %s - pswd: %s\n", ssid,  password);
        } 

        #ifdef DEBUG
        if (error > 0) {
            Serial.printf("\tERROR pswd or ssid not found\n");
        }
        #endif


    } 
    return error;  
}

boolean wfifi_retIP(String sIp, IPAddress & retIp, String & retSip) {

    Serial.printf("\n[wfifi_retIP]");

    String line;  
    boolean spiffIp = false;


    if (sIp == "") {
        #ifdef DEBUG
        Serial.printf(" ERROR reading WIFI IP from wifiIp.txt\n");
        #endif
    }
    else {
        #ifdef DEBUG
        Serial.printf(" SUCCES reading file\n");
        #endif
        String ip = sIp;
        if (ip != "") {
            IPAddress SpiffIp = string2ip(ip);
            #ifdef DEBUG
            Serial.printf("\twfifi_retIP: %s\n", ip.c_str());
            #endif
            spiffIp = true;  
            retIp = SpiffIp; 
            retSip = ip;           
        }
    }
    return spiffIp;

}

boolean wfifi_connect_station(char* ssid, char* password, IPAddress ip, int mod, boolean savId, boolean cfgIp){
    #ifdef DEBUG
    Serial.printf("\n[wfifi_connect_station] ssid: %s - pswd: %s - ip: %s\n", ssid, password, ip2string(ip).c_str());
    #endif

    int retries = 0;
    int maxRetries = 0;   

    WiFi.mode(WIFI_STA);   
    WiFi.setAutoReconnect(true);  

    WiFi.disconnect();
    delay(20);

    int count = 0;
    char buffer[250];
    sprintf(buffer, "host:%s\npswd:%s",ssid, password);



    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)  {
        retries++;
        count++;
        if (retries > 40) {  

            WiFi.mode(WIFI_STA);   

            WiFi.disconnect();
            delay(20);

            WiFi.begin(ssid, password);
            retries = 0;
            maxRetries++;
            #ifdef DEBUG
            Serial.printf("\n\tmaxRetries: %d\n", maxRetries);
            #endif
        }   

        delay(500);
        #ifdef DEBUG
        Serial.printf(".");
        #endif

        if (maxRetries > 2) break;
    }

    if (WiFi.status() == WL_CONNECTED) {
        if (mod == 0) {
            String      sIp;
            IPAddress gateway856_28;
            IPAddress subnet856_28;  

            if (!wfifi_retIP(wifi_id.gateway,   gateway856_28,   sIp))    gateway856_28  = {192, 168, 0,   254};
            wifi_id.gateway = ip2string(gateway856_28);

            if (!wfifi_retIP(wifi_id.subnet,    subnet856_28,    sIp))    subnet856_28   = {255, 255, 255, 0};
            wifi_id.subnet = ip2string(subnet856_28);

            if (cfgIp) WiFi.config(ip, gateway856_28, subnet856_28);
            wifi_id.pswd    = String(password);
            wifi_id.ssid    = String(ssid);
            wifi_id.gateway = WiFi.gatewayIP().toString();
            wifi_id.subnet =  WiFi.subnetMask().toString();
            wifi_id.ip =      WiFi.localIP().toString();
        }
        if (mod == 1) {
            wifi_id.pswd    = String(password);
            wifi_id.ssid    = String(ssid);
            wifi_id.gateway = WiFi.gatewayIP().toString();
            wifi_id.subnet =  WiFi.subnetMask().toString();
            wifi_id.ip =      WiFi.localIP().toString();
        }      


        if (savId) wfifi_getID_toSPIFF();

        #ifdef DEBUG
        Serial.printf("[wfifi_connect_station] Succes\n %s - %s - %s\n", wifi_id.ip.c_str(), wifi_id.gateway.c_str(), wifi_id.subnet.c_str());
        #endif
        return true;
    } else {
        #ifdef DEBUG
        Serial.printf("[wfifi_connect_station] Fail\n");
        #endif
        return false;
    }
}

void wfifi_connect_ap(boolean savId){
    String AP_psk = "adrilight1234";
    // IPAddress ap_local_ip   (192, 168, 0, 1);
    // IPAddress ap_gateway    (192, 168, 0, 254);
    // IPAddress ap_subnet     (255, 255, 255, 0);
    // String      sIp;
    // IPAddress   ip;
    // if (!wfifi_retIP(wifi_id.ap_ip ,ip, sIp)) ip = ap_local_ip;

    build_host_name();

    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    // uint8_t mac[WL_MAC_ADDR_LENGTH];
    // WiFi.softAPmacAddress(mac);
    // String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
    //       String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
    // macID.toUpperCase();

    // String AP_NameString = HOST_NAME_PREFIX + macID;

    // char AP_NameChar[AP_NameString.length() + 1];
    // memset(AP_NameChar, 0, AP_NameString.length() + 1);

    // for (int i = 0; i < AP_NameString.length(); i++)
    //    AP_NameChar[i] = AP_NameString.charAt(i);
    WiFi.softAP(b_host_name, AP_psk);
    wifi_id.ap_ssid   = String(b_host_name);
    wifi_id.ap_psk    = AP_psk;
    wifi_id.ap_ip     = ip2string(WiFi.softAPIP());
    if (savId) wfifi_getID_toSPIFF();
    #ifdef DEBUG
    Serial.printf("\n[wfifi_connect_ap] ssid: %s - pswd: %s\n", wifi_id.ap_ssid.c_str(), wifi_id.ap_psk.c_str());
    #endif
    // Serial.printf("[wfifi_connect_ap] Succes\n");
}

// int mod     : 0 = FORCESTA | 1 = AP | 2 = AUTO
// wifi_id.mod : 0 = STA | 1 = AP
void _wfifi_connect(int mod, char* ssid, char* password, IPAddress staticIP, String hostName, boolean savId, boolean cfgIp) {
    #ifdef DEBUG
    Serial.printf("\n[wfifi_connect] Start\n");
    #endif

    wfifi_getID_fromSPIFF();

    int         id;
    char        sSSID[80];
    char        sPSWD[80];  
    String      sIp;
    IPAddress   ip;      
    int         modStation = -1;


    if (mod == 0) {
        if ((ssid == "") && (password == "")){ 
            // if (wifi_id.mod == 0) {     
            int   id = wfifi_getID(sSSID, sPSWD); 
            if (id == 0)  modStation = 0;   
            // }      
        } else modStation = 1;
    }  
    if (mod == 2) {
        if (wifi_id.mod == 0) {
            int id = wfifi_getID(sSSID, sPSWD); 
            if (id == 0)  modStation = 0;   
        }   
    }   
    if (modStation == 0) {
        if (wfifi_retIP(wifi_id.ip, ip, sIp)) {
            if (!wfifi_connect_station(sSSID, sPSWD, ip, 0, savId, cfgIp)) modStation = -1;      
        } else {
            if (!wfifi_connect_station(sSSID, sPSWD, ip, 1, savId, cfgIp)) modStation = -1;  
        }
    }
    if (modStation == 1) {
        if (!wfifi_retIP(wifi_id.ip, ip, sIp)) ip = staticIP;
        if (!wfifi_connect_station(ssid, password, ip, 0, savId, cfgIp)) modStation = -1;
    }
    if (modStation == -1) {
        wfifi_connect_ap(savId);
    } else {
        #ifdef DEBUG
        Serial.printf("\tStarting mDSN : %s.local\n", DEVICENAME.c_str());
        #endif
        MDNS.begin(DEVICENAME.c_str());  
        MDNS.addService("http", "tcp", 80);  
        wifi_id.mod = 0;
    }
    #ifdef DEBUG
    Serial.printf("\tmodStation: %d\n", modStation);
    Serial.printf("\n[wfifi_connect] End\n");
    #endif
}

//*************************** TELNET THINGS *************************************

WiFiServer telnetServer(23);
WiFiClient telnetClient;

void telnet_setup() {
      telnetServer.begin();
      telnetServer.setNoDelay(true);
      fsprintln("Telnet Client can connect");
}
void telnet_loop() {
    // look for Client connect trial
    if (telnetServer.hasClient()) {
        if (!telnetClient || !telnetClient.connected()) {
            if (telnetClient) {
                telnetClient.stop();
                fsprintln("Telnet Client Stop");
            }
            telnetClient = telnetServer.available();
            fsprintln("New Telnet client");
            telnetClient.flush();  // clear input buffer, else you get strange characters
        }
    }
}

// when we want to send data to telnet
boolean telnet_put(char * output_string) {
    if (telnetClient && telnetClient.connected()) {
        telnetClient.print(output_string);
    }
}

boolean telnet_put(String * output_string) {
    if (telnetClient && telnetClient.connected()) {
        telnetClient.print(output_string->c_str());
    }
}

// when we want to get data from telnet
boolean telnet_get(char * input_string) {
    if (telnetClient.available()) {
        input_string[0] = char(telnetClient.read());
        return true;
    } else {
        return false;
    }
}

void remote_print(String * s) {
    if( *s == "") return;
    telnet_put(s);
}

void remote_print(char * s) {
    if( s == "") return;
    telnet_put(s);
}


void local_print(String * s) {
    if( *s == "") return;
    Serial.println(*s);
}
void local_print(char * s) {
    Serial.println(s);
}

void debug(String * s) {
    *s+="\n";
    switch (debug_mode) {
        case both:      local_print(s);
                        remote_print(s);
                        break;
        case local:     local_print(s);
                        break;
        case remote:    remote_print(s);
                        break;
    }
}

dm debug_mode = local;

void debug(const String * s) {
    String t = *s;
    debug(&t);
}

void debug(char * buf) {
    strcat(buf,"\n");
    switch (debug_mode) {
        case both:      local_print(buf);
                        remote_print(buf);
                        break;
        case local:     local_print(buf);
                        break;
        case remote:    remote_print(buf);
                        break;
    }
}