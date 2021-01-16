#ifndef DEVICESSPIFF_H
#define DEVICESSPIFF_H 

    #include <arduino.h>
    #include <LittleFS.h>  


    class deviceSpiff_item {
    public: 
        String      name;
        deviceSpiff_item(String add);
    };
    extern int deviceSpiff_array_nbr;
    extern deviceSpiff_item * deviceSpiff_array[];

    class devicesspiff
    {
    public:
        devicesspiff();
        ~devicesspiff();
         
        void    deviceSpiff_write();
        int     deviceSpiff_search(String dn);
        void    deviceSpiff_print();
        void    deviceSpiff_load();
        void    deviceSpiff_addDevice(String dn);
        void    deviceSpiff_addData(String dn, String data);  
        int     deviceSpiff_toArray() ;     
    };
    devicesspiff * devicesspiffPtr_get();

#endif