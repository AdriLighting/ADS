#include "devices.h"
#include <adri_tools.h>
#include <adri_webserver_reponselist.h>
#include <adri_webserver_reponse.h>
#include <adri_webserver_parsecommand.h>
#include <adri_wifiConnect.h>
namespace {

    const size_t serializeSize = 512 * 20;



}
    // server.on("/request_pattern",  HTTP_GET,  [](AsyncWebServerRequest *request)  { 
    //     request_command(request);
    //     String rep = setReponseJson(0, "", false);
    //     request->send(200, "application/json", rep );
    // });
    // server.on("/request_upd",  HTTP_GET,  [](AsyncWebServerRequest *request)  {   
    //     String rep = setReponseJson(0, "", false);
    //     request->send(200, "application/json", rep );

    // }); 

    int command_wifi_staSSID(String value)  {wifiConnect_instance()->_credential_sta->ssid_set(value);  wifi_credential_sta_toSpiff(); wifi_credential_sta_print();}    
    int command_wifi_staPsk(String value)   {wifiConnect_instance()->_credential_sta->psk_set(value);   wifi_credential_sta_toSpiff(); wifi_credential_sta_print();}    
    int command_wifi_sta(String value)      {
        wifiConnect_instance()->connect_set(AWC_LOOP);
        wifiConnect_instance()->connectSSID_set(AWCS_NORMAL);   
        wifiConnect_instance()->savToSpiff(); 
        ESP.restart();
    }   
    int command_wifi_ap(String value)       {
        wifiConnect_instance()->connect_set(AWC_SETUP);
        wifiConnect_instance()->connectSSID_set(AWCS_AP);       
        wifiConnect_instance()->savToSpiff();
        ESP.restart();
    }   
    requestCommandsLists requestEventCommandsList [] = {
        {"sta_ssid",        &command_wifi_staSSID,              "ALL",          NULL,   0},
        {"sta_psk",         &command_wifi_staPsk,               "ALL",          NULL,   0},
        {"co_sta",          &command_wifi_sta,              "ALL",          NULL,   0},
        {"co_ap",           &command_wifi_ap,               "ALL",          NULL,   0},
    };
    uint8_t requestEventCommandsListCount = ARRAY_SIZE(requestEventCommandsList);




devicesManage * devicesManagePtr = nullptr;
devicesManage * devicesManagePtr_get(){return devicesManagePtr;}


devicesManage::devicesManage(){
	devicesManagePtr = this;
	_devices_edit 	= new devices_edit();
	_output_edit 	= new output_edit();
    _devices_spiff  = new devicesspiff();
    _oledRequest    = new oledRequest();
    _appiUdp        = new appiUdp();
	_timer_upd		= new adri_timer(5000, "", false);
	_devices_spiff->deviceSpiff_load();

}

void devicesManage::devices_print(){
	_devices->print();
}
void devicesManage::outputEdit_print(){
	_output_edit->display(_output_edit);
}
void devicesManage::udpParse(String udp_msg) { 
    String msg = udp_msg;  

    // #ifdef WEBSERVER_ENABLE
        String disp = msg + "\n";
        // fsprintf("\n[devicesManage::udpParse]\n%s",disp.c_str());
        String rep;
        String op =     literal_value("op",     msg);
        String uMsg =   literal_value("msg",    msg);
        String cmd =    literal_value("cmd",    msg);
        String dn =     literal_value("dn",     msg);
        String val =    literal_value("val",    msg);
        String grp =    literal_value("grp",    msg);
        String upd =    literal_value("upd",    msg);

        if (op == "request_btn" ) { 
            // int c = getCommand(uMsg, "request_pattern", true); 
            int c = getCommand( uMsg, "request_mod", true, requestEventCommandsListCount, requestEventCommandsList);
            rep = appi_rep_loop("udpParse");
            udp_serverPtr_get()->udp_send(rep);            
        }
        else if (op == "udpRequest_dn" ) { 
            char buff[80];
            sprintf(buff, "%s", cmd.c_str());
            _appiUdp->appi_send_udpRequest_dn(dn, buff, val, upd);
        }
        else if (op == "udpRequest_grp" ) { 
            
            char buff[80];
            sprintf(buff, "%s", cmd.c_str());
            _appiUdp->appi_send_udpRequest_mdn(val, buff, grp, upd);
        }        
        else if (op == "udpRequest_transfert" ) {
                cmd = literal_value("msg",  msg);
                dn  = literal_value("dn",   msg);
                op  = _appiUdp->device_setOp(dn);
                if (op == "") return;
                String  s2 = "";
                        s2 =    literal_item("dn",  dn);
                        s2 +=   literal_item("op",  op);
                        s2 +=   literal_item("msg", cmd);   
                        if (upd!=""){
                            _oledRequest->clear();   
                            _appiUdp->udp_appi_rep = true;
                            int cnt = _oledRequest->oled_request.cnt;
                            _oledRequest->oled_request.device[cnt][0] = dn;
                            _oledRequest->oled_request.device[cnt][1] = "";
                            _oledRequest->oled_request.cnt++; 
                            _oledRequest->start();
                        }                                       
                        udp_serverPtr_get()->udpMulti_send(s2) ;  
        }else {
            int c = getCommand( uMsg, "request_mod", true, requestEventCommandsListCount, requestEventCommandsList);
        }
        // if (upd == "s_output") {
        //     rep = appi_rep_loop("udp_list");
        //     udp_send(rep); 
        // }
        


    // #endif

}
void devicesManage_udpParse(String udp_msg) { 
    devicesManagePtr->udpParse(udp_msg);
}

