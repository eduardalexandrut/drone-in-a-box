//
// Created by Eduard on 24/11/2026.
//

#include "OverridableEnvSensor.h"
#include <unity.h>

#include "EnvMonitorTask.h"
#include "HangarBlackBoard.h"
#include "SafetyPolicy.h"

// Stand-in for the real DHT11: returns the values the test decides
class FakeEnvSensor : public IEnvSensor {
public:
    void willReturn(int temperature, int humidity) {
        next.temperature = temperature;
        next.humidity = humidity;
        fail = false;
    }

    void willFail() { fail = true; }

    bool read(EnvReading& reading) override {
        if (fail) return false;
        reading = next;
        return true;
    }

private:
    EnvReading next;
    bool fail = false;
};

static const int REAL_TEMP = 22;
static const int REAL_HUM = 48;
static const int FORCED_TEMP = 20;
static const int FORCED_HUM = 95;

void setUp() {}
void tearDown() {}

void test_env_passes_real_values_by_default() {
    FakeEnvSensor real;
    real.willReturn(REAL_TEMP, REAL_HUM);
    OverridableEnvSensor sensor(real);
    EnvReading reading;

    const bool ok = sensor.read(reading);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_INT(REAL_TEMP, reading.temperature);
    TEST_ASSERT_EQUAL_INT(REAL_HUM, reading.humidity);

}

void test_env_force_replaces_real_values() {
    FakeEnvSensor real;
    real.willReturn(REAL_TEMP, REAL_HUM);
    OverridableEnvSensor sensor(real);
    EnvReading reading;

    sensor.force(FORCED_TEMP, FORCED_HUM);
    const bool ok = sensor.read(reading);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_INT(FORCED_TEMP, reading.temperature);
    TEST_ASSERT_EQUAL_INT(FORCED_HUM, reading.humidity);

}

void test_env_clear_restores_real_values() {
    FakeEnvSensor real;
    real.willReturn(REAL_TEMP, REAL_HUM);
    OverridableEnvSensor sensor(real);
    EnvReading reading;
    sensor.force(FORCED_TEMP, FORCED_HUM);

    sensor.clear();
    const bool ok = sensor.read(reading);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_INT(REAL_TEMP, reading.temperature);
    TEST_ASSERT_EQUAL_INT(REAL_HUM, reading.humidity);
}

void test_env_force_works_when_real_sensor_fails() {
    FakeEnvSensor real;
    real.willFail();
    OverridableEnvSensor sensor(real);
    EnvReading reading;

    sensor.force(FORCED_TEMP, FORCED_HUM);
    const bool ok = sensor.read(reading);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_INT(FORCED_TEMP, reading.temperature);
    TEST_ASSERT_EQUAL_INT(FORCED_HUM, reading.humidity);
}

void test_forced_values_go_through_the_monitor_filter() {
    HangarBlackBoard bb;
    FakeEnvSensor real;
    real.willReturn(REAL_TEMP, REAL_HUM);
    OverridableEnvSensor sensor(real);
    SafetyPolicy policy(0, 40, 85);
    EnvMonitorTask monitor(bb, sensor, policy);
    for (int i = 0; i < 3; i++) monitor.tick();   // now safe
    TEST_ASSERT_TRUE(bb.isEnvOk());

    sensor.force(FORCED_TEMP, FORCED_HUM);        // humidity 95 % > 85 %
    monitor.tick();
    TEST_ASSERT_TRUE(bb.isEnvOk());               // one bad reading is not enough

    monitor.tick();
    TEST_ASSERT_FALSE(bb.isEnvOk());              // the second one is
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_env_passes_real_values_by_default);
    RUN_TEST(test_env_force_replaces_real_values);
    RUN_TEST(test_env_clear_restores_real_values);
    RUN_TEST(test_env_force_works_when_real_sensor_fails);
    RUN_TEST(test_forced_values_go_through_the_monitor_filter);
    return UNITY_END();
}
