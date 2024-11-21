//
// Created by os on 7/20/23.
//

#ifndef PROJECT_BASE_PCB_H
#define PROJECT_BASE_PCB_H

#include "riscV.h"
#include "scheduler.h"
#include "syscall_c.h"
#include "queue.h"

class PCB{
public:
    enum State{READY, RUNNING, SUSPENDED, FINISHED, EXITING, SLEEPING, ERROR};
    //Overrided operators
    static void* operator new(size_t size);
    static void operator delete(void* a);
private:
    enum REGISTER{R0, RA, SP, GP, TP, A0=10, A1=11, A2=12, A3=13, A4=14, A5=15, A6=16, A7=17};
    typedef size_t* jmp_buf;//Context of Register(Values of all available 32 registers)
    static void allocateJmpBuf(jmp_buf* bufa ){
        size_t numOfBlcks = (sizeof(size_t)*32 + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
        *bufa = (jmp_buf)MemoryAllocator::mem_alloc(numOfBlcks);
    }
    static void freeJmpBuf(jmp_buf* bufa){
        MemoryAllocator::mem_free(*bufa);
        *bufa = nullptr;
    }
    static void fillJmpBuf(jmp_buf buf, REGISTER r, size_t info){
        buf[r] = info;
    }
    static size_t getRegJmpBuf(jmp_buf buf, REGISTER r){
        return buf[r];
    }
    //Constructors & Destructors
    PCB ();
    PCB (void (*body)(void*), void*args, void* stack_space);
    ~PCB();



    //Feilds
    static PCB* runningThread;
    static PCB::jmp_buf mainContext;
    void (*myBody)(void*);
    void* args;
    char* stackPointer;
    size_t timeSlice = DEFAULT_TIME_SLICE;
    size_t timeRunning = 0;
    State status;
    jmp_buf myContext;
    Queue* semToSignal;
    int isBeginning;
    int isOver;

    //Methods
    //inline int setContext ();     //setjmp
    static void dispatch ();
    //static PCB* running ();         //is this thread running??
    static void wrapper ();
    void start ();                  //longjmp
    void exit();
    void markOver ();               //??? Status:exiting
    void resume ();
    char* getStackPointer () const;
    void signalToSem(Sem* s);
    void fork();                    //??????!!!!!!!!!!

    //Context Switching
    static int setjmp(jmp_buf buffer); //_ZN3TCB6setjmpENS_7jmp_bufE
    static void longjmp(jmp_buf buffer); //_ZN3TCB7longjmpENS_7jmp_bufEi


    //Friend classes & functions
    friend class RiscV;
    friend class Scheduler;
    friend class ABI;
    friend class Sem;
    friend class SleepingThread;
    friend class Thread;
    friend void main ();
};

#endif //PROJECT_BASE_PCB_H
