#include "thread.h"
#include "scheduler.h"

static pvm_mpmc_qnode_t* pvm_mpmcq_node_alloc(void* data) {
    pvm_mpmc_qnode_t* node = (pvm_mpmc_qnode_t*) PVM_MALLOC(sizeof(pvm_mpmc_qnode_t));
    node->next = NULL;
    node->data = data;
    return node;
}

void pvm_mpmcq_free(pvm_mpmc_queue_t* queue) {
    pvm_atomic_store(&queue->head, NULL, PVM_ATOMIC_RELAXED);
    PVM_FREE((void*) queue->tail.node);
}

void pvm_mpmcq_push(pvm_mpmc_queue_t* queue, void* data) {
    pvm_mpmc_qnode_t* node = pvm_mpmcq_node_alloc(data);
    pvm_atomic_fence(PVM_ATOMIC_RELEASE);
    pvm_mpmc_qnode_t* prev = pvm_atomic_exchange(&queue->head, node, PVM_ATOMIC_RELAXED);
    pvm_atomic_store(&prev->next, node, PVM_ATOMIC_RELAXED);
}

void pvm_mpmcq_init(pvm_mpmc_queue_t* queue) {
    queue->head = nk_mpmc_node_alloc(NULL);
    queue->tail.node = queue->head;
    queue->tail.count = 0;
}
void* pvm_mpmcq_pop(pvm_mpmc_queue_t* queue) {
    pvm_mpmc_qnode_aba_t cmp, xchg; // compare and exchange
    pvm_mpmc_qnode_t *tail, *next;

    cmp.node = queue->tail.node;
    cmp.count = queue->tail.count;

    do {
        tail = cmp.node;
        next = pvm_atomic_load(&tail->next, PVM_ATOMIC_RELAXED);
        if (next == NULL) {
            return NULL;
        }

        xchg.node = next;
        xchg.count = cmp.count + 1;
    } while (!pvm_atomic_comp_xchg_weak(&queue->tail, &cmp, xchg,PVM_ATOMIC_RELAXED, PVM_ATOMIC_RELAXED));

    pvm_atomic_fence(PVM_ATOMIC_ACQUIRE);
    void* data = pvm_atomic_load(&next->data, PVM_ATOMIC_RELAXED);
    // since we are freeing the old tail, we need to make sure no other thread is reading the current tail. 
    // first, set the new tail to NULL. Wait until the data pointer of the other tail is null. 
    // We synchronised on next->data to make sure all memory writes we've done
  // will be visible from the thread that will free our tail when it starts
  // freeing it.

    pvm_atomic_store(&next->data, NULL, PVM_ATMOIC_RELAXED);
    while (pvm_atomic_load(&tail->data, PVM_ATOMIC_RELAXED)) {
        // yeild thread
    }

    pvm_atomic_fence(PVM_ATOMIC_ACQUIRE);
    PVM_FREE((void*) tail);
    return data;



}



