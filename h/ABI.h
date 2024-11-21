//
// Created by os on 7/18/23.
//

#ifndef PROJECT_BASE_ABI_H
#define PROJECT_BASE_ABI_H

#include "memoryAllocator.h"
#include "PCB.h"
#include "sem.h"
#include "sleepingThreads.h"


class ABI{
private:
    static const uint64 MEM_ALLOC = 1UL;
    static const uint64 MEM_FREE = 1UL;
    static const uint64 THREAD_CREATE = 11UL;
    static const uint64 THREAD_EXIT = 12UL;
    static const uint64 THREAD_DISPATCH = 13UL;
    static const uint64 THREAD_JOIN = 14UL;
    static const uint64 SEM_OPEN = 21UL;
    static const uint64 SEM_CLOSE = 22UL;
    static const uint64 SEM_WAIT = 23UL;
    static const uint64 SEM_SIGNAL = 24UL;
    static const uint64 TIME_SLEEP = 31UL;
    static const uint64 GETC = 41UL;
    static const uint64 PUTC = 42UL;


    static void mem_alloc();                //01
    static void mem_free();                 //02

    static void thread_create();            //11
    static void thread_exit();              //12
    static void thread_dispatch();          //13

    static void thread_join();              //14
    static void sem_open();                 //21

    static void sem_close();                //22
    static void sem_wait();                 //23
    static void sem_signal();               //24

    static void time_sleep();               //31
    const int EOF = -1;
    static void getc();                     //41
    static void putc();                     //42





    friend class RiscV;
    friend class Scheduler;
};


#endif //PROJECT_BASE_ABI_H
