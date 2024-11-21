//
// Created by os on 7/17/23.
//


#include "../h/riscV.h"

//initialize static feilds
uint64 RiscV::currentTime = 0;
void* RiscV::stackPointer = nullptr;
PCB* RiscV::kernelThread = nullptr;

//assembly
void RiscV::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("li t0, 256");
    __asm__ volatile ("csrc sstatus, t0");
    __asm__ volatile ("sret");
}

void RiscV::readRaToRunning() {
    size_t ra;
    __asm__ volatile ("mv %0, ra" : "=r"(ra));
    PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::RA, ra);
}

//initialization
void RiscV::initStvec() {
    w_stvec((uint64)RiscV::interruptRoutine | 1);
}

void RiscV::initScheduler(){
    size_t numOfBlcks = (DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    void* sp = (void*)((size_t)MemoryAllocator::mem_alloc(numOfBlcks) + DEFAULT_STACK_SIZE-16);
    PCB* idleThr = new PCB(RiscV::bussyWait, nullptr, sp);
    Scheduler::Instance()->setDefaultThread(idleThr);
    //TCB* idleThread = new TCB(RiscV::bussyWait, nullptr);
    //Scheduler::setDefaultThread(idleThread);
    //create thread using busyWait and set it as default in Scheduler
}

void RiscV::initKernel() {
    size_t numOfBlcks = (DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    RiscV::stackPointer = (void*)((size_t)MemoryAllocator::mem_alloc(numOfBlcks) + DEFAULT_STACK_SIZE-16);
    RiscV::kernelThread = new PCB(nullptr, nullptr, RiscV::stackPointer);
    //size_t numOfBlcks = (DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    //stackPointer = (void*)((size_t)MemoryAllocator::mem_alloc(numOfBlcks) + DEFAULT_STACK_SIZE - 1);

    //kernelThread = new TCB(stackPointer);
}

void RiscV::initializeKernel() {
    //size_t ra;
    //__asm__ volatile ("mv  %0, x1" : "=r"(ra));

    initStvec();
    initScheduler();
    initKernel();


    //TCB::running->context[1] = ra;

}

//interrupt
void RiscV::interruptHandler() {
    uint64 sepc = r_sepc();
    PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::R0, sepc);
    sepc+=4;
    uint64 sstatus = r_sstatus();


    uint64 scause = r_scause();
/*
    if(scause == timerInterrupt){
        //Timer
        mc_sip(SIP_SSIE);

        RiscV::currentTime++;
        PCB::runningThread->timeRunning++;
        SleepingThread::updateTime();
        if(PCB::runningThread->timeRunning >= PCB::runningThread->timeSlice){
            PCB::dispatch();
        }
    }
    if(scause == hwInterrupt){
        //External hardware interrupt
        //console_handler();


        size_t device = plic_claim();
        if(device == 10UL){
            //Console
            //ConsoleHandler::writeToConsole();
            ConsoleHandler::readFromConsole();
        }
        plic_complete(device);
    }
*/
    if(scause == operationInterrupt){
        //Illegal instruction
    }
    if(scause == addrReadInterrupt){
        //Addr not allowed for reading
    }
    if(scause == addrWriteInterrupt){
        //Addr not allowed for writing
    }
    if(scause == ecallUserInterrupt || scause==ecallSystemInterupt){
        PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::R0, sepc);
        //ecall
        size_t operationCode = PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A1);   //a0 -> buf.A1
        //__asm__ volatile ("mv %0, a0" : "=r"(operationCode));

        if(operationCode == 1)  { ABI::mem_alloc(); }
        if(operationCode == 2)  { ABI::mem_free(); }
        if(operationCode == 11) { ABI::thread_create(); }
        if(operationCode == 12) { ABI::thread_exit(); }
        if(operationCode == 13) { ABI::thread_dispatch(); }
        if(operationCode == 14) { ABI::thread_join(); }
        if(operationCode == 21) { ABI::sem_open(); }
        if(operationCode == 22) { ABI::sem_close(); }
        if(operationCode == 23) { ABI::sem_wait(); }
        if(operationCode == 24) { ABI::sem_signal(); }
        if(operationCode == 31) { ABI::time_sleep(); }
        if(operationCode == 41) { ABI::getc(); }
        if(operationCode == 42) { ABI::putc(); }


    }


    //reseno u abi-ju stsvljeno u running buf.A1
    //size_t regA0;
    //__asm__ volatile ("mv %0, a0" : "=r"(regA0));

    //PCB::setjmp(RiscV::kernelThread->myContext);  Prepunjava kernel stek
    //PCB::fillJmpBuf(RiscV::kernelThread->myContext, PCB::A1, regA0);    //a0 -> buf.A1

    //PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::A1, regA0);    //a0 -> buf.A1

    size_t newSepc = PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::R0);//grtting valid sepc
    w_sepc(newSepc);
    w_sstatus(sstatus);

    PCB::longjmp(PCB::runningThread->myContext);

}

void RiscV::timerHandler() {
    uint64 sepc = r_sepc();
    PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::R0, sepc);
    sepc+=4;
    uint64 sstatus = r_sstatus();
    //uint64 scause = r_scause();


    //Timer
    mc_sip(SIP_SSIE);

    RiscV::currentTime++;
    PCB::runningThread->timeRunning++;
    SleepingThread::updateTime();
    if(PCB::runningThread->timeRunning >= PCB::runningThread->timeSlice){
        PCB::dispatch();
    }


    size_t newSepc = PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::R0);//grtting valid sepc
    w_sepc(newSepc);
    w_sstatus(sstatus);

    PCB::longjmp(PCB::runningThread->myContext);
}

void RiscV::consoleHandler() {
    uint64 sepc = r_sepc();
    PCB::fillJmpBuf(PCB::runningThread->myContext, PCB::R0, sepc);
    sepc+=4;
    uint64 sstatus = r_sstatus();
    //uint64 scause = r_scause();


    //External hardware interrupt
    //console_handler();
    size_t device = plic_claim();
    if(device == 10UL){
        //Console
        //ConsoleHandler::writeToConsole();
        ConsoleHandler::readFromConsole();
    }
    plic_complete(device);


    size_t newSepc = PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::R0);//grtting valid sepc
    w_sepc(newSepc);
    w_sstatus(sstatus);

    PCB::longjmp(PCB::runningThread->myContext);
}

void RiscV::enableInterrupts(){
    ms_sstatus(BitMaskSstatus::SSTATUS_SIE);
}

void RiscV::disableInterrupts() {
    mc_sstatus(BitMaskSstatus::SSTATUS_SIE);
}


//threads
void RiscV::bussyWait(void*) {
    while(1){

    }
}

