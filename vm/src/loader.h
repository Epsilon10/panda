typedef union {
    uint8_t*  u8;
    uint16_t* u16;
    uint32_t* u32;
    double*   f64;
    uintptr_t* uptr;
} pvm_parser_t;

typedef struct {
    pvm_vm_t* vm;
    pvm_value* consts;
    pvm_func_t* funcs;
    uint8_t num_of_funcs;
} pvm_program_t;
