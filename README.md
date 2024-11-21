&emsp; This repo represents the implementation of an Operating system designed for RISC-V core emulator. It contains two directories h and src.
<br> &emsp; Functionalities: Work with memory(allocation/dealocation), Multithreading, Synchronizations(semaphores), Thread sleeping, Work with console, Async and sync interrupts. 
<br> &emsp; The former one contains all C/C++ header files including the ABI,and syscall files with declarations of system calls that can be used from user programs.
<br> &emsp; The latter keeps implementation of the whole system where riscV.cpp represents the main of the core and userMain files are examples of user programs that run different tests.
<br><br>
### C API:
- thread_create
- thread_exit
- thread_dispatch
- thread_join
- sem_open
- sem_close
- sem_wait
- sem_signal
- time_sleep
- getc
- putc
### C++ API:
- Thread::Thread
- Thread::start
- Thread::join
- Thread::dispatch
- Thread::sleep
- Semaphore::Semaphore
- Semaphore::wait
- Semaphore::signal
- PeriodicThread::terminate
- Console::getc
- Console::putc