#include "../h/memoryAllocator.h"

int MemoryAllocator::initialized = 0;
MemoryAllocator::FreeMem* MemoryAllocator::fmem_head = nullptr;
//MemoryAllocator::FreeMem* MemoryAllocator::fmem_head = (FreeMem*) HEAP_START_ADDR; //0x80005630;

void MemoryAllocator::initialize(){
    if(!initialized) {
        initialized = 1;
        MemoryAllocator::fmem_head = (FreeMem*) HEAP_START_ADDR;
        MemoryAllocator::fmem_head->next = 0;
        MemoryAllocator::fmem_head->prev = 0;
        MemoryAllocator::fmem_head->numOfBlcks = ((uchar*)HEAP_END_ADDR - (uchar*)HEAP_START_ADDR + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    }
}

size_t MemoryAllocator::mem_free(void* a){
    //uchar* address = (uchar*)a;
    initialize();/*
    //Descriptor* descPtr = (Descriptor*)address-1;
    Descriptor* descPtr = (Descriptor*)((size_t)address-MEM_BLOCK_SIZE);
    size_t numOfBlock = descPtr->numOfBlock;
    address = (uchar*) descPtr;

    if(!fmem_head || address<HEAP_START_ADDR || numOfBlock<0 || address>=HEAP_END_ADDR) return -1;
    if(numOfBlock==0)return 0;
    FreeMem* cur;

    //dobar
    FreeMem* previous = (FreeMem*)address;
    previous->numOfBlock = numOfBlock;
    if((size_t)address<(size_t)fmem_head){
        previous->prev = 0;
        previous->next = fmem_head;
        fmem_head->prev = previous;
        fmem_head = previous;
    }
    for(cur = fmem_head; cur->next; cur=cur->next){
        if((size_t) cur < (size_t)address && (size_t)address < (size_t)cur->next) {
            previous->prev = cur;
            previous->next = cur->next;
            previous->prev->next = previous;
            previous->next->prev = previous;
        }
    }
    if(!cur->next && (size_t)address>(size_t)cur){
        previous->next = 0;
        previous->prev = cur;
        cur->next = previous;
    }

    tryToJoin(previous); //proba sppajanje sa prethodnim
    tryToJoin(previous->next); //proba spajanje sa prethodnim
    */

    size_t* desc = (size_t*)((size_t)a - MEM_BLOCK_SIZE);
    size_t size = *desc;

    FreeMem* newFreeBlck = (FreeMem*)desc;
    newFreeBlck->numOfBlcks = size;

    FreeMem* prev = nullptr, * cur = fmem_head;

    for (; cur; prev = cur, cur = cur->next) {
        if (newFreeBlck < cur)break;
    }

    if (!prev)fmem_head = newFreeBlck;
    else prev->next = newFreeBlck;

    newFreeBlck->next = cur;

    tryToJoin(newFreeBlck);
    if (prev)tryToJoin(prev);

    return 0;
}

// Helper: Try to join cur with the cur->next free segment:
int MemoryAllocator::tryToJoin (FreeMem* cur) {
    //FreeMem* cur = (FreeMem*)a;
    if (!cur->next)return-1;

    if ((size_t)cur + cur->numOfBlcks * MEM_BLOCK_SIZE != (size_t)cur->next)return-2;

    cur->numOfBlcks = cur->numOfBlcks + cur->next->numOfBlcks;
    cur->next = cur->next->next;

    /*
    //pokusava spajanje sa prethodnim
    if(!cur)return -1;
    if(!cur->prev)return -1;
    if((size_t)cur->prev + cur->prev->numOfBlock*MEM_BLOCK_SIZE != (size_t)cur)return -1;

    cur->prev->numOfBlock+=cur->numOfBlock;
    cur->prev->next = cur->next;
    if(cur->next) cur->next->prev = cur->prev;*/

    return 0;
}

void* MemoryAllocator::mem_alloc (size_t s){
    initialize();
    /*
    size_t neededBlocks = size + 1; //(size + sizeof (Descriptor) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    FreeMem* cur=0;

    for(cur =  fmem_head; cur; cur = cur->next){
        if(cur->numOfBlock>=neededBlocks)break;
    }



    if(!cur)return nullptr;

    if(cur->numOfBlock-neededBlocks){
        FreeMem* newFreeBlk = (FreeMem*)((size_t)cur+neededBlocks*MEM_BLOCK_SIZE);
        newFreeBlk->next = cur->next;
        newFreeBlk->prev = cur->prev;
        newFreeBlk->numOfBlock = cur->numOfBlock-neededBlocks;
        if(!cur->prev)fmem_head=newFreeBlk;
        else cur->prev->next = newFreeBlk;
    }
    else{
        if(!cur->prev){fmem_head = cur->next; fmem_head->prev = nullptr; }
        else cur->prev->next = cur->next;
    }

    ((Descriptor*)cur)->numOfBlock = neededBlocks;
    cur=(FreeMem*)((size_t)cur + MEM_BLOCK_SIZE);

    return (void*)cur;*/

    s += 1;

    MemoryAllocator::FreeMem* prev = nullptr, * cur = MemoryAllocator::fmem_head;

    for (; cur; prev = cur, cur = cur->next) {
        if (cur->numOfBlcks >= s)break;
    }

    if (!cur)return nullptr; //not enough space

    if (cur->numOfBlcks - s <= 1) { //no free space left
        if (!prev)MemoryAllocator::fmem_head = cur->next;
        else prev->next = cur->next;
        s = cur->numOfBlcks;
    }
    else {
        MemoryAllocator::FreeMem* newFreeBlck = (MemoryAllocator::FreeMem*)((size_t)cur + MEM_BLOCK_SIZE * s);
        if (!prev)MemoryAllocator::fmem_head = newFreeBlck;
        else prev->next = newFreeBlck;

        newFreeBlck->next = cur->next;
        newFreeBlck->numOfBlcks = cur->numOfBlcks - s;
    }
    size_t* desc = (size_t*)cur;
    *desc = s;

    return (void*)((size_t)desc + MEM_BLOCK_SIZE);

}

