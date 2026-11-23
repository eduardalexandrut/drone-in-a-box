//
// Created by aedua on 11/23/2026.
//

#ifndef DRONE_IN_A_BOX_IPRESENCESENSOR_H
#define DRONE_IN_A_BOX_IPRESENCESENSOR_H
#pragma once

class IPresenceSensor
{
public:
    virtual ~IPresenceSensor() = default;
    virtual bool isDetected() = 0;
};

#endif //DRONE_IN_A_BOX_IPRESENCESENSOR_H

