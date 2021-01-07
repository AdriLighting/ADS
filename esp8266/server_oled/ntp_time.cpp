
#include "config_base.h"

#ifdef NTP_TIME
	#include <TimeLib.h>
	#include <ESP8266WiFi.h>
	#include <WiFiUdp.h>
	#include "ntp_time.h"
	#include "tools.h"
	#include "udp.h"
	#ifdef TFT_ENABLE
		#include "tft_display.h"
	#endif
	#ifdef OW_METEO
		#include "meteo.h"
	#endif
    #ifdef DHT_ENABLE
        #include "meteo_dht.h"
    #endif 

	static const    char            ntpServerName[]     = "fr.pool.ntp.org";    // NTP Servers:
	const           int             timeZone            = 0;                    // Central European Time
	                unsigned int    localPort           = 8888;                 // local port to listen for UDP packets
	                WiFiUDP         Udp;
	                time_t          prevDisplay         = 0; 
	                boolean         ntp_time_display = true;
	                unsigned long   ntp_time_diplay_last;                
	void sendNTPpacket(IPAddress &address);
	String  CurTimeString();
	String  CurDateString();

void ntp_time_init(){
    Udp.begin(localPort);
    setSyncProvider(getNtpTime);
    setSyncInterval(10000);	
}	

bool IsDst(int hour, int day, int month, int dow)  //north american dst  dow 0=SUN
{
    // if (WebServer.daylight == false) return false; //option to disable DST

                                                     //January, february, and december are out.
    if (month < 3 || month > 11) { return false; }
    //April to October are in
    if (month > 3 && month < 11) { return true; }
    int previousSunday = day - dow;
    //In march, we are DST if our previous sunday was on or after the 8th.
    if (month == 3) { return previousSunday >= 8; }
    //In november we must be before the first sunday to be dst.
    //That means the previous sunday must be before the 1st.
    return previousSunday <= 0;
}


/*-------- NTP code ----------*/
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime() {

    IPAddress ntpServerIP; // NTP server's ip address

    while (Udp.parsePacket() > 0) ; // discard any previously received packets
    // Serial.println("Transmit NTP Request");
    // get a random server from the pool
    WiFi.hostByName(ntpServerName, ntpServerIP);
    Serial.print(ntpServerName);
    Serial.print(": ");
    Serial.println(ntpServerIP);
    sendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
        // Serial.println("Receive NTP Response");
        Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
        unsigned long secsSince1900;
        // convert four bytes starting at location 40 to a long integer
        secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
        secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
        secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
        secsSince1900 |= (unsigned long)packetBuffer[43];
        // return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;

        secsSince1900 = secsSince1900 - 2208988800UL + (timeZone * SECS_PER_HOUR);
        if (IsDst(hour(secsSince1900), day(secsSince1900), month(secsSince1900), dayOfWeek(secsSince1900) - 1)) secsSince1900 += 3600;  //add hour if DST           
        return secsSince1900;
    }
  }
  // Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
 
void time_sendToUdp(){
	String s_temp;
	String s_hum;

	#ifdef DHT_ENABLE
        float h = dht_get_humidity();
        float t = dht_get_temperature();
        s_temp = literal_item("temperature", String(t) );
        s_hum = literal_item("relative_humidity", String(h) );    
	#else
		#ifdef OW_METEO
				String temp = String(currentWeather.temp, 1) ;
				String hum = String(currentWeather.humidity) ;		
				s_temp = literal_item("temperature", temp );
				s_hum = literal_item("relative_humidity", hum );		
			#else
				s_temp = literal_item("temperature", "" );
				s_hum = literal_item("relative_humidity", "" );
		#endif			
	#endif
		
	String s_hour 	= literal_item("hour", 		String(hour()) 		);
	String s_minute	= literal_item("minute", 	String(minute())	);
	String s_second	= literal_item("second", 	String(second())	);

	String send;
	send +=	literal_item("op", "realtime");
	send += s_hour;
	send += s_minute;
	send += s_second;
	send += s_temp;
	send += s_hum;
	udpMulti_send("2", send);
}
#endif
  // Calcul la température ressentie. Il calcul est effectué à partir de la température en Fahrenheit
  // On fait la conversion en Celcius dans la foulée
  // float hi = dht.computeHeatIndex(f, h);
  

  // Serial.print("Humidite: "); 
  // Serial.print(h);
  // Serial.print(" %\t");
  // Serial.print("Temperature: "); 
  // Serial.print(t);
  // Serial.print(" *C ");
  // Serial.print("Temperature ressentie: ");
  // Serial.print(dht.convertFtoC(hi));
  // Serial.println(" *C");