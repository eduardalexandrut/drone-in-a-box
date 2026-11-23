//
// Created by Eduard on 22/11/2026.
//

#ifndef DRONE_IN_A_BOX_SAFETYPOLICY_H
#define DRONE_IN_A_BOX_SAFETYPOLICY_H
class SafetyPolicy {
    public:
    SafetyPolicy(int minTemperature, int maxTemperature, int maxHumidity)
        : minTemperature(minTemperature),
          maxTemperature(maxTemperature),
          maxHumidity(maxHumidity) {}

    bool isSafe(const EnvReading& reading) const {
        return reading.temperature >= minTemperature
            && reading.temperature <= maxTemperature
            && reading.humidity <= maxHumidity;
    }

    private:
    int minTemperature;
    int maxTemperature;
    int maxHumidity;
};
#endif //DRONE_IN_A_BOX_SAFETYPOLICY_H
