#ifndef GPS_H
#define GPS_H

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

namespace gps
{
    static const int GPS_TX_PIN = 9, GPS_RX_PIN = 8; // pins for ATGM336H GPS device
    static const uint32_t GPSBaud = 9600;            // default baudrate of ATGM336H GPS device
    void setup();
    void loop();
}

#endif
