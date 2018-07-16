#ifndef _PVM_ATOMIC_H
#define _PVM_ATOMIC_H

#include "pvm.h"
#include <stdatomic.h>

#define PVM_ATOMIC(T) T volatile
#define PVM_ALIGN(s) __attribute__((aligned(s)))

#define PVM_ATOMIC_RELAXED __ATOMIC_RELAXED
#define PVM_ATOMIC_CONSUME __ATOMIC_CONSUME
#define PVM_ATOMIC_ACQUIRE __ATOMIC_ACQUIRE
#define PVM_ATOMIC_RELEASE __ATOMIC_RELEASE
#define PVM_ATOMIC_ACQ_REL __ATOMIC_ACQ_REL
#define PVM_ATOMIC_SEQ_CST __ATOMIC_SEQ_CST

#define pvm_atomic_fence(mo) __atomic_thread_fence(mo)
#define pvm_atomic_load(ptr, mo) __atomic_load_n(ptr, mo)
#define pvm_atomic_add(ptr, val, mo) __atomic_fetch_add(ptr, val, mo)
#define pvm_atomic_sub(ptr, val, mo) __atomic_fetch_sub(ptr, val, mo)
#define pvm_atomic_store(ptr, val, mo) __atomic_store_n(ptr, val, mo)
#define pvm_atomic_xchg(ptr, val, mo) __atomic_exchange_n(ptr, val, mo)

#endif