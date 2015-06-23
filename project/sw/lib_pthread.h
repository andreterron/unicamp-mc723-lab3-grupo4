
#define PTHREAD_CREATE_JOINABLE 1

typedef int pthread_t;
typedef void* pthread_attr_t;
typedef int pthread_mutex_t;
typedef void* pthread_mutexattr_t;


int pthread_attr_init(pthread_attr_t *attr);

int pthread_mutex_init(
		pthread_mutex_t * mutex,
		const pthread_mutexattr_t * attr);

int pthread_attr_setdetachstate(
		pthread_attr_t *attr,
		int detachstate);

int pthread_create(
		pthread_t *thread,
		const pthread_attr_t *attr,
    	void *(*start_routine)(void*),
    	void *arg);

int pthread_join(pthread_t thread, void **value_ptr);