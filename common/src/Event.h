//
// Created by aedua on 11/23/2026.
//

#ifndef DRONE_IN_A_BOX_EVENT_H
#define DRONE_IN_A_BOX_EVENT_H

template <typename T>
class Event
{
    public:
        void post(const T& value)
        {
            this->value = value;
            pending = true;
        }

        bool consume(T& out)
        {
            if (!pending)
            {
                return false;
            }

            out = value;
            pending = false;
            return true;
        }

        bool isPending() const {return pending;}

private:
    bool pending = false;
    T value{};
};

#endif //DRONE_IN_A_BOX_EVENT_H
