//
// Created by Eduard on 22/11/2026.
//

#include <unity.h>
#include "EnvMonitorTask.h"
#include "SafetyPolicy.h"

// ---------------------------------------------------------------------------
// Test double: a sensor that returns the reading we decide
// ---------------------------------------------------------------------------

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

// Thresholds used by all tests: 0..40 °C, humidity at most 85 %
static const SafetyPolicy policy(0, 40, 85);

static EnvReading reading(int temperature, int humidity) {
    EnvReading r;
    r.temperature = temperature;
    r.humidity = humidity;
    return r;
}

static void tickN(EnvMonitorTask& task, int n) {
    for (int i = 0; i < n; i++) task.tick();
}

void setUp() {}
void tearDown() {}

// ---------------------------------------------------------------------------
// SafetyPolicy
// ---------------------------------------------------------------------------

void test_policy_values_in_range_are_safe() {
    TEST_ASSERT_TRUE(policy.isSafe(reading(20, 50)));
}

void test_policy_temperature_below_min_is_unsafe() {
    TEST_ASSERT_FALSE(policy.isSafe(reading(-1, 50)));
}

void test_policy_temperature_above_max_is_unsafe() {
    TEST_ASSERT_FALSE(policy.isSafe(reading(41, 50)));
}

void test_policy_humidity_above_max_is_unsafe() {
    TEST_ASSERT_FALSE(policy.isSafe(reading(20, 86)));
}

void test_policy_limits_are_safe() {
    TEST_ASSERT_TRUE(policy.isSafe(reading(0, 50)));    // temperature = min
    TEST_ASSERT_TRUE(policy.isSafe(reading(40, 50)));   // temperature = max
    TEST_ASSERT_TRUE(policy.isSafe(reading(20, 85)));   // humidity = max
}

// ---------------------------------------------------------------------------
// EnvMonitorTask
// ---------------------------------------------------------------------------

void test_monitor_starts_unsafe() {
    HangarBlackBoard bb;
    bb.setEnvOk(true);   // the constructor must overwrite it
    FakeEnvSensor sensor;
    EnvMonitorTask task(bb, sensor, policy);

    TEST_ASSERT_FALSE(bb.isEnvOk());
}

void test_monitor_becomes_safe_after_three_good_readings() {
    HangarBlackBoard bb;
    FakeEnvSensor sensor;
    EnvMonitorTask task(bb, sensor, policy);
    sensor.willReturn(20, 50);

    tickN(task, 2);
    TEST_ASSERT_FALSE(bb.isEnvOk());

    tickN(task, 1);
    TEST_ASSERT_TRUE(bb.isEnvOk());
}

void test_monitor_becomes_unsafe_after_two_bad_readings() {
    HangarBlackBoard bb;
    FakeEnvSensor sensor;
    EnvMonitorTask task(bb, sensor, policy);
    sensor.willReturn(20, 50);
    tickN(task, 3);   // now safe

    sensor.willReturn(20, 95);
    tickN(task, 1);
    TEST_ASSERT_TRUE(bb.isEnvOk());

    tickN(task, 1);
    TEST_ASSERT_FALSE(bb.isEnvOk());
}

void test_monitor_ignores_single_bad_reading() {
    HangarBlackBoard bb;
    FakeEnvSensor sensor;
    EnvMonitorTask task(bb, sensor, policy);
    sensor.willReturn(20, 50);
    tickN(task, 3);   // now safe

    sensor.willReturn(20, 95);
    tickN(task, 1);
    sensor.willReturn(20, 50);
    tickN(task, 1);
    TEST_ASSERT_TRUE(bb.isEnvOk());
}

void test_monitor_failed_reading_counts_as_unsafe() {
    HangarBlackBoard bb;
    FakeEnvSensor sensor;
    EnvMonitorTask task(bb, sensor, policy);
    sensor.willReturn(20, 50);
    tickN(task, 3);   // now safe

    sensor.willFail();
    tickN(task, 2);
    TEST_ASSERT_FALSE(bb.isEnvOk());
}

void test_monitor_failed_reading_keeps_last_values() {
    HangarBlackBoard bb;
    FakeEnvSensor sensor;
    EnvMonitorTask task(bb, sensor, policy);
    sensor.willReturn(22, 48);
    tickN(task, 1);

    sensor.willFail();
    tickN(task, 1);
    TEST_ASSERT_EQUAL(22, bb.getTemperature());
    TEST_ASSERT_EQUAL(48, bb.getHumidity());
}

void test_monitor_valid_reading_updates_values() {
    HangarBlackBoard bb;
    FakeEnvSensor sensor;
    EnvMonitorTask task(bb, sensor, policy);

    sensor.willReturn(22, 48);
    tickN(task, 1);
    TEST_ASSERT_EQUAL(22, bb.getTemperature());
    TEST_ASSERT_EQUAL(48, bb.getHumidity());

    sensor.willReturn(25, 60);
    tickN(task, 1);
    TEST_ASSERT_EQUAL(25, bb.getTemperature());
    TEST_ASSERT_EQUAL(60, bb.getHumidity());
}

void test_monitor_alternating_readings_never_become_safe() {
    HangarBlackBoard bb;
    FakeEnvSensor sensor;
    EnvMonitorTask task(bb, sensor, policy);

    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) sensor.willReturn(20, 50);
        else            sensor.willReturn(20, 95);
        task.tick();
        TEST_ASSERT_FALSE(bb.isEnvOk());
    }
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_policy_values_in_range_are_safe);
    RUN_TEST(test_policy_temperature_below_min_is_unsafe);
    RUN_TEST(test_policy_temperature_above_max_is_unsafe);
    RUN_TEST(test_policy_humidity_above_max_is_unsafe);
    RUN_TEST(test_policy_limits_are_safe);

    RUN_TEST(test_monitor_starts_unsafe);
    RUN_TEST(test_monitor_becomes_safe_after_three_good_readings);
    RUN_TEST(test_monitor_becomes_unsafe_after_two_bad_readings);
    RUN_TEST(test_monitor_ignores_single_bad_reading);
    RUN_TEST(test_monitor_failed_reading_counts_as_unsafe);
    RUN_TEST(test_monitor_failed_reading_keeps_last_values);
    RUN_TEST(test_monitor_valid_reading_updates_values);
    RUN_TEST(test_monitor_alternating_readings_never_become_safe);

    return UNITY_END();
}