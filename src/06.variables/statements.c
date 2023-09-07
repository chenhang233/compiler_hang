#include "function.h"

void statements_print()
{
    AST *ast;
    match_print();
    ast = ast_generator(0);
};
void statements_assign(){};
void statements_variable(){

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