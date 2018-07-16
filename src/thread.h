#ifndef _PVM_THREAD_H
#define _PVM_THREAD_H

#include "pvm.h"
#include "alloc.h"

#include <sched.h>
#include <pthread.h>
#include <sys/sysinfo.h>

typedef pthread_t pvm_thread_t;
typedef pthread_mutex_t pvm_mutex_t;
typedef pthread_cond_t pvm_cond_t;

#define pvm_thread_yield sched_yield



#endif