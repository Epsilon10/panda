#include "vm.h"

#define PVM_CONST_8 = 0
#define PVM_CONST_16 = 1
#define PVM_CONST_32 = 2
#define PVM_CONST_64 = 3
#define PVM_CONST_SIZE(s) ((s) >> 3)
#define PVM_CONST_TYPE(t) ((t) & 0b111)



int load_program(pvm_vm_t* vm, const* char bytecode) {
    pvm_func_t* func;
    pvm_program_t* program;
    pvm_parser_t pos = { .u8 = (uint8_t* bytecode )};
    uint64_t value;
    program = (pvm_program_t*) PVM_MALLOC(sizeof(pvm_program_t));
    program->vm = vm;

    uint8_t const_type;
    program->consts = (pvm_value*) PVM_CALLOC(sizeof(pvm_value), program->num_of_consts);

    for (size_t i = 0; i < program->number_of_consts; i++) {
        value = (PVM_NAN | PVM_CONST_INT) << PVM_SHIFT;
        switch (PVM_CONST_TYPE(const_type = *pos.u8++) {
            case PVM_CONST_INT:
                switch(PVM_SIZE(const_type)) {
                    case PVM_CONST_8: value |= *pos.u8++; break;
                    case PVM_CONST_16: value |= *pos.u16++; break;
                    case PVM_CONST_32: value |= *pos.u32++; break;
                    default: // TODO: 
                }

                program->consts[i] = PVM_FROM(value);
            case PVM_CONST_STRING:
                switch (PVM_SIZE(const_type)) { // todo: implement size thingy 
                    case EIGHT_BITS: value = *pos.u8++; break;
                    case SIXTEEN_BITS: value = *pos.u16++;  break;      
                    case THIRTY_TWO_BITS: value *pos.u32++; break;
                }
                pvm_slot_t* ptr = (pym_slot_t*) PVM_MALLOC(sizeof(pvm_slot_t) + value);
                memcpy(&ptr[1], (const void*) pos.u8, sizeof(value));
                pos.u8 += (int) sizeof(value)
                program->consts[i] = PVM_VALUE(PVM_CONST_STRING, (uintptr_t) data));
            
            default: break;

        }
    }

    uint8_t num_of_funcs = program->num_of_funcs = *pos.u8++;

    for (uint8_t opcode, i = 0; i < num_of_funcs; i++) {
        func = (pvm_func_t*) PVM_MALLOC(sizeof(pvm_func_t));

        while ((opcode = *pos.u8++)) {
            switch (opcode) {
                
            }
        }
    }

}   

