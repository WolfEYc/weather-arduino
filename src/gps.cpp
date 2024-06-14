#include "gps.h"

namespace gps
{
    TinyGPSPlus gps;
    SoftwareSerial ss(GPS_RX_PIN, GPS_TX_PIN);

    void setup()
    {
        ss.begin(GPSBaud);
    }

    void displayInfo()
    {
        Serial.print(F("Location: "));
        if (gps.location.isValid())
        {
            Serial.print(gps.location.lat(), 6);
            Serial.print(F(","));
            Serial.print(gps.location.lng(), 6);
        }
        else
        {
            Serial.print(F("INVALID"));
        }

        Serial.print(F("  Date/Time: "));
        if (gps.date.isValid())
        {
            Serial.print(gps.date.month());
            Serial.print(F("/"));
            Serial.print(gps.date.day());
            Serial.print(F("/"));
            Serial.print(gps.date.year());
        }
        else
        {
            Serial.print(F("INVALID"));
        }

        Serial.print(F(" "));
        if (gps.time.isValid())
        {
            if (gps.time.hour() < 10)
                Serial.print(F("0"));
            Serial.print(gps.time.hour());
            Serial.print(F(":"));
            if (gps.time.minute() < 10)
                Serial.print(F("0"));
            Serial.print(gps.time.minute());
            Serial.print(F(":"));
            if (gps.time.second() < 10)
                Serial.print(F("0"));
            Serial.print(gps.time.second());
            Serial.print(F("."));
            if (gps.time.centisecond() < 10)
                Serial.print(F("0"));
            Serial.print(gps.time.centisecond());
        }
        else
        {
            Serial.print(F("INVALID"));
        }

        Serial.println();
    }

    void loop()
    {
        while (ss.available() > 0)
        {
            auto ss_data = ss.read();
            Serial.print("ss_data=");
            Serial.println(ss_data);
            auto encode_fl = gps.encode(ss_data);
            Serial.print("encode_fl=");
            Serial.println(encode_fl);
            if (encode_fl)
            {
                displayInfo();
            }
        }
    }
}