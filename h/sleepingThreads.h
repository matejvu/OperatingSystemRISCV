//
// Created by os on 7/26/23.
//

#ifndef PROJECT_BASE_SLEEPINGTHREADS_H
#define PROJECT_BASE_SLEEPINGTHREADS_H

#include "riscV.h"
#include "../lib/hw.h"

class SleepingThread{
private:
    struct SleepNode{
      unsigned int timeLeft;
      PCB* thread;
      SleepNode* next;
    };

    //Constructors & descrutors
    SleepingThread() = delete;

    //Feilds
    static SleepNode* list;

    //Methods
    static void putToSleep(PCB* t, unsigned int time);
    static void updateTime();
    static void awakeThread(PCB*t);

    friend class ABI;
    friend class RiscV;
    friend class Scheduler;
};


#endif //PROJECT_BASE_SLEEPINGTHREADS_H
