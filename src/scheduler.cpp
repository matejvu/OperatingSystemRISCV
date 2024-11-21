//
// Created by os on 7/18/23.
//


#include "../h/scheduler.h"

Scheduler* Scheduler::instance = nullptr;


Scheduler *Scheduler::Instance() {
   if(Scheduler::instance == nullptr){
       create();
   }

   return Scheduler::instance;
}

void Scheduler::setDefaultThread(PCB* t){
    defaultThread = t;
}

void Scheduler::putThread(PCB* t){
    if(t == nullptr)return;
    if(t->myBody == RiscV::bussyWait)return;

    //----------
    size_t numOfBlcks = (sizeof(Node) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    Node* newNode = (Node*)MemoryAllocator::mem_alloc(numOfBlcks);
    newNode->next= nullptr;
    newNode->thread = t;
    //Node* newNode = new Node(t);
    Node* prev = nullptr, *cur = linkedList;
    for(;cur;prev = cur, cur = cur->next);

    if(!prev)linkedList = newNode;
    else prev->next = newNode;

    newNode->next= nullptr;
}

PCB* Scheduler::getThread(){
    if(!linkedList && !SleepingThread::list && !ConsoleHandler::inputBufferAvailable->blocked->list) PCB::longjmp(PCB::mainContext);
    if(linkedList == nullptr)return defaultThread;
    //if(linkedList == nullptr) PCB::longjmp(PCB::mainContext);

    Node* old = linkedList;
    PCB* rT = linkedList->thread;

    linkedList = linkedList->next;


    //--------
    //naknadno komentarisano
    //old->next = nullptr;
    //old->thread = nullptr;
    MemoryAllocator::mem_free((void*)old);
    //delete old;
    rT->timeRunning=0;
    return rT;
}

void Scheduler::create() {
    instance = new Scheduler();
}


