#include <ESP8266WiFi.h>

#include <udp.h>
#include <WiFiUdp.h>

#include "config_base.h"

#include "udp.h"
#include "tools.h"
#include "adriwifi.h"
#include "devices.h"
#include "devices_output.h"
#include "devices_preset.h"

#ifdef OLED_ENABLE
    #include <oled_display.h>
    #include "oled_menu.h"
#endif
#ifdef WEBSERVER_ENABLE
    #include "webserver_update.h"
    #include "appi.h"
#endif

#include "serverota.h"

#include "request.h"
#include "oled_menu_func.h"

int 	udp_port 			= 9100;
boolean debug_udp			= false;
WiFiUDP udp_server;
String 	udp_receive_buffer;

extern devices devices;

#ifdef OLED_UPD_WEBSOCKET
    extern oled_requests oled_request;
#endif

/*
	um_hue
	um_slav

*/

IPAddress   udp_multicast_ip    	= {239, 0, 0, 57};
int         udp_multicast_port 		= 9200;
WiFiUDP 	udp_multi;
String 		udpMulti_receive_buffer;

#ifdef WEBSERVER_ENABLE
boolean     udp_wait_for_hsvLoop    = false;
boolean     udp_appi_rep            = false;
#endif

extern String realtime;

boolean upd_websocket = false;
unsigned long upd_aapp_last;

