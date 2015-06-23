#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib_pthread.h"



int pthread_attr_init(pthread_attr_t *attr) {
	printf("ATTR_INIT\n");
}

int pthread_mutex_init(
		pthread_mutex_t * mutex,
		const pthread_mutexattr_t * attr) {
	printf("MUTEX_INIT\n");
}

int pthread_attr_setdetachstate(
		pthread_attr_t *attr,
		int detachstate) {
	printf("ATTR_DETACH_STATE\n");
}

int pthread_create(pthread_t *thread,
		const pthread_attr_t *attr,
    	void *(*start_routine)(void*),
    	void *arg) {
	printf("MOCK THREAD CREATED\n");
}

int pthread_join(pthread_t thread, void **value_ptr) {
	printf("MOCK THREAD JOINED\n");	
}