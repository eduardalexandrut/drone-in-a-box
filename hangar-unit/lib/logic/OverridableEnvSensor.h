//
// Created by Eduard on 24/11/2026.
//

#ifndef DRONE_IN_A_BOX_OVERRIDABLEENVSENSOR_H
#define DRONE_IN_A_BOX_OVERRIDABLEENVSENSOR_H
#include "IEnvSensor.h"

class OverridableEnvSensor: public IEnvSensor {
    public:
        explicit OverridableEnvSensor(IEnvSensor& realSensor):
            realSensor(realSensor) {};

        void force(int temperature, int humidity) {
            this->temperature = temperature;
            this->humidity = humidity;
            forced = true;
        };

        void clear() {
            this->forced = false;
        }

        bool read(EnvReading& reading) override {
            if (forced) {
                reading.temperature = temperature;
                reading.humidity = humidity;
                return true;
            }

            return this->realSensor.read(reading);
        }

private:
    IEnvSensor& realSensor;
    int temperature = 0;
    int humidity = 0;
    bool forced = false;
};

#endif //DRONE_IN_A_BOX_OVERRIDABLEENVSENSOR_H
