#include "scheduler.h"

void pvm_scheduler_init(pvm_vm_t* vm, pvm_schedulers_t* schedulers, size_t amount) {
    pvm_sched_t* sched;
    schedulers->num_of_schedulers = pvm_num_of_cores();
    schedulers->worker_threads = (pvm_sched_t*) PVM_MALLOC(sizeof(pvm_sched_t) * schedulers->num_of_schedulers);    
}


void pvm_scheduler_submit(pvm_actor_t* actor) {
    pvm_vm_t* vm = PVM_VM_THIS(NULL); // TODO: implement
    pvm_sched_t* scheduler = (pvm_sched_t*) pvm_mpmcq_pop(&vm->idle_schedulers);
    
    if (scheduler == NULL) {
        pvm_mpmcq_push(&vm->rt_queue, (void*) actor);
    }

    else {
        scheduler->actor = actor;
    }

}

void* pvm_sched_loop(pvm_sched_t* scheduler) {
    pvm_mpsc_queue_t* rt_queue = &schedulder->rt_queue;
    pvm_vm_t* vm = scheduler->vm;
    size_t amount = vm->schedulers.amount;
    const pvm_sched_t* threads = vm->schedulers.worker_threads;

    while ((scheduler->actor)->is_active == 1) {

        if (schudeler->actor != NULL) {
            actor_interp(scheduler->actor);
        } 
        if ((scheduler->actor = (pvm_actor_t*) pvm_mpscq_pop(&scheduler->rt_queue)) != NULL) {
            continue;
        }

        scheduler->actor = pvm_sched_work_steal(threads, amount);

        if (scheduler->actor !=NULL) {
            continue;
        }

        pvm_atomic_store(&scheduler->is_active, 0, PVM_ATOMIC_RELAXED);
        pvm_mpmcq_push(&vm->idle_schedulers, (void*) scheduler);

        if (scheduler->actor == NULL) {
            pvm_cond_wait(&scheduler->waiter, &scheduler->mutex);
        }

        pvm_mutex_unlock(&scheduler->mutex);
          
    }
    // Free some stuff
    // exit thread

    return NULL;

}




void pvm_sched_init(pvm_vm_t* vm, pvm_schedulers_t* schedulers, size_t num_of_schedulers) {
    pvm_sched_t* scheduler;
    pvm_thread_t thread_id;

    schedulers->num_of_schedulers = amount = pvm_cores();
    schedulers->worker_threads = (pvm_sched_t*) PVM_MALLOC(sizeof(pvm_sched_t) * amount);

    while (amount--) {
        scheduler = &((pvm_sched_t*) schedulers->worker_threads)[amount];
        scheduler->running = false;
        scheduler->actor = NULL;
        scheduler->vm = vm;
        scheduler->rt_queue.head = scheduler->rt_queue.tail = pvm_mpscq_node(NULL);

        pvm_condition_init(&scheduler->waiter);
        pvm_mutex_init(&scheduler->mutex);

        scheduler->thread_id = pvm_thread_create(&thread_id, NULL, pvm_sched_loop, scheduler);
        // init some other stuff
    }
}

void pvm_sched_submit(pvm_actor_t* actor) {
    pvm_vm_t* vm = PVM_VM_THIS();
    pvm_sched_t* scheduler = (pvm_scheduler_t*) pvm_mpmcq_pop(&vm->idle_schedulers);
    pvm_schedulers_t* scheds = &vm->schedulers;
    if (scheduler != NULL) {
        scheduler->actor = actor;
        // do stuff
    }

    else {
        size_t amount = vm->schedulers.amount;
        const pvm_sched_t* threads = vm->schedulers.worker_threads;
        pvm_sched_t* scheduler = threads;
        while (amount--) {
            if (pvm_atomic_load(&scheduler->rt_queue.count, PVM_ATOMIC_RELAXED) < pvm_atomic_load(&threads[amount]->rt_queue.count, PVM_ATOMIC_RELAXED)) {
                scheduler = amount + threads;
            }
        }

        pvm_mpscq_push(&scheduler->rt_queue, pvm_mpscq_node(actor));
}

pvm_actor_t* pvm_sched_work_steal(pvm_sched_t* threads, size_t amount) {
    pvm_sched_t* scheduler = threads;
    while (amount--) {
        if (pvm_atomic_load(&scheduler->rt_queue.count, PVM_ATOMIC_RELAXED) > pvm_atomic_load(&threads[amount]->rt_queue.count, PVM_ATOMIC_RELAXED)) {
            scheduler = amount + threads;
        }
    }
    
    if (pvm_atomic_load(&scheduler->rt_queue.count, PVM_ATOMIC_RELAXED) > 0) {
        return pvm_mpscq_pop(&scheduler->rt_queue);
    }

    return NULL;
}

void pvm_sched_free(pvm_schedulers_t* schedulers) {
    // TODOs
}