void devicesManage::udpMultiParse(String req){

    String  server  = literal_value("server_send",  req);
    String  op      = literal_value("op",           req);
    String  preset 	= literal_value("preset",  		req);

    
    if (preset != "") return;

    if ( (op == "upd_websocket") && (!_oledRequest->oled_request.send) ) {

        _oledRequest->clear();

        String 	msg = literal_value("msg", req);
        String 	list[MAX_DEVICES];
        int 	count = explode(msg, ';', list);
        for( int j = 0; j < count-1; j++) {
        	int cnt = _oledRequest->oled_request.cnt;
            _oledRequest->oled_request.device[cnt][0] = list[j];
            _oledRequest->oled_request.device[cnt][1] = "";
            _oledRequest->oled_request.cnt++;        
        }

        #ifdef DEBUG
        	Serial.printf("\noled_request_start\nlist: %s\n", msg.c_str());
        #endif

       _oledRequest->start();

    }


    String udp_hsv_req = literal_value( "udp_hsv_req",  req);
    if (udp_hsv_req == "0") {
        _appiUdp->udp_wait_for_hsvLoop = false;

        String rep = appi_rep_loop("udp_list");
        udp_serverPtr_get()->udp_send(rep);  

        // webserverSend_list_devicesOutput_toSocket(); 

        return;

    } else if (udp_hsv_req == "1") {
        _appiUdp->udp_wait_for_hsvLoop = true;
        return;
    }


    String 	dn 	    = literal_value("dn",           req);
    String 	ip 	    = literal_value("ip",           req);
    String 	str     = literal_value("add",          req);


    // if (op == "upd_websocket") {
    // 	_timer_upd->activate();
    //     fsprintf("\n[udpMultiParse upd_websocket]\n\t[server: %s][op: %s][preset: %s][dn: %s]\n", 
    //         server.c_str(), 
    //         op.c_str(), 
    //         preset.c_str(),
    //         dn.c_str()
    //     );
    // }
    if (_devices_spiff->deviceSpiff_search(dn) < 0) {

    	if ((str != "") && (op == "add") && (server == "")  && (dn != "")) {
            
            _devices_spiff->deviceSpiff_addDevice(dn);

            _devices_spiff->deviceSpiff_addData(dn, str);

            _output_edit->update(_output_edit);

            #ifdef OLED_ENABLE
                oled_menu_create_items_device();
            #endif

        }

    }  else {
            if ((server=="") ) {

                _devices->deviceIsConnected(dn, true);

                if (str != "") {

                     _devices_spiff->deviceSpiff_addData(dn, str);

                    _output_edit->update(_output_edit);

                    // if (_appiUdp->udp_appi_rep) {
                        if ((!_appiUdp->udp_wait_for_hsvLoop) && (op == "rep")) {
                            if (_oledRequest->oled_request.send) {
								    fsprintf("\n[udpMultiParse 2]\n\t[server: %s][op: %s][preset: %s][dn: %s]\n", 
								        server.c_str(), 
								        op.c_str(), 
								        preset.c_str(),
								        dn.c_str()
								    );                               	
                                _oledRequest->upd_device(dn); 
                                if (_oledRequest->canSend()){

  

                                    // webserverSend_selected_devicesOutput_toSocket();
                                    String rep = appi_rep_loop("udp_list");
                                    udp_serverPtr_get()->udp_send(rep);      
                                    _oledRequest->end();

                                }                                
                            } else {
                                // webserverSend_list_devicesOutput_toSocket();
                            }
                        }
                    // } 
                }
            }    	
    }
}
void devicesManage_udpMultiParse(String req){ 
	devicesManagePtr->udpMultiParse(req);
}