void udMulti_rec(String req) {
    String  server  = literal_value("server_send",  req);
    String  op      = literal_value("op",           req);

    String  preset = literal_value( "preset",  req);
    if (preset != "") return;

    #ifdef OLED_ENABLE
        if (server == "2") {
            if (op == "realtime") {

                String s_hour   = literal_value("hour",      req );
                String s_minute = literal_value("minute",    req );
                String s_second = literal_value("second",    req );
                String s_hum = literal_value("relative_humidity",    req );
                String s_temp = literal_value("temperature",    req );

                if (oled_display_mod == oled_display_time) {
                    #ifdef ADAFRUIT_SD1306_LIB
                        display.clearDisplay();
                        display.setTextSize(2);
                        display.setTextColor(WHITE);
                        display.setCursor(0, 20);
                        display.println("Time: "+ s_hour + ":" + s_minute + ":" + s_second);
                        display.setTextSize(1);
                        display.setCursor(0, 40);
                        display.print(F("Temperature: "));
                        display.print(s_temp);
                        display.println(F(" C"));
                        display.setCursor(0, 50);
                        display.print(F("Humiditee: "));
                        display.print(s_hum);
                        display.println(F("%"));                                        
                        display.display();      
                    #endif
                    
                    #ifdef SD1306WIRE_LIB
                        oled_clear();
                        display.setFont(ArialMT_Plain_16);
                        display.drawString(0, 20, "Time: "+ s_hour + ":" + s_minute + ":" + s_second);
                        display.setFont(Dialog_plain_11);
                        display.drawString(0, 40, "Temperature: " + s_temp + " C");
                        display.drawString(0, 50, "Humiditee: " + s_hum + " %");
                        display.display();      
                    #endif 
                }
            }
            return;
        }
    #endif

    #ifdef WEBSERVER_ENABLE
        if ( (socket_isConnected()) && (op == "upd_websocket") && (!oled_request.send)) {

            oled_request_clear();

            String msg = literal_value("msg", req);
            String list[MAX_DEVICES];
            int count = explode(msg, ';', list);
            for( int j = 0; j < count-1; j++) {
                oled_request.device[oled_request.cnt][0] = list[j];
                oled_request.device[oled_request.cnt][1] = "";
                oled_request.cnt++;        
            }
            Serial.printf("\noled_request_start\nkist: %s\n", msg.c_str());
            oled_request_start();

        }
        // if (op == "TAMERER") {
        //     Serial.printf("\nreq:\n%s\n", req.c_str());
        //     String msg = literal_value("CONTENT", req);
        //     send_devices_load(msg);
        //     webserverSend_list_devicesOutput_toSocket(); 
        //     return;            
        // }
        String  udp_hsv_req = literal_value( "udp_hsv_req",  req);
        if (udp_hsv_req == "0") {
            udp_wait_for_hsvLoop = false;
            String rep = appi_rep_loop("udp_list");
            udp_send(rep);              
            webserverSend_list_devicesOutput_toSocket(); 
            return;
        } else if (udp_hsv_req == "1") {
            udp_wait_for_hsvLoop = true;
            return;
        }

    #endif 

 
    String 	dn 	    = literal_value("dn",           req);
    String 	ip 	    = literal_value("ip",           req);
    String 	str     = literal_value("add",          req);

    #ifdef DEVICEFROMSPIFF

        if (deviceSpiff_search(dn) < 0) {

        	if ((str != "") && (op == "add") && (server == "")  && (dn != "")) {
                
                deviceSpiff_addDevice(dn);

                deviceSpiff_addData(dn, str);

                outputEdit_update();

                #ifdef OLED_ENABLE
                    oled_menu_create_items_device();
                #endif

            }

        } else {
            
            if ((server=="") ) {



                devices.deviceIsConnected(dn, true);

                if (str != "") {


                    deviceSpiff_addData(dn, str);

                    outputEdit_update();

                    #ifdef WEBSERVER_ENABLE
                        if ((socket_isConnected()) || (udp_appi_rep)) {
                            if ((!udp_wait_for_hsvLoop) && (op == "rep")) {
                                if (oled_request.send) {
                                    oled_request_upd_device(dn); 
                                    if (oled_request_canSend()){
                                        webserverSend_selected_devicesOutput_toSocket();
                                        String rep = appi_rep_loop("udp_list");
                                        udp_send(rep);      
                                        oled_request_end();

                                    }                                
                                } else {
                                    // webserverSend_list_devicesOutput_toSocket();
                                }
                            }
                        }
                    #endif  
                }
            }
        }

    #else

        // Serial.printf("UDP ADD DEVICE %s\n",dn.c_str() );
        if (!devices.deviceIsListed(dn)) {

            if ((str != "") && (op == "add") && (server == "")  && (dn != "")) {
                

                devices.addDevice(dn, str);

                outputEdit_update();

                #ifdef OLED_ENABLE
                    oled_menu_create_items_device();
                #endif
            }

        } else {
            
            if ((server=="") ) {

                // Serial.printf("\req: %s\n", req.c_str() );

                devices.deviceIsConnected(dn, true);

                if (str != "") {

                    if (devices.getListPos(dn) >= 0) {

                        device_add(str, devices.getListPos(dn));

                        outputEdit_update();
                                                                 
                    }
                }
            }
        }

    #endif


    yield();


}

void udpMulti_parse(String * udp_msg) { 
    String msg = *udp_msg;	
    udMulti_rec(msg);
}
boolean udpMulti_receive(String * receive_buffer) {
    int packet_size = udp_multi.parsePacket();
    if (!packet_size) return false;
    char buf[1024];
    int length = udp_multi.read(buf, 1024);
    if (length>0) buf[length]=0;
    * receive_buffer = String(buf);
    return true;
}	
void udpMulti_send(String transmit_buffer) {
    transmit_buffer = literal_item("server_send", "1") + transmit_buffer;

    int size = transmit_buffer.length();
    char buf[size+1];
    sprintf(buf, "%s", 
        transmit_buffer.c_str()
    );	
    udp_multi.beginPacketMulticast(udp_multicast_ip, udp_multicast_port, WiFi.localIP());
    udp_multi.write(buf);
    udp_multi.endPacket();

}		
void udpMulti_send(String serveSend, String transmit_buffer) {
    transmit_buffer = literal_item("server_send", serveSend) + transmit_buffer;

    int size = transmit_buffer.length();
    char buf[size+1];
    sprintf(buf, "%s", 
        transmit_buffer.c_str()
    );  
    udp_multi.beginPacketMulticast(udp_multicast_ip, udp_multicast_port, WiFi.localIP());
    udp_multi.write(buf);
    udp_multi.endPacket();

}   

