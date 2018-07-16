#include "pvm.h"
#include "actor.h"


struct pvm_msgq_node_t {
    pvm_value* value;
    PVM_ATOMIC(pvm_msgq_node_t*) next;
}

static inline pvm_msgq_node_t* pvm_msgq_node(pvm_value value) {
    pvm_msgq_node_t* node = (pvm_msgq_node_t*) PVM_MALLOC(sizeof(pvm_msgq_node_t));
    node->next = NULL;
    node->value = value;
    return node;
}

pvm_actor_t* pvm_actor_spawn(pvm_func_t* func, uint8_t argc, pvm_value* args) {
    pvm_actor_t* actor = (pvm_actor_t*) PVM_MALLOC(sizeof(pvm_actor_t));
    pvm_ctx_t* ctx = &actor->ctx;
    actor->is_active = 0;
    actor->refc = 1;
    actor->mailbox.head = actor->mailbox.tail = pvm_msgq_node_t(NULL);

    pvm_gc_init(actor);
    ctx->ip.u8 = func->code;
    ctx->stack_size = func->num_of_locals + argc;
    ctx->stack = (pvm_value*) PVM_MALLOC(sizeof(pvm_value) * ctx->stack_size);
    ctx->sp = (frame->fp = ctx->stack[argc])[func->num_of_locals];

    memcpy((void*) ctx->stack, (const void*) args, (size_t) argc);
    pvm_sched_submit(actor);
    return actor;

}

void pvm_actor_send(pvm_actor_t* dest_actor, pvm_value value) {
    pvm_value send_val = 
    pvm_msgq_node_t* node = pvm_msgq_node(value);
    pvm_msgq_node_t* prev = pvm_atomic_xchg(&dest_actor->mailbox.head, node, PVM_ATOMIC_ACQ_REL);
    pvm_atomic_store(&prev->next, node, PVM_ATOMIC_RELEASE);

    if (pvm_atomic_comp_xchg_weak(&actor->active, 0, 1, PVM_ATOMIC_RELAXED, PVM_ATOMIC_RELAXED)) {
        pvm_sched_submit(actor);
    }
}

nk_value pvm_actor_recv(pvm_actor_t* actor) {

}

