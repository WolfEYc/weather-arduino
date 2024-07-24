#include "weefee.h"

namespace weefee
{

    struct ReadFromStringRes
    {
        int current_addr;
        String out;
    };

    struct WifiCreds
    {
        String ssid;
        String pass;
    };

    int status = WL_IDLE_STATUS; // the Wifi radio's status
    byte mac[6];                 // the MAC address of your Wifi shield
    WifiCreds existing_creds;
    bool has_creds = false;

    int write_byte_to_eeprom(int current_addr, byte data)
    {
        EEPROM.write(current_addr, data);
        return current_addr + 1;
    }

    int write_string_to_eeprom(int current_addr, String str)
    {
        current_addr = write_byte_to_eeprom(current_addr, str.length());
        for (auto c : str)
        {
            current_addr = write_byte_to_eeprom(current_addr, c);
        }
        return current_addr;
    }

    ReadFromStringRes read_string_from_eeprom(int current_addr)
    {
        byte size = EEPROM.read(current_addr);
        char *str = new char[size];

        for (int i = 0; i < size; i++)
        {
            byte c = EEPROM.read(i + current_addr);
            str[i] = c;
        }
        String arduino_str = String(str);
        ReadFromStringRes res;
        res.current_addr = current_addr + size + 1;
        res.out = arduino_str;
        return res;
    }

    void wipe_eeprom()
    {
        for (int i = 0; i < EEPROM.length(); i++)
        {
            EEPROM.write(i, 0);
        }
    }

    void write_wifi_creds_to_eeprom(WifiCreds new_creds)
    {
        int current_addr = 0;
        wipe_eeprom();
        current_addr = write_string_to_eeprom(current_addr, new_creds.ssid);
        current_addr = write_string_to_eeprom(current_addr, new_creds.pass);
    }

    bool does_wifi_creds_exist_on_eeprom()
    {
        auto ssid_size = EEPROM.read(0);
        return ssid_size != 0;
    }

    WifiCreds read_wifi_creds_from_eeprom()
    {
        WifiCreds new_creds;
        int current_addr = 0;
        auto ssid_res = read_string_from_eeprom(current_addr);
        current_addr = ssid_res.current_addr;
        new_creds.ssid = ssid_res.out;
        auto pass_res = read_string_from_eeprom(current_addr);
        current_addr = pass_res.current_addr;
        new_creds.pass = pass_res.out;
        return new_creds;
    }

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

    void setRandomSeed()
    {
        uint64_t seed = 0;
        for (int i = 0; i < 6; i++)
        {
            seed |= ((uint64_t)mac[i]) << (8 * i);
        }
        randomSeed(seed);
    }

    void on_connected(WifiCreds new_creds)
    {
        // you're connected now, so print out the data:
        Serial.print("You're connected to the network");
        printCurrentNet();
        printWifiData();
        setRandomSeed();
        write_wifi_creds_to_eeprom(new_creds);
    }

    int connect_to_wifi(WifiCreds new_creds)
    {

        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(new_creds.ssid);
        // Connect to WPA/WPA2 network:
        status = WiFi.begin(new_creds.ssid.c_str(), new_creds.pass.c_str());

        if (status == WL_CONNECTED)
        {
            on_connected(new_creds);
        }
        else
        {
            Serial.println("WIFI Connection Failed, please try again");
        }
        return status;
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
            Serial.println("Please upgrade the WiFi firmware");
        }

        // Initialize EEPROM
        if (!EEPROM.begin())
        {
            Serial.println("Failed to initialize EEPROM");
            while (true)
                ;
        }

        if (does_wifi_creds_exist_on_eeprom())
        {
            auto new_creds = read_wifi_creds_from_eeprom();
            connect_to_wifi(new_creds);
            has_creds = true;
            existing_creds = new_creds;
        }
    }

    unsigned long last_millis = 0;
    unsigned long wifi_check_delay = 10000;

    void loop()
    {
        auto new_millis = millis();
        auto diff = new_millis - last_millis;
        auto abs_diff = abs(diff);

        if (abs_diff > wifi_check_delay && has_creds && WiFi.status() != WL_CONNECTED)
        {
            connect_to_wifi(existing_creds);
            last_millis = millis();
        }
    }
}