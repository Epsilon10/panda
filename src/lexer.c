#include "lexer.h"

static const char* operator_chars = "+-*/%.:=<>|&^"


pvm_lexer_t* pvm_lexer_init(char* filename, char* code) {
    pvm_lexer_t* lexer;
    lexer->filename = filename;
    lexer->line_no = 1;
    lexer->current = 0;
    lexer->code = code;
    return lexer;
}


static inline is_operator(char c) {
    return strchr(operator_chars, c);
}

static inline bool is_digit(char c) {
    return (c >= '0' && c <= 9);
}

static inline bool is_numeric(char c) {
    return isdigit(c);
}

static inline bool is_whitespace(char c) {
    switch(c) {
        case '\t': case ' ': case '\r': return true;
        default: return false;
    }
}

#define current_char(lexer)
    ((lexer).code[(lexer).current])

