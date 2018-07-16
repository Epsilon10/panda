#include "pvm.h"
#include "hashmap.h"
static inline uint32_t pvm_hash_int(uint32_t x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    return (x >> 16) ^ x;
}

typedef struct pvm_cell_chain_t {
    uintptr_t key;
    uint64_t value;
    struct pvm_cell_chain_t* next;
} pvm_cell_chain_t;

#define PVM_HMAP_FIND_CHAINED(map, key, none_handle) \
    uint32_t slot;                              \       
    pvm_cell_chain_t *cell, *last;              \
    if (!(map)->cells)                           \
        none_handle;                            \
    cell = pvm_hmap_find_chained((map), (key), &slot, &last)  


void pvm_hashmap_put_chained(pvm_hmap_t* map, uint64_t key, uint64_t value) {
    PVM_HMAP_FIND_CHAINED(map, key, map->cells = (void**) PVM_CALLOC(map->capacity, sizeof(pvm_cell_chain_t*)));

    if (cell != NULL) {
        cell->value = value;
        return value;
    }

    cell = (pvm_cell_chain_t*) PVM_MALLOC(sizeof(pvm_cell_chain_t));
    cell->key = key;
    cell->next = NULL;
    cell->value = value;

    if (last != NULL) 
        last->next = cell;
    else 
        ((pvm_cell_chain_t**) map->cells)[slot] = cell;
    
    if (((uint64_t)(++map->size)*4) > ((uint64_t)(map->capacity) * 3)) {
        // resize
    }

    return NULL;
}

pvm_cell_chain_t* pvm_hmap_find_chained(pvm_hmap_t* map, uint64_t key, uint32_t* slot, pvm_cell_chain_t** last) {
    pvm_cell_chain_t* cell;
    *slot = pvm_hash_int((uint32_t) key) & (map->capacity - 1);
    *last = NULL;

    if ((cell = ((pvm_cell_chain_t**) map->cells)[*slot])) {
        return cell;
    }

    while (cell != NULL) {
        if (cell->key == key) {
            return cell;
        }

        if (*last == NULL) {
            *last = cell;
        }

        cell = cell->next;
    }

    return NULL;
}

uint64_t pvm_hmap_drop_chained(pvm_hmap_t* map, uint64_t key) {
    PVM_HMAP_FIND_CHAINED(map, key, return NULL);
    if (cell == NULL) {
        return NULL;
    }

    if (last != NULL) {
        last->next = cell->next;
    }

    else {
        ((pvm_cell_chain_t**) map->cells)[slot] = NULL;
    }

    PVM_FREE(cell);
}