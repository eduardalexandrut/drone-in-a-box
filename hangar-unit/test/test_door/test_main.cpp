//
// Created by Eduard on 22/11/2026.
//
// test/test_door/test_main.cpp
#include <unity.h>
#include "DoorTask.h"

class FakeServo : public IServo {
public:
    int last = -1;
    void setAngle(int degrees) override { last = degrees; }
};

static void tickN(DoorTask& door, int n) {
    for (int i = 0; i < n; i++) door.tick();
}

void setUp() {}
void tearDown() {}

void test_opens_gradually() {
    HangarBlackBoard bb;
    FakeServo servo;
    DoorTask door(bb, servo, 90, 3);
    door.begin();

    bb.setDoorCommand(DoorCommand::OPEN_DOOR);
    tickN(door, 1);
    TEST_ASSERT_EQUAL(DoorState::OPENING, bb.getDoorState());

    tickN(door, 30);   // 30 steps of 3°
    TEST_ASSERT_EQUAL(DoorState::OPEN, bb.getDoorState());
    TEST_ASSERT_EQUAL(90, servo.last);
    TEST_ASSERT_EQUAL(100, bb.getDoorPosition());
}

void test_reverses_while_opening() {
    HangarBlackBoard bb;
    FakeServo servo;
    DoorTask door(bb, servo, 90, 3);
    door.begin();

    bb.setDoorCommand(DoorCommand::OPEN_DOOR);
    tickN(door, 11);                  // angle 30
    bb.setDoorCommand(DoorCommand::CLOSE_DOOR);
    tickN(door, 1);
    TEST_ASSERT_EQUAL(DoorState::CLOSING, bb.getDoorState());
    TEST_ASSERT_EQUAL(30, servo.last);

    tickN(door, 1);
    TEST_ASSERT_EQUAL(27, servo.last);
}

void test_closes_completely() {
    HangarBlackBoard bb;
    FakeServo servo;
    DoorTask door(bb, servo, 90, 3);
    door.begin();

    bb.setDoorCommand(DoorCommand::OPEN_DOOR);
    tickN(door, 31);
    bb.setDoorCommand(DoorCommand::CLOSE_DOOR);
    tickN(door, 31);
    TEST_ASSERT_EQUAL(DoorState::CLOSED, bb.getDoorState());
    TEST_ASSERT_EQUAL(0, servo.last);
    TEST_ASSERT_EQUAL(0, bb.getDoorPosition());
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_opens_gradually);
    RUN_TEST(test_reverses_while_opening);
    RUN_TEST(test_closes_completely);
    return UNITY_END();
}