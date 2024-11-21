//
// Created by os on 8/14/23.
//

#ifndef PROJECT_BASE_CONSOLEHANDLER_H
#define PROJECT_BASE_CONSOLEHANDLER_H

#include "../lib/hw.h"
#include "riscV.h"

class ConsoleHandler{
private:
    static const size_t STATUS_READY_IN = 1UL;
    static const size_t STATUS_READY_OUT = 1UL << 5UL;

    static const size_t bufferSize = MEM_BLOCK_SIZE;
    static char *inputBuffer;
    static char *outputBuffer;

    struct WriteRequest{
        char c;
        WriteRequest* next;
    };

    //Feilds
    static bool initialized;
    static WriteRequest* requests;
    static size_t inputTail;
    static size_t inputHead;
    static size_t inputNum;
    static size_t outputTail;
    static size_t outputHead;
    static size_t outputNum;

    static Sem* inputBufferAvailable;


    //Methods
    static void initialize();           //allocates buffers and semaphores
    static void writeRequest(char c);   //from syscall to buffer
    static char readRequest();          //from buffer to syscall
    static void writeToConsole();       //from buffer to console
    static void readFromConsole();      //from console to buffer

    static bool inputEmpty();            //is input buffer full?
    static bool outputFull();           //is output buffer full?
    static void resolveWriteRequest();  //writes from next request to output buffer

    friend class RiscV;
    friend class ABI;
    friend class Scheduler;

};


#endif //PROJECT_BASE_CONSOLEHANDLER_H
