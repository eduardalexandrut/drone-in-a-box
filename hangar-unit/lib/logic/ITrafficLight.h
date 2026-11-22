//
// Created by Eduard on 22/11/2026.
//

#ifndef DRONE_IN_A_BOX_ITRAFFICLIGHT_H
#define DRONE_IN_A_BOX_ITRAFFICLIGHT_H

class ITrafficLight {
public:
    virtual ~ITrafficLight() = default;
    virtual set(bool green, bool yellow, bool red) = 0;
};

#endif //DRONE_IN_A_BOX_ITRAFFICLIGHT_H
