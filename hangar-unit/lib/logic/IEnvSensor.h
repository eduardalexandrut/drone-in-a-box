//
// Created by Eduard on 22/11/2026.
//

#ifndef DRONE_IN_A_BOX_IENVSENSOR_H
#define DRONE_IN_A_BOX_IENVSENSOR_H
#include "EnvReading.h"

class IEnvSensor {
public:
    virtual ~IEnvSensor() = default;
    virtual bool read(EnvReading& reading) = 0;
};
#endif //DRONE_IN_A_BOX_IENVSENSOR_H