void devicesManage::loop(){
	_devices->upd_udpDevices();	

        if ((_oledRequest->oled_request.send)) {

            // if (web_socket_server_msg == "init_webpage"){
            //     webserverSend_list_devicesOutput_toSocket();
            //     webserverSend_list_group_toSocket();
            //     web_socket_server_msg= "";
            //     return;         
            // }       

                        
                // String cmd;
                // String dn;
                // String op;
                //     cmd = literal_value("msg",  web_socket_server_msg);
                //     dn  = literal_value("dn",   web_socket_server_msg);
                //     op  = _appiUdp->device_setOp(dn);
                //     if (op == "") return;
                //     String  s2 = "";
                //             s2 =    literal_item("dn",  dn);
                //             s2 +=   literal_item("op",  op);
                //             s2 +=   literal_item("msg", cmd);   
                //             _oledRequest->clear();  
                //             int cnt = _oledRequest->oled_request.cnt; 
                //             _oledRequest->oled_request.device[cnt][0] = dn;
                //             _oledRequest->oled_request.device[cnt][1] = "";
                //             _oledRequest->oled_request.cnt++; 
                //             _oledRequest->start();                                       
                //             udp_serverPtr_get()->udpMulti_send(s2) ;  

                // web_socket_server_msg= "";
        }     
}

