#include "bluetooth.h"
namespace bluetooth
{
    BLEService carService("180A"); // create service: "Device Information"

    // create direction control characteristic and allow remote device to read and write
    BLEByteCharacteristic carControlCharacteristic("2A57", BLERead | BLEWrite); // 2A57 is "Digital Output"

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

        BLE.setLocalName("UnoR4 BLE Car");
        BLE.setAdvertisedService(carService);

        // add the characteristics to the service
        carService.addCharacteristic(carControlCharacteristic);

        // add the service
        BLE.addService(carService);

        carControlCharacteristic.writeValue(0);

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

                if (carControlCharacteristic.written())
                {

                    switch (carControlCharacteristic.value())
                    {
                    case 01:
                        Serial.println("LEFT");
                        break;
                    case 02:
                        Serial.println("RIGHT");
                        break;
                    case 03:
                        Serial.println("UP");
                        break;
                    case 04:
                        Serial.println("DOWN");
                        break;
                    default: // 0 or invalid control
                        Serial.println("STOP");
                        break;
                    }
                }
            }

            // when the central disconnects, print it out:
            Serial.print(F("Disconnected from controller: "));
            Serial.println(controller.address());
            digitalWrite(LED_BUILTIN, LOW); // when the central disconnects, turn off the LED
        }
    }
}