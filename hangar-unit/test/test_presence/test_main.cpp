//
// Created by aedua on 11/23/2026.
//

#include "../../lib/logic/IPresenceSensor.h"
#include "../../lib/logic/PresenceTask.h"
#include <unity.h>

class FakePresenceSensor : public IPresenceSensor
{
    public:
        void setDetected(bool detected)
        {
            this->detected = detected;
        }

        bool isDetected() override
        {
            return detected;
        }

    private:
        bool detected = false;

};

static void tickN(PresenceTask& task, int n)
{
    for (int i = 0; i < n; i++) task.tick();
}

void setUp() {}
void tearDown() {}

void test_drone_starts_as_detected()
{
    HangarBlackBoard bb;
    FakePresenceSensor fakePresenceSensor;
    fakePresenceSensor.setDetected(true);
    PresenceTask task(bb, fakePresenceSensor);
    task.begin();

    TEST_ASSERT_TRUE(bb.isDroneDetected());
}

void test_drone_absent_when_starting()
{
    HangarBlackBoard bb;
    FakePresenceSensor fakePresenceSensor;
    fakePresenceSensor.setDetected(false);
    PresenceTask task(bb, fakePresenceSensor);
    task.begin();

    TEST_ASSERT_FALSE(bb.isDroneDetected());

}

void test_blackboard_gets_overwritten()
{
    HangarBlackBoard bb;
    bb.setDroneDetected(true);
    FakePresenceSensor fakePresenceSensor;
    fakePresenceSensor.setDetected(false);
    PresenceTask task(bb, fakePresenceSensor);
    task.begin();

    TEST_ASSERT_FALSE(bb.isDroneDetected());
}

// ---------------------------------------------------------------------------
// From absent to present
// ---------------------------------------------------------------------------

void test_two_present_readings_are_not_enough() {
    HangarBlackBoard bb;
    FakePresenceSensor sensor;
    PresenceTask task(bb, sensor);
    sensor.setDetected(false);
    task.begin();

    sensor.setDetected(true);
    tickN(task, 2);

    TEST_ASSERT_FALSE(bb.isDroneDetected());
}

void test_third_present_reading_changes_state() {
    HangarBlackBoard bb;
    FakePresenceSensor sensor;
    PresenceTask task(bb, sensor);
    sensor.setDetected(false);
    task.begin();

    sensor.setDetected(true);
    tickN(task, 3);

    TEST_ASSERT_TRUE(bb.isDroneDetected());
}

// ---------------------------------------------------------------------------
// From present to absent
// ---------------------------------------------------------------------------

void test_two_absent_readings_are_not_enough() {
    HangarBlackBoard bb;
    FakePresenceSensor sensor;
    PresenceTask task(bb, sensor);
    sensor.setDetected(true);
    task.begin();

    sensor.setDetected(false);
    tickN(task, 2);

    TEST_ASSERT_TRUE(bb.isDroneDetected());
}

void test_third_absent_reading_changes_state() {
    HangarBlackBoard bb;
    FakePresenceSensor sensor;
    PresenceTask task(bb, sensor);
    sensor.setDetected(true);
    task.begin();

    sensor.setDetected(false);
    tickN(task, 3);

    TEST_ASSERT_FALSE(bb.isDroneDetected());
}

// ---------------------------------------------------------------------------
// Spurious readings
// ---------------------------------------------------------------------------

void test_single_spurious_reading_is_ignored() {
    HangarBlackBoard bb;
    FakePresenceSensor sensor;
    PresenceTask task(bb, sensor);
    sensor.setDetected(false);
    task.begin();

    const bool readings[] = { true, false, false };
    for (bool r : readings) {
        sensor.setDetected(r);
        task.tick();
        TEST_ASSERT_FALSE(bb.isDroneDetected());
    }
}

void test_two_spurious_readings_are_ignored() {
    HangarBlackBoard bb;
    FakePresenceSensor sensor;
    PresenceTask task(bb, sensor);
    sensor.setDetected(false);
    task.begin();

    const bool readings[] = { true, true, false };
    for (bool r : readings) {
        sensor.setDetected(r);
        task.tick();
        TEST_ASSERT_FALSE(bb.isDroneDetected());
    }
}

void test_count_restarts_after_interruption() {
    HangarBlackBoard bb;
    FakePresenceSensor sensor;
    PresenceTask task(bb, sensor);
    sensor.setDetected(false);
    task.begin();

    // two present readings, then an absent one resets the count
    const bool readings[] = { true, true, false };
    for (bool r : readings) {
        sensor.setDetected(r);
        task.tick();
    }

    // three new consecutive present readings are needed
    sensor.setDetected(true);
    tickN(task, 2);
    TEST_ASSERT_FALSE(bb.isDroneDetected());

    tickN(task, 1);
    TEST_ASSERT_TRUE(bb.isDroneDetected());
}

void test_alternating_readings_never_change_state() {
    HangarBlackBoard bb;
    FakePresenceSensor sensor;
    PresenceTask task(bb, sensor);
    sensor.setDetected(false);
    task.begin();

    for (int i = 0; i < 10; i++) {
        sensor.setDetected(i % 2 == 0);
        task.tick();
        TEST_ASSERT_FALSE(bb.isDroneDetected());
    }
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_drone_starts_as_detected);
    RUN_TEST(test_drone_absent_when_starting);
    RUN_TEST(test_blackboard_gets_overwritten);
    RUN_TEST(test_two_present_readings_are_not_enough);
    RUN_TEST(test_third_present_reading_changes_state);
    RUN_TEST(test_two_absent_readings_are_not_enough);
    RUN_TEST(test_third_absent_reading_changes_state);
    RUN_TEST(test_single_spurious_reading_is_ignored);
    RUN_TEST(test_two_spurious_readings_are_ignored);
    RUN_TEST(test_count_restarts_after_interruption);
    RUN_TEST(test_alternating_readings_never_change_state);

    return UNITY_END();
}