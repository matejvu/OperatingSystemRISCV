//
// Created by os on 7/24/23.
//

#include "../h/sem.h"
#include "../h/syscall_cpp.hpp"


void Sem::block () {

    PCB::runningThread->status = PCB::SUSPENDED;
    blocked->put((size_t*)PCB::runningThread);
    PCB::runningThread = (PCB*)Scheduler::Instance()->getThread();
    PCB::runningThread->status = PCB::RUNNING;
    //PCB::runningThread->resume(); // context switch  //will happen on IR exit

}
void Sem::deblock () {
    PCB* t = (PCB*)blocked->get();
    if(t!= nullptr) { t->status = PCB::READY; }
    Scheduler::Instance()->putThread(t);

}
void Sem::wait () {
    //lock();
    if (--val<0)
        block();
    //unlock();
}
void Sem::signal () {
    //lock();
    if (val++<0)
        deblock();
}

void signalWait (Sem* s, Sem* w) {
    //lock();
    if (s && s->val++<0) s->deblock();
    if (w && --w->val<0) w->block();
    //unlock();
}

Sem::Sem(int val) {
    this->blocked = new Queue();
    this->val = val;

}

Sem::~Sem () {
    PCB* t = (PCB*)blocked->get();
    while(t){
        //t->status = PCB::ERROR;
        PCB::fillJmpBuf(t->myContext, PCB::A1, (size_t)-2); //error code
        Scheduler::Instance()->putThread(t);
        t = (PCB*)blocked->get();
    }


    delete blocked;
}

void *Sem::operator new(size_t size) {
    size_t numOfBlcks = (sizeof (Sem) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return MemoryAllocator::mem_alloc(numOfBlcks);
}

void Sem::operator delete(void *a) {
    MemoryAllocator::mem_free(a);
}
