//
// Created by os on 7/14/23.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_H
#define PROJECT_BASE_MEMORYALLOCATOR_H

#include "../lib/hw.h"



class MemoryAllocator{

private:
    struct FreeMem {
        FreeMem* next; // Next in the list
        FreeMem* prev; // Previous in the list
        size_t numOfBlcks; // Size of the free fragment
    };
    struct Descriptor{
        size_t numOfBlock;
    };

    //Feilds
    static int initialized;
    static FreeMem* fmem_head;

    //Methods
    static int tryToJoin(FreeMem* cur);
    static void initialize();

    static size_t mem_free(void* address);
    static void* mem_alloc (size_t size);  //size in Blocks

    //Friend classes
    friend class RiscV;
    friend class ABI;
    friend class Scheduler;
    friend class PCB;
    friend class Sem;
    friend class Queue;
    friend class SleepingThread;
    friend class ConsoleHandler;

    friend void main();
};




#endif //PROJECT_BASE_MEMORYALLOCATOR_H
