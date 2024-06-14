#include "weather.h"
namespace weather
{
    DHT dht(DHTPIN, DHTTYPE);

    void setup()
    {
        dht.begin();
    }

    void loop()
    {
        auto temp = dht.readTemperature(true);
        auto humidity = dht.readHumidity();
        Serial.print("temp: ");
        Serial.print(temp);
        Serial.print(" humidity: ");
        Serial.println(humidity);
    }
}