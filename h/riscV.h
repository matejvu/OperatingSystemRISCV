//
// Created by os on 7/17/23.
//

#ifndef PROJECT_BASE_RISCV_H
#define PROJECT_BASE_RISCV_H

#include "../lib/hw.h"
#include "../lib/console.h"
#include "ABI.h"
#include "PCB.h"
#include "sem.h"
#include "queue.h"
#include "consoleHandler.h"

class Sem;
class PCB;

class RiscV {

public:
    static void initializeKernel(); //initialization

private:
    //Initialization related methods
    static void initStvec();    //initialize stvec for 1 interrupt routine
    static void initScheduler();    //initialize Scheduler with an idle thread
    static void initKernel();

    static void* stackPointer;
    static PCB* kernelThread;


    //Interrupt related methods
    static void interruptRoutine();
    static void interruptRoutine2();    //IR for ecall
    static void timerRoutine2();        //IR for timer
    static void consoleRoutine2();      //IR for console
    static void interruptHandler();     //Handler for ecall and errors
    static void timerHandler();         //Handler for timer(softwareInterrupt)
    static void consoleHandler();       //Handler for console(hwInterrupt)

    static void enableInterrupts();
    static void disableInterrupts();


    //Timer related feild
    static uint64 currentTime;

    //Thread related methods
    static void bussyWait(void*);

    //Methods for using assembly functions

    // pop sstatus.spp and sstatus.spie bits (has to be a non inline function)
    static void popSppSpie();

    static void readRaToRunning();//must not be inline

    // push x3..x31 registers onto stack
//    static void pushRegisters();

    // pop x3..x31 registers onto stack
//    static void popRegisters();

    // read register scause
    static uint64 r_scause();

    // write register scause
    static void w_scause(uint64 scause);

    // read register sepc
    static uint64 r_sepc();

    // write register sepc
    static void w_sepc(uint64 sepc);

    // read register stvec
    static uint64 r_stvec();

    // write register stvec
    static void w_stvec(uint64 stvec);

    // read register stval
    static uint64 r_stval();

    // write register stval
    static void w_stval(uint64 stval);

    enum BitMaskSip
    {
        SIP_SSIE = (1 << 1),
        SIP_STIE = (1 << 5),
        SIP_SEIE = (1 << 9),
    };

    // mask set register sip
    static void ms_sip(uint64 mask);

    // mask clear register sip
    static void mc_sip(uint64 mask);

    // read register sip
    static uint64 r_sip();

    // write register sip
    static void w_sip(uint64 sip);

    enum BitMaskSstatus
    {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8),
    };

    // mask set register sstatus
    static void ms_sstatus(uint64 mask);

    // mask clear register sstatus
    static void mc_sstatus(uint64 mask);

    // read register sstatus
    static uint64 r_sstatus();

    // write register sstatus
    static void w_sstatus(uint64 sstatus);

    enum BitMaskSie
    {
        SIE_SSIE = (1 << 1),
        SIE_SEIE = (1 << 9),
    };


    // mask set register sie
    static void ms_sie(uint64 mask);

    // mask clear register sie
    static void mc_sie(uint64 mask);

    // read register sie
    static uint64  r_sie();

    // write register sie
    static void w_sie(uint64 sie);


    static const uint64 bntOne = 1UL << 63;;
    static const uint64 bntZero = 0UL;
    static const uint64 timerInterrupt = bntOne + 1UL;
    static const uint64 hwInterrupt = bntOne + 9UL;
    static const uint64 operationInterrupt = bntZero + 2UL;
    static const uint64 addrReadInterrupt = bntZero + 5UL;
    static const uint64 addrWriteInterrupt = bntZero + 7UL;
    static const uint64 ecallUserInterrupt = bntZero + 8UL;
    static const uint64 ecallSystemInterupt = bntZero + 9UL;

    //Friend classes
    friend class PCB;
    friend class Scheduler;
    friend class Sem;
    //friend class TCB;
    friend void main();
};






inline uint64 RiscV::r_scause(){
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void RiscV::w_scause(uint64 scause)
{
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 RiscV::r_sepc()
{
    uint64 volatile sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}

inline void RiscV::w_sepc(uint64 sepc)
{
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 RiscV::r_stvec()
{
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}

inline void RiscV::w_stvec(uint64 stvec)
{
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 RiscV::r_stval()
{
    uint64 volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}

inline void RiscV::w_stval(uint64 stval)
{
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline void RiscV::ms_sip(uint64 mask)
{
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sip(uint64 mask)
{
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::r_sip()
{
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline void RiscV::w_sip(uint64 sip)
{
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void RiscV::ms_sstatus(uint64 mask)
{
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sstatus(uint64 mask)
{
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::r_sstatus()
{
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void RiscV::w_sstatus(uint64 sstatus)
{
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}


inline void RiscV::ms_sie(uint64 mask) {
    __asm__ volatile ("csrs sie, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sie(uint64 mask) {
    __asm__ volatile ("csrc sie, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::r_sie() {
    uint64 sie;
    __asm__ volatile ("csrr %[sie], sie" : [sie] "=r"(sie));
    return sie;
}

inline void RiscV::w_sie(uint64 sie) {
    __asm__ volatile ("csrw sie, %[sie]" : : [sie] "r"(sie));
}




#endif //PROJECT_BASE_RISCV_H
