//
// Created by Eduard on 22/11/2026.
//

#ifndef DRONE_IN_A_BOX_ISERVO_H
#define DRONE_IN_A_BOX_ISERVO_H
#pragma once

class IServo {
public:
    virtual ~IServo() = default;
    virtual void setAngle(int degrees) = 0;
};

#endif //DRONE_IN_A_BOX_ISERVO_H
