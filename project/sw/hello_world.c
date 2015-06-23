#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOCK_ADDRESS 0x800000
#define PROC_CTRL_ADDR (LOCK_ADDRESS << 1)
volatile int *lock = (volatile int *) LOCK_ADDRESS;

int pcount = 0;
int total_procs = 0;

void AcquireLock(){
    while (*lock);
}

void ReleaseLock(){
    *lock = 0;
}

void Barrier() {
    AcquireLock();
    total_procs--;
    ReleaseLock();
    while (total_procs);
}

unsigned int Procs[8] = {1,0,0,0,0,0,0,0};

void yield_proc(unsigned int procId) {
    AcquireLock();
    Procs[procId] = 0;
    ReleaseLock();
}

typedef void (*thr_run_t)(unsigned int procId, void(*func)(void*),void* args);
void thr_run(unsigned int procId, void (*func)(void*), void* args) {
    //func(args);
    AcquireLock();
    printf("I am a thread.\n");
    ReleaseLock();
    yield_proc(procId);
}

void start_thread(void (*func)(void*), void* args) {
    unsigned int p = 8;
    while (p >= 8) {
        AcquireLock();
        for (p = 0; p < 8 && Procs[p]; p++);
        Procs[p] = 1;
        ReleaseLock();
    }

    volatile thr_run_t* control_addr = (volatile thr_run_t*)(PROC_CTRL_ADDR + p);

    int upper_u = 0xFF000000 & (int)thr_run;
    int upper_l = 0x00FF0000 & (int)thr_run;
    int lower_u = 0x0000FF00 & (int)thr_run;
    int lower_l = 0x000000FF & (int)thr_run;

    volatile thr_run_t addr = (volatile thr_run_t)(((upper_u>>8) | (upper_l<<8)>>16) | (((lower_u >> 8) | (lower_l << 8) )<< 16));
    AcquireLock();
    printf("thr_run addr = %p\n", thr_run);
    ReleaseLock();
    *control_addr = addr;
    //*control_addr = thr_run;
}

void SayHello(void* args) {
    int i;
    for(i=0;i<10;i++){
        AcquireLock();
        printf("Hi from processor %d!\n", (int)args);
        ReleaseLock();
    }
}

int main(int argc, char *argv[]){
    int pid;

    AcquireLock();
    pid = pcount++;
    total_procs++;
    ReleaseLock();

    start_thread(SayHello, (void*)pid);

    Barrier();

    exit(0); // To avoid cross-compiler exit routine
    return 0; // Never executed, just for compatibility
}

