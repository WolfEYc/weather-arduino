#ifndef WEATHER
#define WEATHER

#include <DHT.h>
#define DHTTYPE DHT22

#define DHTPIN 13

namespace weather
{
    void setup();
    void loop();
}

#endif
