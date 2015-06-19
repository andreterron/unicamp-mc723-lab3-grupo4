#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOCK_ADDRESS 0x800000
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

int main(int argc, char *argv[]){
    int i;
    int pid;

    AcquireLock();
    pid = pcount++;
    total_procs++;
    ReleaseLock();

    for(i=0;i<10;i++){
        AcquireLock();
        printf("Hi from processor %d!\n", pid);
        ReleaseLock();
    }

    Barrier();

    exit(0); // To avoid cross-compiler exit routine
    return 0; // Never executed, just for compatibility
}

