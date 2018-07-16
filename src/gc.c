#include "alloc.h"
#include "gc.h"
#include <string.h>
#include "actor.h"

#define PVM_SLOT_SIZE sizeof(pvm_slot_t)
#define PVM_GC_BMAP_ROW_SIZE sizeof(uint64_t)
#define PVM_GC_ROW_INDEX(map, slot) (map)[(slot) / PVM_GC_BMAP_ROW_SIZE] // divide by 64 to scan each slot
#define PVM_GC_SLOT_BIT(slot) (1ULL << ((slot) & (PVM_GC_BMAP_ROW_SIZE - 1)))
#define PVM_GC_IS_SET(map,slot) (PVM_GC_ROW_INDEX(map, slot) & PVM_GC_SLOT_BIT(slot))
#define PVM_GC_MARK_BIT(map, slot) (PVM_GC_ROW_INDEX(map, slot) ^= PVM_GC_SLOT_BIT(slot))

inline pvm_slot_t pvm_gc_slot_alloc(pvm_gc_t* gc) {
    uint64_t* bitmap;
    while (true) {
        bitmap = gc->bitmap;
        if (*bitmap == UINT64_MAX) {
            bitmap++;
            continue;
        }
    }
    int pos = ffsl(~*bitmap) - 1;
    *bitmap |= 1ULL << pos;
    return gc->last_row * PVM_GC_BMAP_ROW_SIZE + pos;
}

bool pvm_gc_mark(pvm_gc_t* gc, pvm_value obj) {
    pvm_slot_t slot;
    if (pvm_is_shared(obj)) {
        slot = (pvm_hmap_t) pvm_hmap_get_chained(gc->shared, PVM_PTR(uint64_t*, obj), slot);
    }
    else {
        slot = *(PVM_PTR(uint8_t*, obj) - sizeof(pvm_slot_t)); // ptr to the start of slot
    }

    if (PVM_GC_SLOT_SET(map, slot)) {
        return false;
    }
    PVM_GC_MARK_SLOT_BIT(map, slot);
    PVM_GC_TRAVERSE(obj, pvm_gc_mark(gc, obj))
    return true;
}


#define PVM_GC_TRAVERSE(obj, handle) 
    uint8_t type;                                       /
    pvm_iter_t iter;                                    /
    pvm_value value;                                    /
    switch(type = PVM_TYPE(obj)) {                      /
        case PVM_TYPE_ARRAY:                            /
            pvm_array(obj, &iter);                      /
            for (uint8_t i = 0; i < iter.size; ++i) {   /
                value = iter.data[i];                   /
                handle;                                 /
            }                                           /
        case PVM_TYPE_MAP:

    } 



void pvm_gc_share(pvm_actor_t* actor, pvm_value value) {
    if (pvm_is_shared(value)) {
        uint32_t* refc_ptr = PVM_PTR(uint8_t*, value) - sizeof(uint32_t);
        *refc_ptr = *refc_ptr + 1;
        return;
    }
    PVM_TOGGLE_SIGNAL_BIT(value);
    uint32_t* refc_ptr = PVM_PTR(uint8_t*, value) - sizeof(uint32_t);
    *refc_ptr = 1;
    PVM_GC_TRAVERSE(value, pvm_gc_share(value, pvm_gc_share(actor, value)));

}


inline bool pvm_gc_mark_roots(pvm_actor_t* actor, pvm_gc_t* gc) {
   pvm_value* sp = &actor->ctx_t.sp;
   pvm_value* addr = ctx->stack;
   for(; addr < sp; addr++) {
       if (pvm_gc_mark(gc, *addr) && !marked) {
           marked = true;
       }
   }

   return marked;
}

pvm_value pvm_gc_alloc(pvm_actor_t* actor, uint8_t type, size_t bytes) {
    pvm_gc_t* gc = actor->gc;
    pvm_slot_t slot = pvm_gc_slot_alloc(gc);
    uint8_t* slot_ptr = (uint8_t*) PVM_MALLOC(PVM_SLOT_SIZE + bytes); // get ptr to allocated memory
    void* data_ptr = mem_ptr + sizeof(pvm_slot_t);
    return gc->heap[(*slot_ptr = slot)] = PVM_VALUE(type, (slot_ptr + 1)); // add one to make this next gc index
}

bool pvm_gc_alloc_shared(pvm_actor_t* actor, pvm_value value) {
    pvm_gc_t* gc = &actor->gc;
    pvm_slot_t* slot = pvm_gc_slot_alloc(gc);

    pvm_hmap_put_chained(&gc->shared, PVM_PTR(uint64_t, value), (uint64_t) slot);
}

bool pvm_gc_sweep(pvm_gc_t* gc, bool* had_survivors) {
    int bitpos;
    pvm_value to_free;
    uint8_t* payload_ptr;
    uint64_t unreachables;
    bool is_full = true;
    *had_survivors = false;

    for (uint32_t row = 0; row < gc->bmap_size; row++) {
        unreachables = gc->bitmap[row] & ~gc->shared[row];
        gc->bitmap ^= unreachables;

        if (is_full) {
            // figure out if bmap is full
        }
        if (!(*had_survivors)) {
            *had_survivors = true;
        }
        while (unreachables != 0) {
            bitpos = ffsll(unreachables);
            unreachables ^= 1ULL << bitpos;
            to_free = gc->heap[(row * PVM_GC_BMAP_ROW_SIZE) + bitpos];
            payload_ptr = PVM_PAYLOAD_PTR(to_free);
            if (PVM_TYPE(to_free) == PVM_HMAP) {
                pvm_hmap_free(to_free);
            } 
            else {
                if (!pvm_is_actor(to_free)) {
                    PVM_FREE(payload_ptr)
                }
            }
        }
    }
}

bool pvm_gc_collect(pvm_actor_t* actor) {
    pvm_gc_t* gc = &actor->gc;
    bool had_survivors;

    if (pvm_gc_mark_roots(actor, gc)) {
        if (pvm_gc_sweep(gc, &had_survivors)) {
            return true;
        }

        if (had_survivors)
            return false;
    }
    return false;
}