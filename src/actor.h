#ifndef _PVM_ACTOR_H
#define _PVM_ACTOR_H


#include "atomic.h"
#include "gc.h"

typedef struct pvm_msgq_node_t pvm_msgq_node_t;

typedef struct {
    PVM_ATOMIC(pvm_msgq_node_t*) head;
    PVM_ATOMIC(pvm_msgq_node_t*) tail;
} pvm_msgq_t;



struct pvm_actor_t {
    pvm_gc_t gc;
    pvm_ctx_t* ctx;
    uint32_t refcount;
    uint8_t is_active;
    pvm_msgq_t mailbox;
};

#endif