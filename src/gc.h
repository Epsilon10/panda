#ifndef _PVM_GC_H
#define _PVM_GC_H

#include "pvm.h"
#include "map.h"
typedef struct {
    pvm_value* heap; // store all the NaN values. Nan values use first 16 bits to annotate the type and whatnot 
    uint64_t* bitmap;
    uint64_t* refmap;
    uint32_t* bmap_size;
    pvm_imap_t shared;
} pvm_gc_t;
#endif