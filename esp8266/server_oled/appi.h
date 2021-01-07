#ifndef APPI_h
   #define APPI_h
   #include <arduino.h>
	void appi_send_udpRequest_dn(String dn, char * req, String val, String upd);
	void appi_send_udpRequest_mdn(String val, char * req, String dg, String upd);
#endif

      // LampeColor_string = "rgb(" + red + ", " + green + ", " + blue + ")";
