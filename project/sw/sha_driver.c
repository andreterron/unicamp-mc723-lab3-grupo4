/********************************************************************************
 *
 SHA Secure Hash Algorithm
 *                                                          Parallel 1.0 Version
 * Authors:             Yuchen Liang and Syed Muhammad Zeeshan Iqbal
 * Supervisor:                  Hakan Grahn
 * Section:             Computer Science of BTH,Sweden
 * GCC Version:             4.2.4
 * Environment:                 Kraken(Ubuntu4.2.4) with Most 8 Processors
 * Start Date:          27th October 2009
 * End Date:            23th November 2009
 *
 *********************************************************************************/


/* NIST Secure Hash Algorithm */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "pthread.h"
#include "sha.h"

#define MAX_NO_FILES 16
#define MAX_WORKERS 8
#define PRINT 1
#define PARALLEL 1

typedef struct Params {
    int s_index;
    int e_index;
    int no;
    char *name;
} parameters;

struct fileData{
    BYTE* data;
    int size;
}Tdata[MAX_NO_FILES];

char* in_file_list[]={"in_file1.asc","in_file2.asc","in_file3.asc","in_file4.asc","in_file5.asc","in_file6.asc","in_file7.asc","in_file8.asc","in_file9.asc","in_file10.asc","in_file11.asc","in_file12.asc","in_file13.asc","in_file14.asc","in_file15.asc","in_file16.asc",NULL};
char* out_file_list[]={"out_file1.txt","out_file2.txt","out_file3.txt","out_file4.txt","out_file5.txt","out_file6.txt","out_file7.txt","out_file8.txt","out_file9.txt","out_file10.txt","out_file11.txt","out_file12.txt","out_file13.txt","out_file14.txt","out_file15.txt","out_file16.txt",NULL};

int no_files=16;
int no_workers=6;

#define aprintf(fmt,...) \
    do { \
        pthread_acquire(); \
        printf(fmt,##__VA_ARGS__); \
        pthread_release(); \
    } while (0)


//pthread_attr_t string_attr;
//pthread_mutex_t string_mutex;
pthread_t workers[MAX_WORKERS];

static int partition_size;
static parameters paramsArr[MAX_WORKERS];

void readFilesData(){
    FILE *fin;
    int index;

    for ( index=0; index<no_files; index++){
        char buf[256];
        snprintf(buf, 256, "sw/input/%s", in_file_list[index]);
        if ((fin = fopen(buf, "rb")) == NULL) {
            aprintf("Error opening %s for reading\n", buf);
        }
        // Calculate File Size
        fseek(fin, 0, SEEK_END);
        Tdata[index].size = ftell(fin);
        fseek(fin, 0, SEEK_SET);

        Tdata[index].data = (BYTE *) calloc(sizeof(BYTE),Tdata[index].size+5);

        if (Tdata[index].size != fread(Tdata[index].data, sizeof(BYTE), Tdata[index].size, fin))            {
            free(Tdata[index].data);
            aprintf("Error: Could not read file!\n");
        }
    }
    aprintf("File reading completed!\n");
}


void compute_digest(int index){

    SHA_INFO sha_info;

    sha_stream(&sha_info, Tdata[index].data,Tdata[index].size);

    if (PRINT) {
        sha_print_to_file(&sha_info, fopen(out_file_list[index], "w"));
    }
}

void *parallel_SHA_Dig_Calc(void *params){

    int index;
    parameters *param=(parameters *)params;

    if (PRINT) {
        aprintf("Worker number %d: started calculating digest.\n",param->no);
    }
    for (index=param->s_index; index<param->e_index; index++){
        compute_digest(index);
    }

    if (PRINT) {
        aprintf("Worker number %d: finished calculating digest.\n",param->no);
    }
}

void init_workers(){

    //pthread_attr_init(&string_attr);
    //pthread_mutex_init(&string_mutex,NULL);
    //pthread_attr_setdetachstate(&string_attr,PTHREAD_CREATE_JOINABLE);

}

void createWorkers(){
    long index;
    for ( index= 0 ; index<no_workers; index++){
        pthread_create(&workers[index],NULL,parallel_SHA_Dig_Calc,(void *)&paramsArr[index]);
    }

}
void waitForWorkersToFinish(){
    int index;
    for ( index= 0 ; index<no_workers; index++)
        pthread_join(workers[index],NULL);

}

void divide_into_sub_tasks(){

    int i;
    int s_index=0;



    for (i=0;i<no_workers;i++) {
        paramsArr[i].s_index=s_index;
        paramsArr[i].e_index=s_index+partition_size;
        paramsArr[i].name="Worker";
        paramsArr[i].no=i+1;
        s_index+=partition_size;

    }

    // It handles even not of jobs and odd no of workers vice versa.
    paramsArr[i-1].e_index=paramsArr[i-1].e_index+(no_files%no_workers);

}

void show_sub_tasks() {

    int i;
    for (i=0;i<no_workers;i++) {

        aprintf("%d   ",paramsArr[i].s_index);
        aprintf("%d   ",paramsArr[i].e_index);
        aprintf("%s ",paramsArr[i].name);
        aprintf("%d \n ",paramsArr[i].no);
    }
}

void init(){
    partition_size= MAX_NO_FILES/no_workers;
    divide_into_sub_tasks();
    if (PRINT)
        show_sub_tasks();
}

void parallel_process(){
    aprintf("*******  Digest Calucation:Parallel Process  *******\n");

    init();
    init_workers();
    createWorkers();
    waitForWorkersToFinish();

}

void sequential_process(){

    int index;

    aprintf("*******  Digest Calucation:Sequential Process  *******\n");

    for (index=0; index<no_files; index++){
        if (PRINT){
            aprintf("-------*******************************-------------------\n");
            aprintf("Computing Digest for file: %s\n",in_file_list[index]);
        }

        compute_digest(index);

        if (PRINT){
            aprintf("Digest written to  file: %s\n",out_file_list[index]);
            aprintf("-------*******************************-------------------\n");
        }
    }

}


int application_entry(int argc,char *argv[])
{
    if(PARALLEL) {
        readFilesData();
        parallel_process();
    } else {
        readFilesData();
        sequential_process();
    }
    return(0);
}
