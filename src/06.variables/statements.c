#include "function.h"

void statements_print()
{
    AST *ast;
    match_print();
    ast = ast_generator(0);
    int reg = generator_AST(ast, 0);
    generator_printint(reg);
    generator_free_all_registers();
    match_semi();
};

void statements_variable()
{
    match_int();
    match_ident();
    global_add(TEXT);
    generator_global_symbol(TEXT);
    match_semi();
};

void statements_assign()
{
    AST *l, *r, *root;
    int id;
    match_ident();
    if ((id = global_find(TEXT)) == -1)
    {
        custom_error_int("Undeclared variable", id);
    }
    match_equals();
    r = ast_generator(0);
    AST_V v;
    v.id = id;
    l = mkast_leaf(A_IDENT2, v);
    root = mkast_node(A_ASSIGN, l, r, v);
    generator_AST(root, 0);
    generator_free_all_registers();
    match_semi();
};

void statements()
{
    while (1)
    {
        switch (T_instance.type)
        {
        case T_PRINT:
            statements_print();
            break;
        case T_INT:
            statements_variable();
            break;
        case T_IDENT:
            statements_assign();
            break;
        case T_EOF:
            return;
        default:
            custom_error_int("Syntax error, token", T_instance.type);
            break;
        }
    }
}