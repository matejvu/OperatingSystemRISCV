//
// Created by os on 7/18/23.
//

#ifndef PROJECT_BASE_SYSCALL_C_H
#define PROJECT_BASE_SYSCALL_C_H

#include "../h/riscV.h"
#include "../lib/hw.h"


void* mem_alloc (size_t size);          //01
int mem_free (void*);                   //02
//class _thread;
typedef PCB _thread;
typedef _thread* thread_t;
int thread_create(
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
        );                              //11
int thread_exit();                      //12
void thread_dispatch();                 //13
void thread_join(thread_t handle);      //14
typedef Sem _sem;
typedef _sem* sem_t;
int sem_open(
        sem_t* handle,
        unsigned init
        );                              //21
int sem_close(sem_t handle);            //22
int sem_wait(sem_t id);                 //23
int sem_signal(sem_t id);               //24
typedef unsigned long time_t;
int time_sleep(time_t);                 //31
const int EOF = -1;
char getc();                            //41
void putc(char);                        //42




#endif //PROJECT_BASE_SYSCALL_C_H
