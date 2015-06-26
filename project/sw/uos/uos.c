#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "uos.h"

volatile int* __uos_procs = (volatile int*)(PROC_BASE);
volatile int* __uos_lock = (volatile int*)(LOCK_BASE);

volatile int* uos_get_base_lock_address()
{
    return (volatile int*)(LOCK_BASE + 1);
}

void uos_acquire_processor_lock()
{
    while (*__uos_lock);
}
void uos_release_processor_lock()
{
    *__uos_lock = 0;
}

unsigned int started_processors = 0;
unsigned int SHOULD_TERMINATE = 0;

int uos_exit(int exit_code)
{
    static unsigned int exited = 0;
    int p = 0;

    uos_acquire_processor_lock();
    exited++;
    SHOULD_TERMINATE = 1;
    for ( p = 0; p < NPROCS; p++ ) {
        __uos_procs[p] = 0;
    }
    uos_release_processor_lock();
    while ( exited != NPROCS );
    return exit_code;
}

int uos_start_processor(const int data)
{
    int status = 0;
    unsigned int proc_id = 0;
    if ( started_processors == NPROCS ) {
        status = -1;
    } else {
        uos_acquire_processor_lock();
        for ( proc_id = 0; proc_id < NPROCS; proc_id++ ) {
            if ( __uos_procs[proc_id] < 0 ) {
                __uos_procs[proc_id] = data;
                uos_release_processor_lock();
                break;
            }
        }
        uos_release_processor_lock();
    }
    return status;
}

int uos_processor_retain()
{
    int tmp = 0;
    uos_acquire_processor_lock();
    tmp = started_processors++;
    uos_release_processor_lock();
    return tmp;
}

void uos_processor_yield(const int proc_id)
{
    uos_acquire_processor_lock();
    started_processors--;
    uos_release_processor_lock();
}

void uos_processor_halt(const int proc_id)
{
    uos_acquire_processor_lock();
    started_processors--;
    __uos_procs[proc_id] = -1;
    uos_release_processor_lock();
}

void uos_start()
{
    memset((int*)__uos_procs, 0xFF, NPROCS * sizeof(__uos_procs[0]));
}

int uos_main(int argc, char** argv)
{
    int pid;

    uos_acquire_processor_lock();
    pid = started_processors;
    uos_release_processor_lock();

    if (!pid) {
        uos_processor_retain();
        application_entry(argc, argv);
        SHOULD_TERMINATE = 1;
        uos_processor_yield(0);
    } else {
        pid = uos_processor_retain();
        while (!SHOULD_TERMINATE) {
            uos_acquire_processor_lock();
            uos_thread_t* thr = (uos_thread_t*)__uos_procs[pid];
            uos_release_processor_lock();
            if (thr != NULL && thr->func != 0) {
                thr->func(thr->arg);
                uos_processor_yield(pid);
            } else {
                uos_processor_halt(pid);
            }
            uos_processor_retain();
        }
        uos_processor_yield(pid);
    }
    return 0;
}

