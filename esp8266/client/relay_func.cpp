#include "devices.h"
#include "device_def.h"
#include "relay_func.h"



void relay_setup(){
	Serial.printf("relay_setup\n");
	for( int j = 0; j < output_count; j++) {
		if (output_type[j] == OT_RELAY_AC) {pinMode(output_pin[j], OUTPUT);}
		if (output_type[j] == OT_RELAY_LP) {pinMode(output_pin[j], OUTPUT);}
	}      	
}

int relay_statu_get(int pin){
	int statu = digitalRead(pin);
	return statu;
}
boolean relay_statu_isOn(int pin){
	int statu = digitalRead(pin);
	if (statu == OUTPUT_ON) return true;
	else 					return false;
}

void relay_set_on(int pin){
	Serial.printf("relay_set_on\n");
	digitalWrite(output_pin[pin], OUTPUT_ON);
}

void relay_set_off(int pin){
	Serial.printf("relay_set_off\n");
	digitalWrite(output_pin[pin], OUTPUT_OFF);
}

void relay_set_toggle(int pin){
	if (digitalRead(output_pin[pin]) == OUTPUT_ON) 	digitalWrite(output_pin[pin], OUTPUT_OFF);
	else 											digitalWrite(output_pin[pin], OUTPUT_ON);
}