#include "bluetooth.h"
namespace bluetooth
{
#define BUFF_SIZE 4096
#define DEVICE_PREFIX "Thermostatter "
#define SUFFIX_SIZE 6

    String generate_name()
    {
        auto suffix = String(SUFFIX_SIZE);
        for (int i = 0; i < SUFFIX_SIZE; i++)
        {
            suffix[i] = '0' + random(10);
        }

        return DEVICE_PREFIX + suffix;
    }

    BLEService thermostat_service("180A"); // create service: "Device Information"

    // create direction control characteristic and allow remote device to read and write
    BLEStringCharacteristic thermostat_characteristic("2a57", BLERead | BLEWrite, BUFF_SIZE); // 2A57 is "Digital Output"

    void setup()
    {

        pinMode(LED_BUILTIN, OUTPUT); // use the LED as an output

        // begin initialization
        if (!BLE.begin())
        {
            Serial.println("starting Bluetooth® Low Energy module failed!");
            while (1)
            { // blink the built-in LED fast to indicate an issue
                digitalWrite(LED_BUILTIN, HIGH);
                delay(100);
                digitalWrite(LED_BUILTIN, LOW);
                delay(100);
            }
        }

        auto name = generate_name();
        Serial.print("set name to: ");
        Serial.println(name);

        BLE.setDeviceName(name.c_str());
        BLE.setLocalName(name.c_str());

        BLE.setAdvertisedService(thermostat_service);

        // add the characteristics to the service
        thermostat_service.addCharacteristic(thermostat_characteristic);

        // add the service
        BLE.addService(thermostat_service);

        // start advertising
        BLE.advertise();

        Serial.println("Bluetooth® device active, waiting for connections...");
    }

    void loop()
    {
        // listen for BLE peripherals to connect:
        BLEDevice controller = BLE.central();

        // if a central is connected to peripheral:
        if (controller)
        {
            Serial.print("Connected to controller: ");
            // print the controller's MAC address:
            Serial.println(controller.address());
            digitalWrite(LED_BUILTIN, HIGH); // turn on the LED to indicate the connection

            // while the controller is still connected to peripheral:
            while (controller.connected())
            {
                if (!thermostat_characteristic.written())
                {
                    continue;
                }
                auto value = thermostat_characteristic.value();
                Serial.println(value);
                thermostat_characteristic.writeValue("echo: " + value);
            }

            // when the central disconnects, print it out:
            Serial.print(F("Disconnected from controller: "));
            Serial.println(controller.address());
            digitalWrite(LED_BUILTIN, LOW); // when the central disconnects, turn off the LED
        }
    }
}