int* splitColor(String Val) {
    String      aVal            = Val;
    byte        firstIndex      = aVal.indexOf('.');
    byte        secondIndex     = aVal.indexOf('.', firstIndex + 1);
    String      red             = aVal.substring(0, firstIndex);
    String      green           = aVal.substring(firstIndex + 1, secondIndex);
    String      blue            = aVal.substring(secondIndex + 1);
    int         r               = red.toInt();
    int         g               = green.toInt();
    int         b               = blue.toInt();
                r               = map(r, 0, 255, 0, 255);
                g               = map(g, 0, 255, 0, 255);
                b               = map(b, 0, 255, 0, 255);

    int     *array          = new int[3];
            array[0]        = r;
            array[1]        = g;
            array[2]        = b;
    return array;    
}
String devicesManage::list_devicesOutput(){
    boolean     start = true;
    String      dn;
    String      list;
    int         oc;

    for( int i = 0; i < MAX_DEVICES; i++) { 

        dn      = devices_array[i].device_name;
        if (dn == "") continue;

        // webSocketUpdate_array[i]->isConnected = devices_array[i].isConnected;

        oc      = devices_array[i].output_count;

        list += dn + "*";
        list += devices_array[i].device_ip + "*";
        for( int j = 0; j < oc; j++) {
            list += devices_array[i].output_location[j] + ";";
            list += devices_array[i].output_grp[j] + ";";
            list += String(devices_array[i].toggleOnOff[j]) + ";";
            list += String(j) + ";";
            int  *c = splitColor(devices_array[i].strip_color[j]);
            list += devices_array[i].strip_color[j]+";";
            list += String(devices_array[i].strip_hue[j])+"."+ String(devices_array[i].strip_sat[j])+"."+ String(devices_array[i].strip_hBri[j])+";";
            list += String(devices_array[i].trueWhite[j])+";";
            list += String(devices_array[i].isConnected)+";";
            list += String(devices_array[i].output_selected[j])+";";
            list += "*";

        }    
        list += "|";    

    }  
    return list;
}
void devicesManage::list_devicesOutput(JsonObject & object) {
    JsonObject  objectId ;
    String      value;  
    String      dn;
    int         oc;
    // fsprintf("\n[list_devicesOutput]\n");
    for (int i = 0; i < MAX_DEVICES; ++i) {

        dn = devices_array[i].device_name;
        if (dn == "") continue;

        // fsprintf("[%s]\n", dn.c_str());

        objectId = object.createNestedObject(String(i));

        objectId[F("dn")] = dn;
        objectId[F("co")] = devices_array[i].isConnected;
        // if (calendar_array[i]->period == dtDailyAlarm)   objectId[F("lapse")] = String("-1");
        // else                                             objectId[F("lapse")] = calendar_array[i]->o_endValue;
        // objectId[F("repeat")]    =   calendar_array[i]->repeat;
        // objectId[F("value")]     =   calendar_array[i]->startTime;
        // objectId[F("activ")]     =   String(calendar_array[i]->isEnabled);
        oc = devices_array[i].output_count;
        JsonArray array = objectId.createNestedArray(F("oLoc"));
        _output_edit->get_output_info(devices_array[i].output_location, oc, array);
        array = objectId.createNestedArray(F("oGrp"));
        _output_edit->get_output_info(devices_array[i].output_grp,  oc, array);
        array = objectId.createNestedArray(F("oTog"));
        _output_edit->get_output_info(devices_array[i].toggleOnOff, oc, array);
        array = objectId.createNestedArray(F("oCol"));
        _output_edit->get_output_info(devices_array[i].strip_color, oc, array);
        array = objectId.createNestedArray(F("oTw"));
        _output_edit->get_output_info(devices_array[i].trueWhite, oc, array);    
        // array = objectId.createNestedArray(F("oCon"));
        // _output_edit->get_output_info(devices_array[i].isConnected, oc, array); 
        array = objectId.createNestedArray(F("oSel"));
        _output_edit->get_output_info(devices_array[i].output_selected, oc, array);  
        array = objectId.createNestedArray(F("oHsv"));
        for( int j = 0; j < oc; j++) {
            String v = String(devices_array[i].strip_hue[j])+"."+ String(devices_array[i].strip_sat[j])+"."+ String(devices_array[i].strip_hBri[j]);
            array.add(v);
        }                     
        // array = objectId.createNestedArray(F("oHsv"));
        // _output_edit->get_output_info(devices_array[i].strip_color, oc, array);
    }   


}
void devicesManage::list_devicesOutput_jsonFile(String & result) {
    fsprintf("\n[list_devicesOutput_jsonFile]\n");
    DynamicJsonDocument json(serializeSize);
    JsonObject  root    = json.to<JsonObject>();
    JsonObject  object  = root.createNestedObject("l_o");
    list_devicesOutput(object);
    serializeJson(json, Serial);
}
void devicesManage::list_devicesOutput_jsonFile() {
    // File file = SPIFFS.open("/json/events.json", "w");
    // if (file) {
    //     DynamicJsonDocument json(serializeSize);
    //     JsonObject  root    = json.to<JsonObject>();
    //     root["selected"]    = ch_toString(events_spiff_list[appi_event_select]._id);
    //     JsonObject  object  = root.createNestedObject("eventslist");
    //     events_jsonObject(object);
    //     serializeJson(json, file);
    //     file.close();
    // }       
}
String devicesManage::list_devices(){
    boolean     start = true;
    String      s_json;
    String      dn;
    String      list;

    for( int j = 0; j < MAX_DEVICES; j++) { 
        dn      = devices_array[j].device_name;
        list    += dn + ";";
    }  
    s_json = "{";
    s_json += jsonAddStringValue(start, "list_dn", list);
    s_json += "}";
    return s_json;
}
String devicesManage::list_group(){
    boolean     start = true;
    String      dn;
    String      list;

    for (int i = 0; i < _devices_edit->grp_count; ++i) {
        dn      = _devices_edit->groupes[i];
        list    += dn + ";";
    }  

    return list;
}

String devicesManage::appi_rep_loop(String op){
    String      s_json;

    // String json;
    // list_devicesOutput_jsonFile(json) ;
    
    String lo = list_devicesOutput(); 
    String lg = list_group();

    s_json = "{";
    s_json += jsonAddStringValue(true, "op", op);
    s_json += jsonAddStringValue(false, "l_o", lo);
    s_json += jsonAddStringValue(false, "l_g", lg);
    s_json += "}";
    return s_json;
}


