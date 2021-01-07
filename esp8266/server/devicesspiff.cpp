#include "devicesspiff.h"

#include "def.h"
#include "devicesmodule.h"

#include <adri_tools.h>



    deviceSpiff_item * deviceSpiff_array[MAX_DEVICES];
    int deviceSpiff_array_nbr = 0;

    deviceSpiff_item::deviceSpiff_item(String add){   
        if (deviceSpiff_array_nbr < MAX_DEVICES) {
            name = add;
            deviceSpiff_array[deviceSpiff_array_nbr++] = this;
            devices_count = deviceSpiff_array_nbr;
        } 
    }

    devicesspiff * devicesspiffPtr;
    devicesspiff * devicesspiffPtr_get(){return devicesspiffPtr;}
    devicesspiff::devicesspiff(){
        devicesspiffPtr = this;
    }
    int devicesspiff::deviceSpiff_toArray() {

        String  path = "/devcices_save.txt";
        File    file = SPIFFS.open(path, "r");

        if (!file) return-1;

        int     countParam  = 0;
        String  xml;

        while (file.position()<file.size()) {
            xml = file.readStringUntil('\n');
            if (xml != "") {

                new deviceSpiff_item(xml);

                #ifdef WEBSERVER_ENABLE
                    new webSocketUpdate(xml);
                #endif

                int pos = deviceSpiff_array_nbr - 1;
                devices_array[pos].device_name = literal_value("dn", deviceSpiff_array[pos]->name);
                devices_array[pos].isConnected = false;
                devices_array[pos].isConnected_lastTime = millis();
                countParam++;
            }
            yield();
        } 
        file.close();
        return countParam;
    }

    void devicesspiff::deviceSpiff_write() {

        #ifdef DEBUG
            Serial.printf("\n[deviceSpiff_write]\n");
        #endif

        String  path = "/devcices_save.txt";
        File    file = SPIFFS.open(path, "w");
        if (file) { 
            String lines = "";
            for( int j = 0; j < deviceSpiff_array_nbr; j++) {

                String l_n = deviceSpiff_array[j]->name;

                #ifdef DEBUG
                    Serial.printf("write line - pos: %d - name: %s\n", j, l_n.c_str());
                #endif
                    
                if ( j < deviceSpiff_array_nbr -1 )  lines += l_n + "\n";
                else lines += l_n;

            }      

            file.print(lines);   
            file.close();        
        }
        #ifdef DEBUG
            Serial.printf("[deviceSpiff_write] END\n");
        #endif
    }

    int devicesspiff::deviceSpiff_search(String dn){
        int cnt = -1;
        for( int j = 0; j < deviceSpiff_array_nbr; j++) {
            String l_n = literal_value("dn", deviceSpiff_array[j]->name);
            if (dn==l_n)  {cnt=j;}
        }
        return cnt;
    }

    void devicesspiff::deviceSpiff_print(){
        Serial.printf("\n[deviceSpiff_print] cnt : %d\n", deviceSpiff_array_nbr);
        for( int j = 0; j < deviceSpiff_array_nbr; j++) {
            String l_n = deviceSpiff_array[j]->name;
            Serial.printf("pos: %d - name: %s\n", j, l_n.c_str());
        }
        Serial.printf("[deviceSpiff_print] END\n");
    }

    void devicesspiff::deviceSpiff_load(){
        Serial.printf("\n[deviceSpiff_load]\n");

        deviceSpiff_toArray();

        Serial.printf("[deviceSpiff_load] END\n");

        deviceSpiff_print();
    }

    void devicesspiff::deviceSpiff_addDevice(String dn){

        int search = deviceSpiff_search(dn);
        if (search < 0) {
            String name = literal_item("dn", dn);
            new deviceSpiff_item(name);

            #ifdef WEBSERVER_ENABLE
                new webSocketUpdate(name);
            #endif

            deviceSpiff_write();
        }

    }

    void devicesspiff::deviceSpiff_addData(String dn, String data){

        int pos = deviceSpiff_search(dn);
        if (pos >= 0) { output_editPtr_get()->device_add(data, pos); }
    }