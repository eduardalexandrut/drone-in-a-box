// AbstractTask.h
#pragma once

class AbstractTask {
public:
    virtual ~AbstractTask() = default;

    void init(int period) {
        this->period = period;
        this->timeElapsed = 0;
    };

    // Called by the scheduler every base period.
    // Returns true when the task's period has expired.
    bool updateAndCheckTime(int basePeriod) {
        timeElapsed += basePeriod;
        if (timeElapsed >= period) {
            timeElapsed = 0;
            return true;
        }
        return false;
    };

    // Implemented by each specific task
    virtual void tick() = 0;

protected:
    int getPeriod() const {return period; };

private:
    int period = 0;
    int timeElapsed = 0;
};