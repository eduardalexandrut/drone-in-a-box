//
// Created by Eduard on 24/11/2026.
//

#ifndef DRONE_IN_A_BOX_IBUZZER_H
#define DRONE_IN_A_BOX_IBUZZER_H
class IBuzzer {
    public:
        virtual ~IBuzzer() = default;
        virtual void set(bool isOn) = 0;

};

#endif //DRONE_IN_A_BOX_IBUZZER_H
