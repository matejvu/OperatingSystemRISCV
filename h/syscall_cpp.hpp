//
// Created by os on 8/9/23.
//

#ifndef PROJECT_BASE_SYSCALL_CPP_HPP
#define PROJECT_BASE_SYSCALL_CPP_HPP



//#ifndef _syscall_cpp
//#define _syscall_cpp
#include "syscall_c.h"

void* operator new (size_t);
void operator delete (void*);
class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    void join();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {};
private:
    thread_t myHandle;
    void (*body)(void*); void* arg;
    static void runner(void*);
};

class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}

    virtual void run();

private:
    time_t period;
};


class Console {
public:
    static char getc ();
    static void putc (char);
};




#endif
