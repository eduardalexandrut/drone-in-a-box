//
// Created by Eduard on 24/11/2026.
//
#include <unity.h>
#include "OverridablePresenceSensor.h"
#include "PresenceTask.h"

// Stand-in for the real IR sensor: returns the value the test decides
class FakePresenceSensor : public IPresenceSensor {
public:
    void setDetected(bool detected) { this->detected = detected; }
    bool isDetected() override { return detected; }

private:
    bool detected = false;
};

void setUp() {}
void tearDown() {}

void test_presence_passes_real_value_by_default() {
    FakePresenceSensor real;
    OverridablePresenceSensor sensor(real);

    real.setDetected(true);
    TEST_ASSERT_TRUE(sensor.isDetected());

    real.setDetected(false);
    TEST_ASSERT_FALSE(sensor.isDetected());
}

void test_presence_force_true_replaces_real_value() {
    FakePresenceSensor real;
    real.setDetected(false);
    OverridablePresenceSensor sensor(real);

    sensor.force(true);

    TEST_ASSERT_TRUE(sensor.isDetected());
}

void test_presence_force_false_replaces_real_value() {
    FakePresenceSensor real;
    real.setDetected(true);
    OverridablePresenceSensor sensor(real);

    sensor.force(false);

    TEST_ASSERT_FALSE(sensor.isDetected());
}

void test_presence_forced_value_ignores_real_changes() {
    FakePresenceSensor real;
    real.setDetected(false);
    OverridablePresenceSensor sensor(real);
    sensor.force(false);

    real.setDetected(true);

    TEST_ASSERT_FALSE(sensor.isDetected());
}

void test_presence_clear_restores_real_value() {
    FakePresenceSensor real;
    real.setDetected(true);
    OverridablePresenceSensor sensor(real);
    sensor.force(false);

    sensor.clear();

    TEST_ASSERT_TRUE(sensor.isDetected());
}

void test_forced_presence_goes_through_the_task_filter() {
    HangarBlackBoard bb;
    FakePresenceSensor real;
    real.setDetected(false);
    OverridablePresenceSensor sensor(real);
    PresenceTask task(bb, sensor);
    task.begin();
    TEST_ASSERT_FALSE(bb.isDroneDetected());

    sensor.force(true);
    task.tick();
    task.tick();
    TEST_ASSERT_FALSE(bb.isDroneDetected());   // two readings are not enough

    task.tick();
    TEST_ASSERT_TRUE(bb.isDroneDetected());    // the third one is
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_presence_passes_real_value_by_default);
    RUN_TEST(test_presence_force_true_replaces_real_value);
    RUN_TEST(test_presence_force_false_replaces_real_value);
    RUN_TEST(test_presence_forced_value_ignores_real_changes);
    RUN_TEST(test_presence_clear_restores_real_value);
    RUN_TEST(test_forced_presence_goes_through_the_task_filter);
    return UNITY_END();
}