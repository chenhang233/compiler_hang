#include "function.h"
// Generic code generator

int generator_AST(AST *ast, int reg)
{
    int reg1, reg2;
    if (ast->l)
        reg1 = generator_AST(ast->l, 0);
    if (ast->r)
        reg2 = generator_AST(ast->r, reg1);
    switch (ast->op)
    {
    case A_INTLIT:
        return code_generator_load_int(ast->v.int_v);
    case A_ADD:
        return code_generator_add(reg1, reg2);
    case A_SUB:
        return code_generator_sub(reg1, reg2);
    case A_MUL:
        return code_generator_mul(reg1, reg2);
    case A_DIV:
        return code_generator_div(reg1, reg2);
    case A_IDENT:
        return code_generator_load_global(Global_symbol_map[ast->v.id].name);
    case A_IDENT2:
        return code_generator_store_register(reg, Global_symbol_map[ast->v.id].name);
    case A_ASSIGN:
        return reg2;
    default:
        custom_error_int("Unknown AST operator", ast->op);
    }
}

void generator_preamble()
{
    code_generator_preamble();
}

void generator_postamble()
{
    code_generator_postamble();
}

int generator_load_int(int v)
{
    return code_generator_load_int(v);
};
int generator_load_global(char *identifier)
{
    return code_generator_load_global(identifier);
};
int generator_global_symbol(char *symbol)
{
    return code_generator_global_symbol(symbol);
};
int generator_store_register(int r, char *identifier)
{
    return code_generator_store_register(r, identifier);
};
int generator_add(int r1, int r2)
{
    return code_generator_add(r1, r2);
};
int generator_sub(int r1, int r2)
{
    return code_generator_sub(r1, r2);
};
int generator_mul(int r1, int r2)
{
    return code_generator_mul(r1, r2);
};
int generator_div(int r1, int r2)
{
    return code_generator_div(r1, r2);
};
int generator_printint(int r)
{
    return code_generator_printint(r);
};

void generator_free_all_registers()
{
    return free_all_registers();
}