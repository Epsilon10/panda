#include "actor.h"
#include "opcode"

#define PVM_OP_CASE(op, body) case op: body; break; 
#define PVM_LOCAL(index) ctx->fp[index]
#define PVM_PUSH(value) *ctx->sp++ = value
#define PVM_POP() *(ctx->sp--)
#define PVM_ARG(index) ctx->fp[ctx->num_args - (index) - 3]

bool pvm_actor_interpret(pvm_actor_t* actor, pvm_vm_t* vm) {
    while(true) {

        PVM_OP_CASE(PVM_LOAD_LOCAL, {
            const uint8_t index = *ctx->ip.u8++;
            PVM_PUSH(PVM_LOCAL(index));
        })

        PVM_OP_CASE(PVM_STORE_LOCAL, {
            const uint8_t index = *ctx->ip.u8++;
            PVM_LOCAL(index) = PVM_POP();
        })

        PVM_OP_CASE(PVM_LOAD_ARG, {
            const uint8_t index = *ctx->ip.u8++;
            PVM_PUSH(ARG(index));
        })

        PVM_OP_CASE(PVM_STORE_ARG, {
            const uint8_t index = *ctx->ip.u8++;
            PVM_ARG(index) = PVM_POP();
        })

        PVM_OP_CASE(PVM_LOAD_FUNC, {
            const uint8_t func_table_index = *ctx->ip.u8++;  // get bytecode number (index)
            const pvm_func_t* func = &vm->program->funcs[func_table_index]; // index bytecode ... for example LOAD_FUNC PVM_SEND where pvm send is some 
            PVM_PUSH(PVM_VALUE(PVM_TYPE_FUNC, (uintptr_t) func));
        })

        PVM_OP_CASE(PVM_SPAWN_PROC, {
            const uint8_t func_args = *ctx->ip.u8++;
            ctx->num_args = func_args;
            pvm_func_t* func = PVM_PTR(pvm_func_t*, ctx->sp[-(ctx->num_args) - 1]);
            pvm_value* args = &ctx->sp[-(ctx->num_args)];
            pvm_actor_t* actor = pvm_actor_spawn(func, ctx->num_args, args);
            PVM_PUSH(PVM_VALUE(PVM_TYPE_ACTOR, (uintptr_t) actor));
        })

        PVM_OP_CASE(PVM_CALL, {
            const uint8_t func_args = *ctx->ip.u8++;
            PVM_PUSH(ctx->fp);
            PVM_PUSH(ctx->num_args);
            PVM_PUSH(ctx->ip);

            ctx->num_args = func_args;
            pvm_func_t* func = PVM_PTR(pvm_func_t*, ctx->sp[-(ctx->num_args) - 1]);

            for (int i = 0; i < func->num_locals, i++) {
                PVM_PUSH(NULL);
            }

            ctx->ip = func->ip; // perform jump to functions bytecode
        })

        PVM_OP_CASE(PVM_CONST_I32, {
            const uint32_t value = *ctx->ip.u32++;
            PVM_PUSH(value);
        }

        PVM_OP_CASE(PVM_ADD_I32, {
            ctx->ip.u8++;
            uint32_t* a = PVM_PTR(uint32_t*, PVM_POP();
            uint32_t* b = PVM_POP(uint32_t*, PVM_POP());
            uint32_t* result;
            *result = *a + *b;
            PVM_PUSH(PVM_VALUE(PVM_TYPE_INT, (uintptr_t) data))
        })

        

        PVM_OP_CASE(PVM_SEND, {
            ctx->ip.u8++;
            pvm_value addr = PVM_POP();
            pvm_value value = PVM_POP();
            pvm_actor_t* actor = PVM_PTR(pvm_actor_t*, addr);
            pvm_actor_send(actor);

        })

        PVM_OP_CASE(PVM_RECV, {
            ctx->ip.u8++;
            pvm_value value = pvm_actor_recv(actor);
            if (!pvm_is_null(value)) {
                PVM_PUSH(value);
            }

        })



        


    }
}