String _serial_deviceDisplay(String cmd, String value){
	devicesManagePtr->devices_print();
}		
String _serial_outputEdit_display(String cmd, String value){
	devicesManagePtr->outputEdit_print();
}	





oledRequest * oledRequestPtr;
oledRequest * oledRequestPtrGet(){return oledRequestPtr;}    
oledRequest::oledRequest(){
    oledRequestPtr = this;
    oled_request.send = false; 
    oled_request.cnt = 0;
}

int oledRequest::search_device(String search){
    int ret = -1;
    for( int i = 0; i < oled_request.cnt; i++) { 
        if(search == oled_request.device[i][0]) {
            ret = i;
            break;
        }
    }
    #ifdef DEBUG
    	fsprintf("\n[oledRequest::search_device] : %s - ret: %d\n",search.c_str(), ret );
    #endif
    return ret;
}
void oledRequest::upd_device(String search){
    int sc = search_device(search);
    if (sc >= 0) { 
    	oled_request.device[sc][1] = "1"; 
    	#ifdef DEBUG
    		fsprintf("\n[oledRequest::upd_device] : %s\n", search.c_str());
    	#endif
    }
}

void oledRequest::end(){
    for( int i = 0; i < MAX_DEVICES; i++) { 
        oled_request.device[i][0] = "";
        oled_request.device[i][1] = "";
    }
    oled_request.send = false;
}
void oledRequest::start(){
    oled_request.send = true;
}
void oledRequest::clear(){
    for( int i = 0; i < MAX_DEVICES; i++) { 
        oled_request.device[i][0] = "";
        oled_request.device[i][1] = "";
    }
    oled_request.cnt = 0;
}
String oledRequest::sendString(){
    String s = "";
    for( int i = 0; i < oled_request.cnt; i++) { 
        s += oled_request.device[i][0] + ";";
    }
    String send;
    send =  literal_item("op",  "upd_websocket");
    send += literal_item("msg", s);
    udp_serverPtr_get()->udpMulti_send(send);        
}
boolean oledRequest::canSend(){
    int cnt = 0;


    // String d = "\ncansend - cnt: " + String(oled_request.cnt) + "\n";
    // debug(&d);
    #ifdef DEBUG
    	fsprintf("\n[oledRequest::canSend]\n");
    #endif

    for( int i = 0; i < oled_request.cnt; i++) { 
        if(oled_request.device[i][1] != "") {  
            cnt++;

            #ifdef DEBUG
            	fsprintf("\tadd: %s - %d/%d\n", oled_request.device[i][0].c_str(), cnt, oled_request.cnt);          
            #endif

        } else {
            String send;
            send =  literal_item("op",  "get_rep");
            send += literal_item("dn", oled_request.device[i][0]);   
            udp_serverPtr_get()->udpMulti_send(send);           
        }

            // debug(&d);        
    }
    boolean ret = false;

    if ( (cnt) > (oled_request.cnt-1)) {
        String d = "cnt: " + String(cnt) + "\n";
        // debug(&d);        
        ret=true;
    }

    return ret;
}
int oledRequest::cnt(){
    int cnt = 0;
    for( int i = 0; i < oled_request.cnt; i++) { 
        if(oled_request.device[i][1] != "") {  
            cnt++;
        } 
    }
    return cnt;
}









#include <dac_command.h>
appiUdp * appiUdpPtr;
appiUdp * appiUdpPtrGet(){return appiUdpPtr;}
appiUdp::appiUdp(){
    appiUdpPtr = this;
    _oledRequest = oledRequestPtrGet();
}
String appiUdp::c_to_string(char * name){
    char buffer[80];
    sprintf(buffer, "%s", name);
    return String(buffer);
}

String appiUdp::device_setOp(String dn) {
    String ret = "";
    String lib = devicesPtrGet()->getLib(dn);
    if (lib == "multidevice")   ret = "lp";
    if (lib == "multiLampe")    ret = "request_lp";
    return ret;
}
char * appiUdp::device_set_req(char * cmd) {
    char * ret ;
    ret = cmd;
    if (cmd == req_ss_lampOnOff)    {ret = req_lampOnOff;}
    if (cmd == req_ss_ledBri)       {ret = req_ledBri;}
    if (cmd == req_ss_lampBri)      {ret = req_lampBri;}
    if (cmd == req_ss_lampSat)      {ret = req_lampSat;}
    if (cmd == req_ss_lampHue)      {ret = req_lampHue;}
    return ret;
}

