//
// Created by os on 7/20/23.
//
#include "../lib/console.h"
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"

void fija2(void* s){
    //sem_t sem1 = (sem_t)s;
    int zbir=-1000;
    for(int i=0; i<10;i++) {
        putc('2');
        for(int j=0;j<10000000;j++){
            zbir+=j;
        }
        //sem_signal(sem1);
        thread_dispatch();
    }
}

void fija1(void* s){
   // sem_t sem1 = (sem_t)s;
   int suma = -2133;
    for(int i=0; i<10;i++) {
      //  sem_wait(sem1);
        putc('1');
        for(int j=0;j<10000000;j++){
            suma+=j*2 - 100;
        }
        thread_dispatch();
    }
    //sem_signal(sem1);
}


void fijaSem(void* s){
    sem_t sem1 = (sem_t)s;


    __putc('C');
    __putc('E');
    __putc('K');
    __putc('A');
    __putc('J');
    __putc('\n');
    int flg = sem_wait(sem1);
    if(flg == 0){ __putc('0');}
    if(flg == -1){ __putc('-');__putc('1');}
    if(flg == -2){ __putc('-');__putc('2');}
    __putc('\n');

}

class perioda : public PeriodicThread{
public:
    perioda(time_t t): PeriodicThread(t){};
protected:
    virtual void periodicActivation() {
        putc('p');
        putc('e');
        putc('r');
        putc('i');
        putc('o');
        putc('d');
        putc('a');
        putc('\n');
    }
};

void userMain2(void* a){

    perioda* nit1 = new perioda(10);
    nit1->start();

    sem_t sem1;
    putc('O');
    putc('T');
    putc('V');
    putc('O');
    putc('R');
    putc('I');
    sem_open(&sem1, 0);
    putc('\n');

    mem_alloc(34635);

    //thread_t t;

    time_sleep(20);
    putc('a');
    /*thread_t t;

    int flg1 = thread_create(&t, fija1, sem1);
    if(flg1){
        __putc('S');
        __putc('2');
        __putc('\n');
    }
    else{
        __putc('f');
        __putc('l');
        __putc('g');
        __putc('1');
        __putc('=');
        __putc('0');
        __putc('\n');
    }
    int flg2 = thread_create(&t, fija2, sem1);
    if(flg2){
        __putc('S');
        __putc('3');
        __putc('\n');
    }
    else{
        __putc('f');
        __putc('l');
        __putc('g');
        __putc('2');
        __putc('=');
        __putc('0');
        __putc('\n');
    }


    thread_t handle2;
    thread_create(&handle2, fijaSem, sem1);
    thread_dispatch();*/

    sem_close(sem1);




    Thread* nit = new Thread(fija2, nullptr);
    nit->start();



    nit1->terminate();


    putc('\n');
    putc('G');
    putc('E');
    putc('T');
    putc('C');
    putc('\n');


    char c;
    c = getc();

    putc(c);

}



void(*fPok)(void*) = userMain2;