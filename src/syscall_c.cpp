//
// Created by os on 7/18/23.
//
#include "../h/syscall_c.h"

//a0 -> A7
//a1 -> A6
//a2 -> A5
//a3 -> A4
//a4 -> A3
//a5 -> A2
//a6 -> A1
//a7 -> A0

//Memory allocator
void* mem_alloc (size_t size){
    size_t code = 1;
    size_t numOfBlcks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;

    //size_t ra;
    //__asm__ volatile ("mv  %0, x1" : "=r"(ra));

    __asm__ volatile("mv a1, %0" : : "r"(code));
    __asm__ volatile("mv a2, %0" : : "r"(numOfBlcks));

    __asm__ volatile("ecall");

    size_t rVal;
    __asm__ volatile("mv %0, a0": "=r"(rVal));
    //__asm__ volatile ("mv ra, %0" : : "r"(ra));
    return (void*) rVal;
}

int mem_free (void* a){
    size_t code = 2;
    __asm__ volatile("mv a1, %0" : : "r"(code));
    __asm__ volatile("mv a2, %0" : : "r"(a));


    __asm__ volatile("ecall");

    size_t rVal;
    __asm__ volatile("mv %0, a0": "=r"(rVal));
    return (int) rVal;
}


//Threads

/*
class _thread{
public:
    static void* operator new(size_t size){
        return mem_alloc(size);
    }
    static void operator delete(void* a){
        mem_free(a);
    }
private:
    struct _tNode{
        PCB* proc;
        _tNode* next;
        _tNode(PCB* h){this->proc = h; this->next= nullptr;}
        static void* operator new(size_t size){return mem_alloc(size);}
        static void operator delete(void* a){ mem_free(a);}
    };
    static _tNode* allThreads;
    PCB* process = nullptr;
    _thread(PCB* t){
        this->process = t;
        _tNode* newNode = new _tNode(t);
        _tNode *cur = allThreads, *prev = nullptr;
        for(;cur;cur=cur->next);
        if(!prev)allThreads = newNode;
        else prev->next = newNode;
    }

    friend class ABI;
};*/

int thread_create(PCB** handle, void (*start_routine)(void *), void *arg) {
    void* stack = mem_alloc(DEFAULT_STACK_SIZE);
    if(stack){ stack = (void*)((size_t)stack + DEFAULT_STACK_SIZE - 16); }
    else return -1; //Unable to allocate memory for stack

    size_t code = 11;
    __asm__ volatile("mv a1, %0" : : "r"(code));
    __asm__ volatile("mv a2, %0" : : "r"(handle));
    __asm__ volatile("mv a3, %0" : : "r"(start_routine));
    __asm__ volatile("mv a4, %0" : : "r"(arg));
    __asm__ volatile("mv a6, %0" : : "r"(stack));


    __asm__ volatile("ecall");

    size_t rVal;
    __asm__ volatile("mv %0, a0": "=r"(rVal));
    return (int) rVal;
}

int thread_exit() {
    size_t code = 12;
    __asm__ volatile("mv a1, %0" : : "r"(code));

    __asm__ volatile("ecall");

    size_t rVal;
    __asm__ volatile("mv %0, a0": "=r"(rVal));
    return (int) rVal;
}

void thread_dispatch() {
    size_t code = 13;
    __asm__ volatile("mv a1, %0" : : "r"(code));

    __asm__ volatile("ecall");
}

void thread_join(thread_t handle) {
    size_t code = 14;
    __asm__ volatile("mv a1, %0" : : "r"(code));
    __asm__ volatile("mv a2, %0" : : "r"(handle));

    __asm__ volatile("ecall");
}


//Sem
int sem_open(sem_t *handle, unsigned int init) {
    size_t code = 21;
    __asm__ volatile("mv a3, %0" : : "r"(init));
    __asm__ volatile("mv a2, %0" : : "r"(handle));
    __asm__ volatile("mv a1, %0" : : "r"(code));

    __asm__ volatile("ecall");

    size_t rVal;
    __asm__ volatile("mv %0, a0": "=r"(rVal));
    return (int) rVal;
}

int sem_close(sem_t handle) {
    size_t code = 22;
    __asm__ volatile("mv a1, %0" : : "r"(code));
    __asm__ volatile("mv a2, %0" : : "r"(handle));

    __asm__ volatile("ecall");

    size_t rVal;
    __asm__ volatile("mv %0, a0": "=r"(rVal));
    return (int) rVal;
}

int sem_wait(sem_t id) {
    size_t code = 23;
    __asm__ volatile("mv a1, %0" : : "r"(code));
    __asm__ volatile("mv a2, %0" : : "r"(id));

    __asm__ volatile("ecall");

    size_t rVal;
    __asm__ volatile("mv %0, a0": "=r"(rVal));
    return (int) rVal;
}

int sem_signal(sem_t id) {
    size_t code = 24;
    __asm__ volatile("mv a1, %0" : : "r"(code));
    __asm__ volatile("mv a2, %0" : : "r"(id));

    __asm__ volatile("ecall");

    size_t rVal;
    __asm__ volatile("mv %0, a0": "=r"(rVal));
    return (int) rVal;
}

int time_sleep(time_t t) {
    size_t code = 31;
    __asm__ volatile("mv a1, %0" : : "r"(code));
    __asm__ volatile("mv a2, %0" : : "r"(t));

    __asm__ volatile("ecall");

    size_t rVal;
    __asm__ volatile("mv %0, a0": "=r"(rVal));
    return (int) rVal;
}

char getc() {
    //return __getc();

    size_t code = 41;
    size_t rVal;
    while(1){
        __asm__ volatile("mv a1, %0" : : "r"(code));
        __asm__ volatile("ecall");


        __asm__ volatile("mv %0, a0": "=r"(rVal));
        if(rVal!=(char)-3)break;
        size_t semaphore;
        __asm__ volatile("mv %0, a7": "=r"(semaphore));
        sem_wait((sem_t)semaphore);
    }


    return (char)rVal;

}

void putc(char c) {
    //__putc(c);

    size_t code = 42;
    __asm__ volatile("mv a1, %0" : : "r"(code));
    __asm__ volatile("mv a2, %0" : : "r"(c));

    __asm__ volatile("ecall");

}


