//
// Created by os on 7/24/23.
//

#ifndef PROJECT_BASE_SEM_H
#define PROJECT_BASE_SEM_H

#include "riscV.h"
#include "scheduler.h"
#include "queue.h"
#include "syscall_c.h"

class Sem {
    static void* operator new(size_t size);
    static void operator delete (void* a);
private:
    Sem(int initVal);
    ~Sem();

    void wait();

    void signal();

    friend void signalWait(Sem *s, Sem *w);

    int value() { return val; };

    void block();

    void deblock();

    int val;

    Queue* blocked;

    friend class RiscV;
    friend class PCB;
    friend class ABI;
    friend class Scheduler;
    friend class ConsoleHandler;

    friend void main();
};

#endif //PROJECT_BASE_SEM_H
