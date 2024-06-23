#ifndef WEEFEE_H
#define WEEFEE_H

#include <SPI.h>
#include <WiFi.h>

#define SECRET_SSID "Arlo_Network"
#define SECRET_PASS "M0re$4me"

namespace weefee
{
    void setup();
    void loop();
}

#endif