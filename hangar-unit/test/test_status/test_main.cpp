//
// Created by Eduard on 22/11/2026.
//
// test/test_status/test_main.cpp
#include <unity.h>
#include "StatusTask.h"

class FakeTrafficLight : public ITrafficLight {
public:
    bool green = false, yellow = false, red = false;
    void set(bool g, bool y, bool r) override { green = g; yellow = y; red = r; }
};

void setUp() {}
void tearDown() {}

void test_idle_is_steady_green() {
    HangarBlackBoard bb;
    FakeTrafficLight light;
    StatusTask status(bb, light);
    bb.setHangarState(HangarState::IDLE);

    for (int i = 0; i < 2; i++) {
        status.tick();
        TEST_ASSERT_TRUE(light.green);
        TEST_ASSERT_FALSE(light.yellow);
        TEST_ASSERT_FALSE(light.red);
    }
}

void test_alarm_blinks_red() {
    HangarBlackBoard bb;
    FakeTrafficLight light;
    StatusTask status(bb, light);
    bb.setHangarState(HangarState::ALARM);

    status.tick();
    bool first = light.red;
    status.tick();
    TEST_ASSERT_NOT_EQUAL(first, light.red);
    TEST_ASSERT_FALSE(light.green);
    TEST_ASSERT_FALSE(light.yellow);
}

void test_denying_is_steady_red() {
    HangarBlackBoard bb;
    FakeTrafficLight light;
    StatusTask status(bb, light);
    bb.setHangarState(HangarState::DENYING);

    status.tick();
    TEST_ASSERT_TRUE(light.red);
    status.tick();
    TEST_ASSERT_TRUE(light.red);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_idle_is_steady_green);
    RUN_TEST(test_alarm_blinks_red);
    RUN_TEST(test_denying_is_steady_red);
    return UNITY_END();
}