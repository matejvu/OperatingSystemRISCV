//
// Created by os on 7/26/23.
//

#include "../h/sleepingThreads.h"

SleepingThread::SleepNode* SleepingThread::


list = nullptr;

void SleepingThread::putToSleep(PCB *t, unsigned int time) {


    SleepNode* newSleepNode;

    size_t numOfBlcks = (sizeof(SleepNode) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    newSleepNode = (SleepNode*)MemoryAllocator::mem_alloc(numOfBlcks);

    newSleepNode->next = nullptr;
    newSleepNode->thread = t;

    if(!list){
        newSleepNode->timeLeft = time;
        list = newSleepNode;
        return;
    }

    if(list->timeLeft >= time){
        list->timeLeft-=time; //prvi je razlika, ostale povecaj
        newSleepNode->next = list;
        newSleepNode->timeLeft = time;


        list = newSleepNode;
        return;
    }

    time = time - list->timeLeft;
    SleepNode *prev = list, *cur = list->next;
    while(cur){
        if(cur->timeLeft >= time){
            cur->timeLeft -= time;
            newSleepNode->timeLeft = time;
            newSleepNode->next = cur;
            prev->next = newSleepNode;
            return;
        }
        time -= cur->timeLeft;
        prev = cur;
        cur = cur->next;
    }

    prev->next = newSleepNode;
    newSleepNode->timeLeft = time;
    newSleepNode->next = nullptr;




}

void SleepingThread::updateTime() {
    if(!list)return;

    list->timeLeft--;

    if(list->timeLeft>0)return;

    while(list && list->timeLeft<=0){
        awakeThread(list->thread);
        list->thread = nullptr;
        SleepNode* old = list;
        list = list->next;

        MemoryAllocator::mem_free(old);
    }
}

void SleepingThread::awakeThread(PCB *t) {
    t->status = PCB::READY;
    Scheduler::Instance()->putThread(t);
}
