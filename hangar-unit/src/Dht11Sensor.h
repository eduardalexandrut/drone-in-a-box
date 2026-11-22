//
// Created by Eduard on 22/11/2026.
//

#ifndef DRONE_IN_A_BOX_DHT11SENSOR_H
#define DRONE_IN_A_BOX_DHT11SENSOR_H
#include "IEnvSensor.h"
#include <Arduino.h>
#include <cmath>
#include <DHT.h>
#include <math.h>

class Dht11Sensor : public IEnvSensor {
public:
    explicit Dht11Sensor(int pin):  dht(pin, DHT11){};

    void begin() {
        dht.begin();
    };

    bool read(EnvReading &reading) override {
        const float temperature = dht.readTemperature();
        const float humidity = dht.readHumidity();

        if (::isnan(temperature) || isnan(humidity)) {
            return false; // the sensor did not answer correctly
        }

        reading.temperature = lround(temperature);
        reading.humidity = lround(humidity);
        return true;
    };

private:
    DHT dht;
};
#endif //DRONE_IN_A_BOX_DHT11SENSOR_H
