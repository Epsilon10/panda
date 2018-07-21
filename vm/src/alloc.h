#ifndef _PVM_ALLOC_H
#define _PVM_ALLOC_H

#include <stddef.h>

#define PVM_MALLOC_ATTR  __attribute__((__malloc__))
void rpmalloc_initialize();
void rpmalloc_thread_collect();
void rpmalloc_thread_finalize();
void rpmalloc_thread_initialize();
void rpfree(void* ptr);
void* rprealloc(void* ptr, size_t size);
void* rpmalloc(size_t size) PVM_MALLOC_ATTR;
void* rpcalloc(size_t n, size_t size) PVM_MALLOC_ATTR;

#define PVM_FREE rpfree
#define PVM_MALLOC rpmalloc
#define PVM_CALLOC rpcalloc


 

#define PVM_LIKELY(expr) __builtin_expect(!!(expr), 1)
#define PVM_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#endif