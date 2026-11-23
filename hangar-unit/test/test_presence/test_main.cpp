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

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_drone_starts_as_detected);

    return UNITY_END();
}