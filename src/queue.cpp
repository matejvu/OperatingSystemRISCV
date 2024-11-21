//
// Created by os on 7/24/23.
//
#include "../h/queue.h"

Queue::Queue() {
    list= nullptr;
}

Queue::~Queue() {
    while(list){
        Node* old = list;
        list = list->next;
        MemoryAllocator::mem_free(old);
    }
}

void Queue::put(size_t *p) {
    size_t numOfBlcks = (sizeof(Node)+MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    Node* newNode = (Node*)MemoryAllocator::mem_alloc(numOfBlcks);
    newNode->next = nullptr;
    newNode->pok = p;

    Node *prev= nullptr, *cur=list;
    for(;cur;prev=cur, cur = cur->next);
    if(!prev) list = newNode;
    else prev->next = newNode;
}

size_t *Queue::get() {
    if(!list)return nullptr;

    size_t* rVal = list->pok;
    Node* oldN = list;
    list = list->next;\
    //naknadno komentarisano
    //oldN->next = nullptr;
    //oldN->pok= nullptr;

    MemoryAllocator::mem_free(oldN);
    return rVal;
}

void Queue::clear() {
    while(list){
        this->get();
    }
}

void *Queue::operator new(size_t size) {
    size_t numOfBlcks = (sizeof (Queue) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return MemoryAllocator::mem_alloc(numOfBlcks);
}

void Queue::operator delete(void *a) {
    MemoryAllocator::mem_free(a);
}

