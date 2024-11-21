//
// Created by os on 7/20/23.
//

#include "../h/PCB.h"

PCB* PCB::runningThread = nullptr;
PCB::jmp_buf PCB::mainContext = nullptr;

PCB::PCB() {

}

PCB::PCB(void (*body)(void *), void *args, void *stack_space) {
    jmp_buf newContext = nullptr;
    //----
    size_t numOfBlcks = (sizeof(size_t)*32 + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    newContext = (jmp_buf)MemoryAllocator::mem_alloc(numOfBlcks);
    //allocateJmpBuf(&newContext);
    for(int i=0;i<32;i++){ fillJmpBuf(newContext, (REGISTER)i, 0);}//fillZJmpBuf(newContext);
    PCB::setjmp(newContext);
    this->args = args;
    this->myBody = body;
    this->status = SUSPENDED;
    this->stackPointer = (char*)stack_space;
    this->isBeginning = 1;
    this->isOver = 0;
    this->timeSlice = DEFAULT_TIME_SLICE;
    this->semToSignal = new Queue();

    //Kernel thread is initialized with nullptr and it will continue executing(RA will be changed in interrupt routine)
    if(body!= nullptr)fillJmpBuf(newContext, RA, (size_t)PCB::wrapper);
    //if(body!= nullptr)fillJmpBuf(newContext, R0, (size_t)PCB::wrapper);
    fillJmpBuf(newContext, SP, (size_t) stack_space);

    //for(int i=4; i<32; i++){
     //   fillJmpBuf(newContext, (REGISTER)i, 0);
    //}

    this->myContext = newContext;
}

PCB::~PCB() {
    void* sp = (void*)((size_t)this->stackPointer - DEFAULT_STACK_SIZE + 16);
    MemoryAllocator::mem_free(sp);
    delete semToSignal;
    freeJmpBuf(&this->myContext);
    this->myBody = nullptr;
    this->args = nullptr; //Maybe delete them too (Change later)
}

void *PCB::operator new(size_t size) {
    size_t numOfBlcks = (sizeof (PCB) +  MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return MemoryAllocator::mem_alloc(numOfBlcks);
}

void PCB::operator delete(void *a) {
    MemoryAllocator::mem_free(a);
}

void PCB::dispatch() {
    runningThread->status = READY;
    Scheduler::Instance()->putThread(PCB::runningThread);
    PCB::runningThread = Scheduler::Instance()->getThread();
    runningThread->status = RUNNING;
    if(runningThread->isBeginning==1){runningThread->start();}
}

void PCB::wrapper() {
    RiscV::popSppSpie();//Prepare to exit in User regime
    runningThread->myBody(runningThread->args); //run the function
    //lock? System regime?
    //runningThread->exitThread();    //close the thread

    //EXIT
    thread_exit();
    //longjmp(RiscV::kernelThread->myContext);
}

void PCB::start() {
    this->status = READY;
    this->isBeginning = 0;
    if(this!=PCB::runningThread){
        this->status = READY;
        Scheduler::Instance()->putThread(this);
    }
    else{
        this->status = RUNNING;
        this->timeRunning=0;
    }
}

void PCB::exit() {
    PCB::runningThread->markOver();
    Sem* s = (Sem*)this->semToSignal->get();
    while(s){
        s->signal();
        delete s;

        s = (Sem*)this->semToSignal->get();
    }
    PCB::runningThread = Scheduler::Instance()->getThread();
    PCB::runningThread->status = RUNNING;
}

void PCB::markOver() {
    this->isOver = 1;
    this->status = State::EXITING;
}

void PCB::resume() {
    this->status = READY;
    Scheduler::Instance()->putThread(this);
}

char *PCB::getStackPointer() const {
    return this->stackPointer;
}

void PCB::fork() {

}

void PCB::signalToSem(Sem* s) {
    this->semToSignal->put((size_t*)s);
}
