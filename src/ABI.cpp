//
// Created by os on 7/18/23.
//


#include "../h/ABI.h"

void ABI::mem_alloc(){
    uint64 numOfBlcks = PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A2);  //a1->buf.A2
    //__asm__ volatile("mv %0, a1" : "=r"(numOfBlcks));

    void* retVal = MemoryAllocator::mem_alloc(numOfBlcks);

    //__asm__ volatile("mv a0, %0" : : "r"((uint64)retVal));
    PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::A1, (size_t)retVal);
}

void ABI::mem_free(){
    uint64 addr = PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A2);  //a1->buf.A2
    //__asm__ volatile("mv %0, a1" : "=r"(addr));

    size_t retVal = MemoryAllocator::mem_free((void*)addr);

    //__asm__ volatile("mv a0, %0" : : "r"((uint64)retVal));
    PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::A1, (size_t)retVal);
}

void ABI::thread_create() {
    PCB** handle = (PCB**)PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A2);  //a1->buf.A2
    void (*start_routine)(void *) = (void(*)(void*))PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A3);  //a2->buf.A3
    void *arg = (void*)PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A4);  //a3->buf.A4
    void* stack = (void*)PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A6);  //a4->buf.A5->buf.A6

    //__asm__ volatile("mv %0, a1" : "=r"(handle));
    //__asm__ volatile("mv %0, a2" : "=r"(start_routine));
    //__asm__ volatile("mv %0, a3" : "=r"(arg));
    //__asm__ volatile("mv %0, a4" : "=r"(stack));

    if(start_routine!= nullptr)*handle = new PCB(start_routine, arg, stack);
    else *handle = nullptr;

    int retVal=0;
    if(*handle == nullptr)retVal = -1;  //error while allocating memory or given nullptr as start routine
    else (*handle)->start();            //starting valid thread
    //__asm__ volatile("mv a0, %0" : : "r"((uint64)retVal));  //a0->buf.A1
    PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::A1, (size_t)retVal);
}

void ABI::thread_exit() {
    PCB* rT = PCB::runningThread;

    PCB::runningThread->exit();

    int retVal = 0;
    if(rT==PCB::runningThread) {
        retVal = -1;   //Unsuccesful exiting
        PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::A1, (size_t)retVal);  //Jer se ne vraca u kontekst niti koja je pozvala
    }
    //__asm__ volatile("mv a0, %0" : : "r"((uint64)retVal));  //a0->buf.A1
}

void ABI::thread_dispatch() {

    PCB::dispatch();

}

void ABI::thread_join() {
    PCB* handle = (PCB*)PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A2);  //a1->buf.A2

    Sem* newSem = new Sem(0);
    //PCB::runningThread->semWaiting = newSem;
    handle->signalToSem(newSem);
    newSem->wait();

}

void ABI::sem_open() {
    Sem** handle = (Sem**)PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A2);  //a1->buf.A2
    unsigned init = (unsigned)PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A3);  //a2->buf.A3

    *handle = new Sem(init);

    int retVal=0;
    if(*handle == nullptr)retVal = -1;  //error while allocating memory
    //__asm__ volatile("mv a0, %0" : : "r"((uint64)retVal));  //a0->buf.A1
    PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::A1, (size_t)retVal);
}

void ABI::sem_close() {
    Sem* handle = (Sem*)PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A2);  //a1->buf.A2
    //Sem* pokSem = *handle;

    int retVal=0;
    if((size_t)HEAP_START_ADDR<=(size_t)handle && (size_t)handle<=(size_t)HEAP_END_ADDR)delete handle;
    else retVal = -1;   //invalid handle


    //__asm__ volatile("mv a0, %0" : : "r"((uint64)retVal));  //a0->buf.A1
    PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::A1, (size_t)retVal);
}

void ABI::sem_wait() {
    Sem* id = (Sem*)PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A2);  //a1->buf.A2
    //Sem pokSem = *id;

    int retVal=0;
    if((size_t)HEAP_START_ADDR<=(size_t)id && (size_t)id<=(size_t)HEAP_END_ADDR)id->wait();
    else retVal = -1;   //invalid id



    //__asm__ volatile("mv a0, %0" : : "r"((uint64)retVal));  //a0->buf.A1
    PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::A1, (size_t)retVal);
}

void ABI::sem_signal() {
    Sem* id = (Sem*)PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A2);  //a1->buf.A2
    //Sem* pokSem = *id;

    int retVal=0;
    if((size_t)HEAP_START_ADDR<=(size_t)id && (size_t)id<=(size_t)HEAP_END_ADDR)id->signal();
    else retVal = -1;   //invalid id


    //__asm__ volatile("mv a0, %0" : : "r"((uint64)retVal));  //a0->buf.A1
    PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::A1, (size_t)retVal);
}

void ABI::time_sleep() {
    unsigned int time = (unsigned int)PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A2);  //a1->buf.A2

    int retVal=0;
    if(time!=0) {
        PCB::runningThread->status = PCB::SLEEPING;
        SleepingThread::putToSleep(PCB::runningThread, time);

        PCB::runningThread = Scheduler::Instance()->getThread();
        PCB::runningThread->status = PCB::RUNNING;
    }

    //__asm__ volatile("mv a0, %0" : : "r"((uint64)retVal));  //a0->buf.A1
    PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::A1, (size_t)retVal);
}

void ABI::getc() {
    char retVal = ConsoleHandler::readRequest();

    PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::A1, (size_t)retVal);
    PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::A7, (size_t)ConsoleHandler::inputBufferAvailable);
}

void ABI::putc() {
    char c = (char )PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A2);  //a1->buf.A2
    ConsoleHandler::writeRequest(c);
}
