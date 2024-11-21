//
// Created by os on 7/24/23.
//

#ifndef PROJECT_BASE_QUEUE_H
#define PROJECT_BASE_QUEUE_H

#include "../lib/hw.h"
#include "memoryAllocator.h"

class Queue{
public:
    static void* operator new(size_t size);
    static void operator delete (void* a);
private:
    struct Node{
        size_t* pok;
        Node* next;
    };

    //Constructor & destructor
    Queue();
    ~Queue();

    //Feilds
    Node* list=nullptr;

    //Methods
    void put(size_t* p);
    size_t* get();
    void clear();

    friend class Scheduler;
    friend class Sem;
    friend class PCB;
};


#endif //PROJECT_BASE_QUEUE_H
