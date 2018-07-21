#include "scheduler.h"

pvm_mpsc_node_t* pvm_mpscq_node(pvm_actor_t* actor) {
    pvm_mpscq_node_t* node = (pvm_mpscq_node_t*) PVM_MALLOC(sizeof(pvm_mpscq_node_t));
    node->next = NULL;
    node->actor = actor;
    return node;
}

pvm_mpsc_node_t* pvm_mpscq_pop(pvm_mspc_queue_t* queue) {
    pvm_mpscq_node_t* tail = queue->tail;
    pvm_mpscq_node_t* next = tail->next;

    if (next != NULL) {
        queue->tail = next;
        tail->actor = next->actor;
        return tail;
    }
    queue->count = pvm_atomic_sub_explicit(&queue->count, 1, PVM_ATOMIC_ACQ_REL);
    return NULL;
}

void pvm_mpscq_push(pvm_mpsc_queue_t* queue, pvm_mpscq_node_t* node) {
    node->next = NULL;
    pvm_mpscq_node_t* prev = pvm_atomic_xchg(&queue->head, node, PVM_ATOMIC_RELAXED);
    prev->next = node;
    queue->count = pvm_atomic_add_explicit(&queue->count, 1, PVM_ATOMIC_ACQUIRE);
}



