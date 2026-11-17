//
// Created by Eduard on 17/11/2026.
//

#ifndef DRONE_IN_A_BOX_SCHEDULER_H
#define DRONE_IN_A_BOX_SCHEDULER_H
#include "AbstractTask.h"
#include "ITimer.h"



class Scheduler {
public:
    static const int MAX_TASKS = 16;

    explicit Scheduler(ITimer& timer) : timer(timer) {}

    void init(int basePeriodMS) {
        this->basePeriod = basePeriodMS;
        this->timer.setupPeriod(basePeriodMS);
    }

    bool addTask(AbstractTask *task) {
        if (this->nTasks >= MAX_TASKS) return false;

        this->tasks[nTasks++] = task;
        return true;
    }

    void schedule() {
        if (!timer.waitForNextTick()) {
            overruns++;
        }
        for (int i = 0; i < nTasks; i++) {
            if (tasks[i]->updateAndCheckTime(basePeriod)) {
                tasks[i]->tick();
            }
        }
    }

private:
    int basePeriod = 0;
    AbstractTask* tasks[MAX_TASKS] = {};
    int nTasks = 0;
    ITimer& timer;
    unsigned long overruns = 0;
};
#endif //DRONE_IN_A_BOX_SCHEDULER_H
