#include "vm.h"

typedef struct {
    int line_no;
    int current;
    const char* filename;
    const char* code;
} pvm_lexer_t;

void pvm_lexer_init(const char* filename, const char* code);