//
// Created by os on 7/19/23.
//


#include "../h/riscV.h"


void RiscV::interruptRoutine2() {
    //save A0

    //__asm__ volatile ("sd a0, 1*10(%0)" : :"r"((size_t)PCB::runningThread->myContext));

    if(PCB::setjmp(PCB::runningThread->myContext) == 0){
        PCB::fillJmpBuf( RiscV::kernelThread->myContext, PCB::RA, (size_t)RiscV::interruptHandler);

        PCB::longjmp(RiscV::kernelThread->myContext);
    }



    size_t rVal = PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A1);   //a0 -> buf.A1
    __asm__ volatile ("mv a0, %0" : : "r"(rVal));

    __asm__ volatile ("ld ra,8(sp)");
    __asm__ volatile ("ld s0, 0(sp)");
    __asm__ volatile ("addi sp,sp,16");

    __asm__ volatile("sret");

}

void RiscV::timerRoutine2() {
   //timer
    if(PCB::setjmp(PCB::runningThread->myContext) == 0){
        PCB::fillJmpBuf( RiscV::kernelThread->myContext, PCB::RA, (size_t)RiscV::timerHandler);

        PCB::longjmp(RiscV::kernelThread->myContext);
    }

    size_t rVal = PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A1);   //a0 -> buf.A1
    __asm__ volatile ("mv a0, %0" : : "r"(rVal));

    __asm__ volatile ("ld ra,8(sp)");
    __asm__ volatile ("ld s0, 0(sp)");
    __asm__ volatile ("addi sp,sp,16");

    __asm__ volatile("sret");
}

void RiscV::consoleRoutine2() {
    //console
    if(PCB::setjmp(PCB::runningThread->myContext) == 0){
        PCB::fillJmpBuf( RiscV::kernelThread->myContext, PCB::RA, (size_t)RiscV::consoleHandler);

        PCB::longjmp(RiscV::kernelThread->myContext);
    }


    size_t rVal = PCB::getRegJmpBuf(PCB::runningThread->myContext, PCB::A1);   //a0 -> buf.A1
    __asm__ volatile ("mv a0, %0" : : "r"(rVal));

    __asm__ volatile ("ld ra,8(sp)");
    __asm__ volatile ("ld s0, 0(sp)");
    __asm__ volatile ("addi sp,sp,16");

    __asm__ volatile("sret");
}
