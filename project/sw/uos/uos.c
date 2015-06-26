#include <stdlib.h>
#include <string.h>

#define UOS_DEBUG
#ifdef UOS_DEBUG
#include <stdio.h>
#endif

#include "uos.h"

volatile int* __uos_procs = (volatile int*)(PROC_BASE);
volatile int* __uos_lock = (volatile int*)(LOCK_BASE);

volatile int* uos_get_base_lock_address()
{
    return (volatile int*)(LOCK_BASE);
}

void uos_acquire_processor_lock()
{
    while (*__uos_lock);
}
void uos_release_processor_lock()
{
    *__uos_lock = 0;
}

unsigned int running_processors = 0;
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
    return (exit_code);
}

int uos_start_processor(const int data)
{
    int status = 0;
    unsigned int proc_id = 0;
    if ( running_processors == NPROCS ) {
        status = -1;
    } else {
        uos_acquire_processor_lock();
#ifdef UOS_DEBUG
        printf("uOS) Allocating processor to run data segment %x.\n",data);
#endif
        for ( proc_id = 0; proc_id < NPROCS; proc_id++ ) {
            volatile uos_thread_t* thr = (volatile uos_thread_t*)__uos_procs[proc_id];
            if ( __uos_procs[proc_id] < 0 ) {//|| thr->func == NULL ) {
                __uos_procs[proc_id] = data;
#ifdef UOS_DEBUG
                printf("uOS) Data segment %x allocated to processor %d.\n",data, proc_id);
#endif
                uos_release_processor_lock();
                break;
            }
        }
        if ( proc_id == NPROCS ) {
            uos_release_processor_lock();
            return -1;
        }
        uos_release_processor_lock();
    }
    return status;
}

void uos_processor_retain()
{
    uos_acquire_processor_lock();
    running_processors++;
    uos_release_processor_lock();
}

void uos_processor_yield()
{
    uos_acquire_processor_lock();
    running_processors--;
    uos_release_processor_lock();
}

void uos_processor_halt(const int proc_id)
{
    uos_acquire_processor_lock();
    running_processors--;
    __uos_procs[proc_id] = -1;
    uos_release_processor_lock();
}

int uos_start()
{
    static int initialized = 0;
    static int initialized_cores = 0;

    unsigned int i;
    int pid = 0;
    uos_acquire_processor_lock();
    if ( !initialized ) {
#ifdef UOS_DEBUG
        printf("uOS) Initializing uos.\n");
#endif
        __uos_procs[0] = 0;
        memset((int*)(__uos_procs), 0, sizeof(int));
        memset((int*)(__uos_procs+1), 0xFF, (NPROCS-1)*sizeof(int));
        initialized = 1;
#ifdef UOS_DEBUG
        printf("uOS) Done initializing uos.\n");
#endif
    }
    pid = initialized_cores++;
    uos_release_processor_lock();
    return pid;
}

int uos_main(int argc, char** argv)
{
    int pid = uos_start();

    if (!pid) {
#ifdef UOS_DEBUG
        uos_acquire_processor_lock();
        printf("uOS) Starting main processor. pid=%d\n", pid);
        uos_release_processor_lock();
#endif
        application_entry(argc, argv);
        SHOULD_TERMINATE = 1;
    } else {
#ifdef UOS_DEBUG
        uos_acquire_processor_lock();
        printf("uOS) Starting auxiliary processor. pid=%d\n", pid);
        uos_release_processor_lock();
#endif
        while (!SHOULD_TERMINATE) {
            uos_acquire_processor_lock();
            uos_thread_t* thr = (uos_thread_t*)__uos_procs[pid];
            uos_release_processor_lock();
            if (thr != NULL && thr->func != NULL) {
                thr->func(thr->arg);
            }
        }
    }
    return 0;
}

