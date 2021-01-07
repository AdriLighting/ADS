
#include "config_base.h"


#ifndef DEVICES_H
	#define DEVICES_H
    #include <arduino.h>
    #include <FS.h>   

    #include "devices_mod.h"
   

    enum mod_hsv_loop { 
       hsv_loop_h,
       hsv_loop_s,
       hsv_loop_v,
       hsv_loop_b,
       hsv_loop_w,
       hsv_loop_n
    };
    extern mod_hsv_loop hsvb_loop;

    void device_add(String s, int pos);

    String set_output_info(String s, mod_outputs_type *pos);
    String set_output_info(String s, mod_devices_room *pos);
    String set_output_info(String s, mod_devices_grp *pos);
    String set_output_info(String s, int *pos);
    String set_output_info(String s, uint8_t *pos);
    String set_output_info(String s, String *pos);
    String set_output_info(String s, bool *pos);
    String get_output_info(mod_outputs_type *pos, int count);
    String get_output_info(mod_devices_room *pos, int count);
    String get_output_info(mod_devices_grp *pos, int count);
    String get_output_info(String *pos, int count);
    String get_output_info(bool *pos, int count);
    String get_output_info(int *pos, int count);
    String get_output_info(uint8_t *pos, int count);

    #define MAX_GROUPS          10
    #define MAX_PARAM           5

    class devices_edit {
    public: 
        String  parametres[MAX_PARAM];
        String  groupes[MAX_GROUPS];
        String  rooms[MAX_GROUPS];
        int     grp_count;
        int     room_count;
        devices_edit();
        void    room_setup();
        void    group_setup();
        void    group_display();
        void    room_display();
        void    parametres_display();
        void    parametres_add(String s_value, String s_op) ;
        void    parametres_remove(String s_value, String s_op) ;
        int     parametres_getPos(String s_op);
        int     parametres_spiffToArray();
    };

    void devices_edit_setup();


    #ifdef DEVICEFROMSPIFF
        class deviceSpiff {
        public: 
            String      name;
            deviceSpiff(String add);
        };
        extern int deviceSpiff_array_nbr;
        extern deviceSpiff * deviceSpiff_array[];

        void    deviceSpiff_write();
        int     deviceSpiff_search(String dn);
        void    deviceSpiff_print();
        void    deviceSpiff_load();
        void    deviceSpiff_addDevice(String dn);
        void    deviceSpiff_addData(String dn, String data);
    #endif

    class devices {
    public:

        boolean         isAdded = false;
        boolean         isConnected;
        unsigned long   isConnected_lastTime;      
        int             isConnectedCount = 0;

        String device_lib;
        String device_name;
        String device_ip;

        int                 output_count = 0;//
        int                 output_pin[MAX_OUTPUTS];
        mod_outputs_type    output_type[MAX_OUTPUTS];

        String              device_room[MAX_OUTPUTS];
        String              output_grp[MAX_OUTPUTS];
        String              output_location[MAX_OUTPUTS];

        bool    toggleOnOff[MAX_OUTPUTS]; 
        bool    trueWhite[MAX_OUTPUTS]; 
        // int     output_state[MAX_OUTPUTS]; 

        int     output_selected[MAX_OUTPUTS];
        // int     patternStrip_selected[MAX_OUTPUTS];

        // int     strip_size[MAX_OUTPUTS];
        String  strip_color[MAX_OUTPUTS];
        // String  strip_colorOff[MAX_OUTPUTS];
        uint8_t strip_bri[MAX_OUTPUTS];

        uint8_t strip_hue[MAX_OUTPUTS];
        uint8_t strip_sat[MAX_OUTPUTS];
        uint8_t strip_hBri[MAX_OUTPUTS];


    	devices();
        void    addDevice(String dn, String str);
        int     getListPos(String dn);
        boolean deviceIsListed(String dn);
        String  getLib(String dn);

        void    deviceIsConnected(String dn, bool state);
        void    upd_udpDevices();
        int     geOutputPosByName(int dPos, String search);

        

    };
    extern devices devices_array[];
    extern int devices_count;
    extern int number_of_devices;

    boolean output_isRelay(mod_outputs_type type);
    boolean output_isStrip(mod_outputs_type type);
    boolean output_isRGBWStrip(mod_outputs_type type) ;
    boolean output_isRGBStrip(mod_outputs_type type);

    #ifdef OLED_UPD_WEBSOCKET
        struct oled_requests {
            int cnt;
            boolean send;
            String device[MAX_DEVICES][2];

        };  

        int     oled_request_search_device(String search);
        void    oled_request_upd_device(String search);
        boolean oled_request_canSend();
        void    oled_request_start();
        void    oled_request_clear();
        String  oled_request_sendString();
        void    oled_request_end();
        int     oled_request_cnt();
    #endif


    String device_setOp(String dn);
    char * device_set_req(char * cmd);
    String device_setVal(String dn, char * cmd, String val, int pos);
    String device_setVal(char * cmd, String val);
    String c_to_string(char * name);
    String request_set_format(char * cmd, String value, String e, String s);
    String request_set_format(char * cmd, String e, String s);
        
#endif

    // int device_pos;

    // int udp_msgCount = 0;
    // String udp_msg[MAX_OUTPUTS][4];

    // bool isConnected;
    // long isConnected_lastTime;
    // int udp_notConnected = 0;
    // bool udp_open;

    // String deviceName;//
    // String boardName;//
    // IPAddress ip;//

    // mod_devices_room device_room[MAX_OUTPUTS];
    // String requestName[MAX_OUTPUTS];

    // int output_count = 0;//
    // int output_pin[MAX_OUTPUTS];
    // mod_outputs_type output_type[MAX_OUTPUTS];

    // int output_selected[MAX_OUTPUTS];
    // int patternStrip_selected[MAX_OUTPUTS];
    // mod_devices_grp output_grp[MAX_OUTPUTS];
    // String output_location[MAX_OUTPUTS];

    // bool toggleOnOff[MAX_OUTPUTS]; 
    // int output_state[MAX_OUTPUTS]; 

    // int strip_size[MAX_OUTPUTS];
    // String strip_ledChipset[MAX_OUTPUTS];
    // String strip_color[MAX_OUTPUTS];
    // String strip_colorOff[MAX_OUTPUTS];
    // int strip_bri[MAX_OUTPUTS];
    // String strip_colorOrder[MAX_OUTPUTS];
