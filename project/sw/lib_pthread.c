#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib_pthread.h"

void pthread_create() {
	printf("MOCK THREAD CREATED\n");
}

void pthread_join() {
	printf("MOCK THREAD JOINED\n");	
}