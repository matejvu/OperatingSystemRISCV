//
// Created by os on 8/10/23.
//

#include "../h/syscall_cpp.hpp"

void* operator new (size_t s){
    return mem_alloc(s);
}

void operator delete (void* a){
    mem_free(a);
}

Thread::Thread() {
    this->body = nullptr;
    this->arg = this;
    this->myHandle = nullptr;
}

Thread::Thread(void (*body)(void *), void *arg) {
    this->body = body;
    this->arg = arg;
    this->myHandle = nullptr;
}

Thread::~Thread() {
    delete this->myHandle;
}

int Thread::start() {
    int status;

    if(this->body) {
        status = thread_create(&this->myHandle, this->body, this->arg);
    }
    else{
        status = thread_create(&this->myHandle, &Thread::runner, (void*)this);
    }
    return status;
}

void Thread::join() {
    thread_join(this->myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t t) {
    return time_sleep(t);
}

void Thread::runner(void * t) {
    Thread* th = (Thread*) t;
    th->run();
}


Semaphore::Semaphore(unsigned int init) {
    sem_open(&this->myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(this->myHandle);
}

int Semaphore::wait() {
    return sem_wait(this->myHandle);
}

int Semaphore::signal() {
    return sem_signal(this->myHandle);
}


void PeriodicThread::terminate() {
    this->period = 0;
}

PeriodicThread::PeriodicThread(time_t period) : Thread() {
    this->period = period;
}

void PeriodicThread::run() {
    while(this->period){
        time_sleep(this->period);
        this->periodicActivation();
    }
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
   ::putc(c);
}
