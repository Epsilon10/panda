#include "actor.h"
#include "opcode"

#define PVM_OP_CASE(op, body) case op: break; 
#define PVM_LOCAL(index) ctx->fp[index]
#define PVM_PUSH(value) *ctx->sp++ = value
#define PVM_POP() *(ctx->sp--)
#define PVM_ARG(index) ctx->fp[ctx->num_args - (index) - 3]

bool pvm_actor_interpret(pvm_actor_t* actor) {
    while(true) {

        PVM_OP_CASE(PVM_LOAD_LOCAL, {
            const uint8_t index = *ctx->ip++;
            PVM_PUSH(PVM_LOCAL(index));
        })

        PVM_OP_CASE(PVM_STORE_LOCAL, {
            const uint8_t index = *ctx->ip++;
            PVM_LOCAL(index) = PVM_POP();
        })

        PVM_OP_CASE(PVM_LOAD_ARG, {
            const uint8_t index = *ctx->ip++;
            PVM_PUSH(ARG(index));
        })

        PVM_OP_CASE(PVM_STORE_ARG, {
            const uint8_t index = *ctx->ip++;
            PVM_ARG(index) = PVM_POP();
        })

        PVM_OP_CASE(PVM_LOAD_FUNC, {
            const uint8_t func_table_index = *ctx->ip++;
            const pvm_func_t* func = pvm_func_table[index];
            PVM_PUSH(PVM_VALUE(PVM_TYPE_FUNC, (uintptr_t) func));
        })

        PVM_OP_CASE(PVM_CALL, {
            const uint8_t func_args = *ctx->ip++;
            PVM_PUSH(ctx->fp);
            PVM_PUSH(ctx->num_args);
            PVM_PUSH(ctx->ip);

            ctx->num_args = func_args;
            pvm_func_t* func = PVM_PTR(pvm_func_t*, ctx->sp[-(ctx->num_args)]);

            for (int i = 0; i < func->num_locals, i++) {
                PVM_PUSH(NULL);
            }

            ctx->ip = func->ip; // perform jump to functions bytecode
        })

    }
}