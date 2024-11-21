//
// Created by os on 7/18/23.
//

#include "riscV.h"
#include "../lib/hw.h"
//#include "tcb.h"

#ifndef PROJECT_BASE_SCHEDULER_H
#define PROJECT_BASE_SCHEDULER_H


class PCB;
class Sem;


class Scheduler{
public:
    struct Node{
        Node* next;
        PCB* thread;

        Node(PCB* p){
            this->thread = p;
            this->next = nullptr;
        }
        ~Node(){
            this->thread = nullptr;
            this->next = nullptr;
        }
        static void* operator new(size_t size){
            size_t numOfBlcks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
            return MemoryAllocator::mem_alloc(numOfBlcks);
        }
        static void operator delete(void* adr){
            MemoryAllocator::mem_free(adr);
        }

    };


    Scheduler(Scheduler&) = delete;
    //Overrided operators
    static void* operator new(size_t size){
        size_t numOfBlcks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
        return MemoryAllocator::mem_alloc(numOfBlcks);
    }
    static void operator delete(void* a){
        MemoryAllocator::mem_free(a);
    }

private:
    //Constructor
    Scheduler() {};


    //Feilds
    static Scheduler* instance;
    Node* linkedList = nullptr;
    PCB* defaultThread = nullptr;

    //Methods
    static Scheduler* Instance();
    void setDefaultThread(PCB* t);
    void putThread(PCB* t);
    PCB* getThread();
    static void create();


    //Friend classes
    friend class TCB;
    friend class PCB;
    friend class RiscV;
    friend class Sem;
    friend class SleepingThread;
    friend class ABI;

    friend void main();
};


#endif //PROJECT_BASE_SCHEDULER_H