boolean udp_receive(String * receive_buffer) {

    int packet_size = udp_server.parsePacket();
    if (!packet_size) return false;
    char buf[1024];
    int length = udp_server.read(buf, 1024);
    if (length>0) buf[length]=0;
    * receive_buffer = String(buf);
    return true;
}
void udp_send(String transmit_buffer) {
    int size = transmit_buffer.length();
	char buf[size+1];
	sprintf(buf, "%s", 
		transmit_buffer.c_str()
	);	
	udp_server.beginPacket(udp_server.remoteIP(), udp_server.remotePort());
	udp_server.write(buf);
	udp_server.endPacket();	
}
void udp_parse(String * udp_msg) { 
	String msg = *udp_msg;	

    #ifdef WEBSERVER_ENABLE
        String disp = msg + "\n";
        debug(&disp);
        String rep;
        String op =     literal_value("op",     msg);
        String uMsg =   literal_value("msg",    msg);
        String cmd =    literal_value("cmd",    msg);
        String dn =     literal_value("dn",     msg);
        String val =    literal_value("val",    msg);
        String grp =    literal_value("grp",    msg);
        String upd =    literal_value("upd",    msg);

        if (op == "request_btn" ) { 
            int c = getCommand(uMsg, "request_pattern", true); 
            rep = appi_rep_loop("udp_parse");
            udp_send(rep);            
        }
        else if (op == "udpRequest_dn" ) { 
            char buff[80];
            sprintf(buff, "%s", cmd.c_str());
            appi_send_udpRequest_dn(dn, buff, val, upd);
        }
        else if (op == "udpRequest_grp" ) { 
            
            char buff[80];
            sprintf(buff, "%s", cmd.c_str());
            appi_send_udpRequest_mdn(val, buff, grp, upd);
        }        
        else if (op == "udpRequest_transfert" ) {
                cmd = literal_value("msg",  msg);
                dn  = literal_value("dn",   msg);
                op  = device_setOp(dn);
                if (op == "") return;
                String  s2 = "";
                        s2 =    literal_item("dn",  dn);
                        s2 +=   literal_item("op",  op);
                        s2 +=   literal_item("msg", cmd);   
                        if (upd!=""){
                            oled_request_clear();   
                            udp_appi_rep = true;
                            oled_request.device[oled_request.cnt][0] = dn;
                            oled_request.device[oled_request.cnt][1] = "";
                            oled_request.cnt++; 
                            oled_request_start();
                        }                                       
                        udpMulti_send(s2) ;  
        }else {
            int c = getCommand(uMsg, "request_pattern", true); 
        }
        // if (upd == "s_output") {
        //     rep = appi_rep_loop("udp_list");
        //     udp_send(rep); 
        // }
        


    #endif

}
unsigned long send_serverStatu_timer;
boolean udp_loop() {
    #ifdef WEBSERVER_ENABLE
        if ((millis()-send_serverStatu_timer) > (1000)) {
            send_serverStatu_timer = millis();
            String sendStr = literal_item("op", "serverStatu");
            sendStr += literal_item("serverStatu_deviceOn", webserverSend_list_devicesOn());
            udpMulti_send(sendStr);          
        }
    #endif

	if (udp_receive(&udp_receive_buffer)) udp_parse(&udp_receive_buffer);
	if (udpMulti_receive(&udpMulti_receive_buffer))	udpMulti_parse(&udpMulti_receive_buffer);
}

void udp_start(){
	udp_server.begin(udp_port);
	udp_multi.beginMulticast(WiFi.localIP(), udp_multicast_ip, udp_multicast_port);
}


