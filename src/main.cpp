//
// Created by os on 7/17/23.
//
#include "../lib/console.h"
#include "../h/riscV.h"
#include "../h/syscall_c.h"
//#include "../h/tcb.h"
#include "../h/PCB.h"


extern void(*fPok)(void*);

void userMain();

void userMainWrraper(void* ptr){
    userMain();
}

void main(){
    __asm__ volatile(".extern _ZN5RiscV17interruptRoutine2Ev");
    __asm__ volatile(".align 4");


    size_t numOfBlcks = (DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    RiscV::initializeKernel();

    //create user thread and palce it for running


    void* user_stack = (void*)((size_t)MemoryAllocator::mem_alloc(numOfBlcks) + DEFAULT_STACK_SIZE );
    PCB* userThread = new PCB(userMainWrraper, nullptr,user_stack);
    PCB::runningThread = userThread;

    numOfBlcks = (sizeof(size_t)*32 + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    PCB::mainContext = (PCB::jmp_buf)MemoryAllocator::mem_alloc(numOfBlcks);



    if(PCB::setjmp(PCB::mainContext)==0){
        //unlock();
        //RiscV::readRaToRunning();
        //size_t newSepc = (size_t)&PCB::wrapper;
        //RiscV::w_sepc(newSepc);
        //PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::RA, (size_t)&RiscV::interruptRoutine2);
        //PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::R0, newSepc);
        //RiscV::popSppSpie();

        PCB::longjmp(PCB::runningThread->myContext);
        size_t newSstatus = 32;
        RiscV::w_sstatus(newSstatus);
        __asm__ volatile ("sret");

        //PCB::longjmp(PCB::runningThread->myContext);


    }
    else {/*
        __putc('K');
        __putc('R');
        __putc('A');
        __putc('J');
        __putc('\n');
        //ThreadCollector::destroy();
        return;*/
    }


   /*
    PCB* mainThread = (PCB*)mem_alloc(sizeof(PCB));
    PCB::runningThread = mainThread;

    thread_t userThread;
    thread_create(&userThread, fPok, nullptr);

    thread_join(userThread);
    __putc('K');
    __putc('R');
    __putc('A');
    __putc('J');
    __putc('\n');
    */

}


