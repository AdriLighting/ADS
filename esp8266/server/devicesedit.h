#ifndef DEVICESEDIT_H
	#define DEVICESEDIT_H 

    #include <arduino.h>
    #include <LittleFS.h>   


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

#endif