String appiUdp::device_setVal(String dn, char * cmd, String val, int pos) {
    String ret = "";
    String lib = devicesPtrGet()->getLib(dn);
    if (lib == "multidevice")   {ret = String(pos) + "|" + val;}
    if (lib == "multiLampe")    {
        if (cmd == req_ss_lampOnOff) {
            if (val == "1") ret = "off";
            if (val == "0") ret = "on";
        }
    }
    return ret;
}
String appiUdp::device_setVal(char * cmd, String val) {
    String ret = "";
    if (cmd == req_ss_lampOnOff) {
        if (val == "1") ret = "off";
        if (val == "0") ret = "on";
    }
    return ret;
}

String appiUdp::request_set_format(char * cmd, String value, String e, String s){

    String ret = e + c_to_string(cmd) + s + value;
    return ret;
}
String appiUdp::request_set_format(char * cmd, String e, String s){

    String ret = e + c_to_string(cmd) + s ;
    return ret;
}

boolean appiUdp::request_isTrueWhite(char * req){
    if(req == req_ss_lampBri)   return true;
    if(req == req_lampBri)      return true;
    return false;
}
char *  appiUdp::request_getTrueWhite(char * req){
    if(req == req_lampBri)      return req_lampWhite;
    if(req == req_ss_lampBri)   return req_ss_white;
}
void appiUdp::appi_send_udpRequest_dn(String dn, char * req, String val, String upd) {
    // String disp = "oled_send_udpRequest_dn\n"; 
    // debug(&disp);

    String lib  = devicesPtrGet()->getLib(dn);
    if (lib == "multiLampe") req = device_set_req(req);
    String cmd  = request_set_format(req, val, "&", "=");
    String op   = device_setOp(dn);
    if (op == "") return;
    String  s2 = "";
    s2 =    literal_item("dn",  dn);
    s2 +=   literal_item("op",  op);
    s2 +=   literal_item("msg", cmd);
    if (upd!="") {
        _oledRequest->clear();
        udp_appi_rep = true;
        int cnt = _oledRequest->oled_request.cnt;
        _oledRequest->oled_request.device[cnt][0] = dn;
        _oledRequest->oled_request.device[cnt][1] = "";
        _oledRequest->oled_request.cnt++;     
        _oledRequest->sendString();
        _oledRequest->start();
    }
    udp_serverPtr_get()->udpMulti_send(s2);      
}
void appiUdp::appi_send_udpRequest_mdn(String val, char * req, String dg, String upd) {
    // String disp = "oled_send_udpRequest_mdn\n"; 
    // debug(&disp);

    String cmd = request_set_format(req, "&", "=");

    String  s2 = "";

    // #ifdef OLED_UPD_WEBSOCKET
    if (upd!="") {
        _oledRequest->clear();
        udp_appi_rep = true;
    }
    // #endif

    for( int i = 0; i < devices_count; i++) {
        int output_count = devices_array[i].output_count;
        String s = "";
        for( int j = 0; j < output_count; j++) {
            // if ((om_toggle_select) && (val == "1") && (req == req_s_output) && (!devices_array[i].toggleOnOff[j])) continue; 

            // if ( (oled_menu_starterItem == c_to_string(om_act_color)) && (output_isRelay(devices_array[i].output_type[j])) ) continue;
            if (dg != devices_array[i].output_grp[j]) continue;
            // if (om_arborescence_item[1] == c_to_string(om_act_rommGrp)){
            //  if (om_arborescence_item[2] != devices_array[i].device_room[j]) continue;
            // } 

            String lib  = devicesPtrGet()->getLib(devices_array[i].device_name);
            if (lib == "multiLampe")    {   s = device_setVal(req, val); cmd = request_set_format(device_set_req(req), "&", "=");}
            else                            s+= String(j) + "|" + val + "*";        
            
        }
        if (s == "") continue;
        String op   = device_setOp(devices_array[i].device_name);
        if (op == "") continue;   

        #ifdef OLED_UPD_WEBSOCKET
            if (upd!="") {
                int cnt = _oledRequest->oled_request.cnt;
                _oledRequest->oled_request.device[cnt][0] = devices_array[i].device_name;
                _oledRequest->oled_request.device[cnt][1] = "";
                oled_request.cnt++;
            }
        #endif

        s2 +=   literal_item("dn", devices_array[i].device_name);
        s2 +=   literal_item("op", op);
        s2 +=   literal_item("msg", cmd + s);
        s2 +=   "#";
    }   
    if (upd!="") {  
        _oledRequest->sendString();
        _oledRequest->start(); 
    }
    udp_serverPtr_get()->udpMulti_send(s2);
}

