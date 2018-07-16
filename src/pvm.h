#ifndef _PVM_H
#define _PVM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint32_t pvm_slot_t;
typedef double pvm_value;
typedef struct { uint32_t size; pvm_value* data } pvm_string_t;


typedef struct {
    pvm_value* fp;
    pvm_value* sp;
    pvm_value* stack;
    size_t stack_size;
    pvm_parser_t* ip; // points to current bytecode
    uint8_t num_args; // num of args in current frame
} pvm_ctx_t;

typedef struct {
    uint8_t* code;
    pvm_string_t name;
    uint8_t num_locals;
} pvm_func_t;


#define PVM_TYPE_INT = 0
#define PVM_TYPE_MAP = 1
#define PVM_TYPE_FUNC = 2
#define PVM_TYPE_ACTOR = 3
#define PVM_TYPE_DATA = 4
#define PVM_TYPE_ARRAY = 5
#define NK_TYPE_TUPLE  6
#define NK_TYPE_STRING = 7


#define PVM_SHIFT 48
#define PVM_NAN 0xfff8ULL
#define PVM_FROM(v) (*((pvm_value*) &(v)))
#define PVM_INT (v) (*(int32_t) &(v))
#define PVM_UPTR(v) (*((uintptr_t*) &(v)))
#define PVM_TYPE ((PVM_UPTR(v) >> 48) & 0b111)
#define PVM_PTR(type, v) ((type)((PVM_UPTR(v) & 0xfffffffffffULL))
#define PVM_SLOT_PTR(v) ((PVM_PTR(uint8_t*, v)) - sizeof(pvm_slot_t)))   // we have for example gc slot at 0x04, then obj ptr is at 0x08. to get gc slot ptr we subtract off 4 bytes. 
/*

*/
#define PVM_SIGNAL_BIT(v) (v & (1ULL << 51))
#define PVM_TOGGLE_SIGNAL_BIT(v) (v ^= 1ULL << 51)

static inline pvm_value PVM_VALUE(int type, uintptr_t data) {
    uint64_t value = ((PVM_NAN | type) << PVM_SHIFT ) | data;
    return PVM_FROM(value);
} // Nan box it

#define PVM_PAYLOAD_PTR(v) (PVM_PTR(uint8_t*, v) - sizeof(pvm_slot_t) - sizeof(uint8_t));
#define pvm_is_int(v) (PVM_TYPE(v) == PVM_TYPE_INT);
#define pvm_is_ptr(v) (!(pvm_is_int(val)))
#define pvm_is_shared(v) (pvm_is_ptr(v) && PVM_SIGNAL_BIT(v))



#endif