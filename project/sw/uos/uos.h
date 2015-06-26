#ifndef _UOS_H_
#define _UOS_H_

#define NPROCS 8
#define LOCK_BASE 0x800000
#define PROC_BASE (LOCK_BASE << 1)

typedef void (*uos_thread_func)(void*);
typedef struct _uos_thread_t {
    uos_thread_func     func;
    void*               arg;
} uos_thread_t;

//typedef int (*application_entry_t)(int, char**);
//void uos_set_application_entry(application_entry_t entry_point);
extern int application_entry(int argc, char**argv);

volatile int* uos_get_base_lock_address();

void uos_start();
int uos_exit(int exit_code);
int uos_start_processor(const int data);
int uos_main(int argc, char** argv);
void uos_acquire_processor_lock();
void uos_release_processor_lock();

#endif //_UOS_H_

