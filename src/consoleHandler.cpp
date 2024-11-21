//
// Created by os on 8/14/23.
//


#include "../h/consoleHandler.h"


bool ConsoleHandler::initialized = 0;
char* ConsoleHandler::inputBuffer = nullptr;
char* ConsoleHandler::outputBuffer = nullptr;
Sem* ConsoleHandler::inputBufferAvailable = nullptr;
ConsoleHandler::WriteRequest* ConsoleHandler::requests = nullptr;
size_t ConsoleHandler::inputHead = 0;
size_t ConsoleHandler::inputTail = 0;
size_t ConsoleHandler::inputNum = 0;
size_t ConsoleHandler::outputHead = 0;
size_t ConsoleHandler::outputTail = 0;
size_t ConsoleHandler::outputNum = 0;

void ConsoleHandler::initialize() {
    if (!initialized) {
        ConsoleHandler::inputBuffer = (char *) MemoryAllocator::mem_alloc(1);
        ConsoleHandler::outputBuffer = (char *) MemoryAllocator::mem_alloc(1);

        //size_t numOfBlcks = (sizeof (Sem) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
        ConsoleHandler::inputBufferAvailable = new Sem(0);/*(Sem*)MemoryAllocator::mem_alloc(numOfBlcks);
        ConsoleHandler::inputBufferAvailable->val = 0;*/
    }
    initialized = 1;
}

void ConsoleHandler::writeRequest(char c) {
    initialize();
    if(outputFull()){
        WriteRequest* newRequest;
        size_t numOfBlcks = (sizeof (WriteRequest) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
        newRequest = (WriteRequest*)MemoryAllocator::mem_alloc(numOfBlcks);

        newRequest->c = c;
        newRequest->next = nullptr;

        WriteRequest* prev = nullptr, *cur = requests;
        for(;cur;prev = cur, cur = cur->next);
        if(!prev)requests = newRequest;
        else prev->next = newRequest;
        return;
    }

    outputBuffer[outputTail] = c;
    outputTail = (outputTail+1) % MEM_BLOCK_SIZE;
    outputNum++;

    writeToConsole();
}

char ConsoleHandler::readRequest() {
    initialize();
    if(inputEmpty())return -3;

    char rVal = inputBuffer[inputHead];
    inputHead = (inputHead+1) % MEM_BLOCK_SIZE;
    inputNum--;

    return rVal;
}

void ConsoleHandler::writeToConsole() {
    initialize();
    while(1){
        size_t consoleStatus = *(char*)CONSOLE_STATUS;
        if(outputNum<=0 || !(consoleStatus & STATUS_READY_OUT))break;

        *(char*)CONSOLE_TX_DATA = outputBuffer[outputHead];
        outputHead = (outputHead+1) % MEM_BLOCK_SIZE;
        outputNum--;
        resolveWriteRequest();
    }
}

void ConsoleHandler::readFromConsole() {
    initialize();
    while(1){
        size_t consoleStatus = *(char*)CONSOLE_STATUS;
        if(inputNum>=MEM_BLOCK_SIZE || !(consoleStatus & STATUS_READY_IN))break;

        inputBuffer[inputTail] = *(char*)CONSOLE_RX_DATA;
        inputTail = (inputTail+1) % MEM_BLOCK_SIZE;
        inputNum++;
        inputBufferAvailable->signal();
    }
}

bool ConsoleHandler::inputEmpty() {
    if(inputNum<=0)return true;
    return false;
}

bool ConsoleHandler::outputFull() {
    if(outputNum>=MEM_BLOCK_SIZE)return true;
    return false;
}

void ConsoleHandler::resolveWriteRequest() {
    if(!requests)return;    //no requests

    WriteRequest* old = requests;
    requests = requests->next;

    //doesnt care about fullness of buffer so check before calling the function
    outputBuffer[outputTail] = old->c;
    outputTail = (outputTail+1) % MEM_BLOCK_SIZE;
    outputNum++;

    MemoryAllocator::mem_free(old);
}