char PROGMEM rep_wifi_reset                         [] = "wifi_reset";
char PROGMEM rep_wifi_subnet                        [] = "wifi_subnet";
char PROGMEM rep_wifi_gateway                       [] = "wifi_gateway";
char PROGMEM rep_wifi_ip                            [] = "wifi_ip";
char PROGMEM rep_wifi_ssid                          [] = "wifi_ssid";
char PROGMEM rep_wifi_psk                           [] = "wifi_psk";
char PROGMEM rep_wifiAp_ip                          [] = "wifiAp_ip";
char PROGMEM rep_wifiAp_psk                         [] = "wifiAp_psk";
char PROGMEM rep_wifiAp_ssid                        [] = "wifiAp_ssid";
String getValue_wifi_reset()                          { return  wifiConnect_instance()->connectSSID_get();}
String getValue_wifi_subnet()                         { return  wifiConnect_instance()->currentIp_get();}
String getValue_wifi_gateway()                        { return  wifiConnect_instance()->currentIp_get();}
String getValue_wifi_ip()                             { return  wifiConnect_instance()->currentIp_get();}
String getValue_wifi_ssid()                           { return  wifiConnect_instance()->staSsid_get();}
String getValue_wifi_psk()                            { return  wifiConnect_instance()->staPsk_get();}
String getValue_wifiAp_ip()                           { return  wifi_credentialAp_ptr_get()->ip_get();}
String getValue_wifiAp_psk()                          { return  wifi_credentialAp_ptr_get()->psk_get();}
String getValue_wifiAp_ssid()                         { return  wifi_credentialAp_ptr_get()->hostname_get();}
PROGMEM requestReponseLists reponseLists [] = {

{ rep_wifi_reset                            ,rft_number     ,&getValue_wifi_reset                       },
{ rep_wifi_ip                               ,rft_string     ,&getValue_wifi_ip                          },
{ rep_wifi_ssid                             ,rft_string     ,&getValue_wifi_ssid                        },
{ rep_wifi_psk                              ,rft_string     ,&getValue_wifi_psk                         },
{ rep_wifiAp_ip                             ,rft_string     ,&getValue_wifiAp_ip                        },
{ rep_wifiAp_psk                            ,rft_string     ,&getValue_wifiAp_psk                       },
{ rep_wifiAp_ssid                           ,rft_string     ,&getValue_wifiAp_ssid                      },

{ rep_wifi_subnet                           ,rft_string     ,&getValue_wifi_subnet                      },
{ rep_wifi_gateway                          ,rft_string     ,&getValue_wifi_gateway                     },

};
    uint8_t reponseListsCount = ARRAY_SIZE(reponseLists);

void webserver_reponse_setup(){

    int pos = requestReponse_initialize("/request_pattern");
    requestReponse_protocol(pos,    requestProtocol_http);
    requestReponse_mod_set(pos,     requestType_wParam);
    requestReponse_reponseMod(pos,  requestReponseType_jsonFromListForce);
    requestReponse_parseMod(pos,    requestParseCmd_fromList);
    requestReponse_cmdList(pos,     requestEventCommandsListCount, requestEventCommandsList);
    requestReponse_reponseList(pos, reponseListsCount, reponseLists, "op");
    pos = requestReponse_initialize("/request_upd");
    requestReponse_protocol(pos,    requestProtocol_http);
    requestReponse_mod_set(pos,     requestType_wParam);
    requestReponse_reponseMod(pos,  requestReponseType_jsonFromListForce);
    requestReponse_parseMod(pos,    requestParseCmd_none);
    requestReponse_reponseList(pos, reponseListsCount, reponseLists, "op");
}
