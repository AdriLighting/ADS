#include "config_base.h"
#include "request.h"
#include <dac_command.h>
#include "tools.h"
#include "colors.h"
#include "devices.h"
#include "device_def.h"
#include "output_func.h"

#if CLIENT_SELECT==CLIENT_LED
  #include "fastled_func.h"
  #include "led_effect.h"
#endif

// #define DEBUG
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#if CLIENT_SELECT==CLIENT_LED
    void command_ledBri(String v)       {allStrip_setBri(v.toInt());}
    void command_ss_ledBri(String v)    {selectedStrip_setBri(v, false);}

    void command_lampHue(String v)      {allStrip_hue(v.toInt());}
    void command_lampSat(String v)      {allStrip_sat(v.toInt());}
    void command_lampBri(String v)      {allStrip_bri(v.toInt());}
    void command_lampWhite(String v)    {allStrip_white(v.toInt());}

    void command_ss_lampRGB(String v)   {selectedStrip_setcolorFromRGB(v, false);}

    void command_ss_lampHue(String v)   {selectedStrip_hue(v, false);}

    void command_ss_lampSat(String v)   {selectedStrip_sat(v, false);}

    void command_ss_lampBri(String v)   {selectedStrip_bri(v, false);}
    void command_ss_white(String v)     {selectedStrip_white(v, false);}
#endif

void command_s_output(String v)         {output_set_selected(v);}
void command_ss_lampOnOff(String v)     {output_selected_onOff(v, false);}
void command_LampToglle(String v)       {output_all_toggleOnOff();}
void command_ss_lampToglle(String v)    {output_selected_toggleOnOff(v, false);}

void command_lampOnOff(String v) {
    if (        v == "off") output_all_off();
    else if (   v == "on")  output_all_on();
    else {
                            output_all_toggleOnOff();
    }    
}

PROGMEM cmdApp_req_lists cmdApp_req_list [] = { 

    #if CLIENT_SELECT==CLIENT_LED
    {req_ledBri,        &command_ledBri},
    {req_ss_ledBri,     &command_ss_ledBri},

    {req_lampHue,       &command_lampHue},
    {req_lampSat,       &command_lampSat},
    {req_lampBri,       &command_lampBri},
    {req_lampWhite,     &command_lampWhite},

    {req_ss_lampBri,    &command_ss_lampBri},

    {req_lampSetColor,  &selectedStrip_setColor},

    {req_ss_lampRGB,    &command_ss_lampRGB},

    {req_ss_lampHue,    &command_ss_lampHue},

    {req_ss_lampSat,    &command_ss_lampSat},

    {req_ss_white,      &command_ss_white},
    #endif

    {req_s_output,      &command_s_output},

    {req_lampOnOff,     &command_lampOnOff},
    {req_ss_lampOnOff,  &command_ss_lampOnOff},

    {req_toglle,        &command_LampToglle},
    {req_ss_toglle,     &command_ss_lampToglle},    
};
uint8_t cmdApp_req_listCount = ARRAY_SIZE(cmdApp_req_list);


int setCommand(String search, String value, String request_mod, boolean display) {
   char     * name;
   char     buf[80];
   String   s;
   String   ret = "";
   long     start = millis();

   #ifdef DEBUG
       fs(lpinf_titre, "[SetCommand] Start\n");
       fs(lpinf_request_mod, "request_mod");
       fs(lpinf_command, "command");
       fs(lpinf_value, "value");
       fs(lpinf_retvalue, "return value");
       fs(lpinf_runCmd, "------------->RUN FUNCTION<---\n");
   #endif

   value.replace(" ", "");

   int count = 0;

   for (int i = 0; i < cmdApp_req_listCount; ++i) {
      name = cmdApp_req_list[i].name;
      sprintf(buf, "%s",name );
      String req = String(buf);
      if (req == search) {

         // description = request_command_leds[i].description;
         #ifdef DEBUG
             if (display) {
                s+= fsget(lpinf_titre);
                s+= info_parm(fsget(lpinf_request_mod), request_mod);
                s+= info_parm(fsget(lpinf_command), req);
                s+= info_parm(fsget(lpinf_value), value);
                s+= fsget(lpinf_runCmd);
                Serial.printf("%s", s.c_str()); 
             }  
         #endif

        cmdApp_req_list[i].cmd(value);
        count++;

         #ifdef DEBUG
             if (display) {
                s   = "------------->RUN FUNCTION END<---\n";
                ret += req + " - ";
                // s +=info_parm(fsget(lpinf_retvalue), ret);
             }  
         #endif
         break;
      }
   }
   long end = millis();
   #ifdef DEBUG
       if (display) {
          fs(lpinf_end, "Duration");
          s+=info_parm(fsget(lpinf_end), String(end - start));
          fsprintf("%s[SetCommand DONE] Ret: %s\n", s.c_str(), ret.c_str());  
       }
   #endif
   // if (pattern.debugSetvalue) pattern.debugSetvalue  = false;
   // if (command_debug) command_debug  = false;
   return count;
}

int getCommandParm(String s_command, String s_valueCommand, String request_mod, boolean display, int count) {
    int ret = 0;
   
    #ifdef DEBUG
        String s;
        if (display) {
            s = "[getCommandParm] [" + String(count) + "]\n";
            s += info_parm("request_mod", request_mod);
            s += info_parm("command", s_command);
            s += info_parm("value", s_valueCommand); 
        }   
    #endif
    if (request_mod == "cmd_type_1") { 

    } 
    else {

        #ifdef DEBUG
            if (display) Serial.printf("%s", s.c_str());  
        #endif

        ret = setCommand(s_command, s_valueCommand, request_mod, display);

    }

       #ifdef DEBUG
           if (display) {
              fsprintf("[getCommandParm DONE] Ret: %d\n", ret);
           }   
       #endif

   return ret;          
}

int getCommand(String A_readString, String request_mod, boolean display) {
    String  s_command;
    String  s_valueCommand;
    String  s_readString = A_readString;
    char    inputChar[s_readString.length() + 1] ;
            s_readString.toCharArray(inputChar, s_readString.length() + 1);
    char    * Rcommand  = strtok(inputChar, "&");
    int     count       = 1;
    int     cmdCount    = 0;
    while (Rcommand != 0){          
        char* separator  = strchr(Rcommand, '=');
        if (separator != 0) {
            *separator  = 0;            
            ++separator ;
            s_command = String(Rcommand);
            s_valueCommand = String(separator); 
            int c = getCommandParm( s_command,  s_valueCommand, request_mod, display, count);
            cmdCount += c;
            count++;
        }
        Rcommand = strtok(0, "&"); 
    }
    // s_command == "" ? s_command = "UNKNOW_COMMAND" : s_command = s_command;
    return cmdCount;
} 