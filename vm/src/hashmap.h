#ifndef _PVM_HASHMAP_H
#define _PVM_HASHMAP_H

typedef struct {
    void** cells;
    uint32_t size;
    uint32_t capacity;
    bool is_chained;
} pvm_hmap_t;

#endif