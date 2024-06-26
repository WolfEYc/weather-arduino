#include "weefee.h"

namespace weefee
{
    char ssid[] = SECRET_SSID; // your network SSID (name)
    char pass[] = SECRET_PASS; // your network password (use for WPA, or use as key for WEP)

    int status = WL_IDLE_STATUS; // the Wifi radio's status
    byte mac[6];                 // the MAC address of your Wifi shield

    void printMacAddress(byte mac[])
    {
        for (int i = 0; i < 6; i++)
        {
            if (i > 0)
            {
                Serial.print(":");
            }
            if (mac[i] < 16)
            {
                Serial.print("0");
            }
            Serial.print(mac[i], HEX);
        }
        Serial.println();
    }

    void printWifiData()
    {
        // print your board's IP address:
        IPAddress ip = WiFi.localIP();
        Serial.print("IP Address: ");

        Serial.println(ip);

        WiFi.macAddress(mac);
        Serial.print("MAC address: ");
        printMacAddress(mac);
    }

    void printCurrentNet()
    {
        // print the SSID of the network you're attached to:
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());

        // print the MAC address of the router you're attached to:
        byte bssid[6];
        WiFi.BSSID(bssid);
        Serial.print("BSSID: ");
        printMacAddress(bssid);

        // print the received signal strength:
        long rssi = WiFi.RSSI();
        Serial.print("signal strength (RSSI):");
        Serial.println(rssi);

        // print the encryption type:
        byte encryption = WiFi.encryptionType();
        Serial.print("Encryption Type:");
        Serial.println(encryption, HEX);
        Serial.println();
    }

    void setup()
    {
        if (WiFi.status() == WL_NO_MODULE)
        {
            Serial.println("Communication with WiFi module failed!");
            // don't continue
            while (true)
                ;
        }

        String fv = WiFi.firmwareVersion();
        if (fv < WIFI_FIRMWARE_LATEST_VERSION)
        {
            Serial.println("Please upgrade the firmware");
        }

        while (status != WL_CONNECTED)
        {
            Serial.print("Attempting to connect to WPA SSID: ");
            Serial.println(ssid);
            // Connect to WPA/WPA2 network:
            status = WiFi.begin(ssid, pass);

            // wait 10 seconds for connection:
            delay(10000);
        }

        // you're connected now, so print out the data:
        Serial.print("You're connected to the network");
        printCurrentNet();
        printWifiData();
    }

    void loop()
    {
    }
}