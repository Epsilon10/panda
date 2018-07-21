#ifndef _PVM_SCHEDULER_H
#define _PVM_SCHEDULER_H

#include "actor.h"



typedef struct {
    pvm_vm_t* vm;
    pvm_thread_t thread_id;
    pvm_cond_t waiter; // will wait for thread to be notified to process
    pvm_actor_t* actor;
    pvm_rt_queue_t* rt_queue;
    bool running;
} pvm_sched_t;

typedef struct pvm_mpsc_queue_t {
    PVM_ATOMIC(pvm_rtq_node_t*) head;
    PVM_ATOMIC(pvm_rtq_node_t*) tail;
    PVM_ATOMIC(size_t) count;
}

typedef struct pvm_mpscq_node_t {
    pvm_actor_t* actor;
    struct pvm_mpscq_node_t* next;
}

typedef struct {
    size_t num_of_schedulers;
    pvm_sched_t* worker_threads;
} pvm_schedulers_t;







#endif