#include <Arduino.h>
#include "heart.h"
#include "weather.h"
#include "gps.h"

const uint32_t FPS = 15;
const uint32_t DELAY_MS = 1000 / FPS;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    heart::setup();
    weather::setup();
    gps::setup();
}

void loop()
{
    // heart::loop();
    // weather::loop();
    gps::loop();
    // delay(DELAY_MS);
}