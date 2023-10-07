#include "function.h"

Primitive_type parse_type()
{
    Primitive_type type;
    switch (t_instance.token)
    {
    case T_VOID:
        type = P_VOID;
        break;
    case T_CHAR:
        type = P_CHAR;
        break;
    case T_INT:
        type = P_INT;
        break;
    case T_LONG:
        type = P_LONG;
        break;
    default:
        custom_error_int("Illegal type, token", t_instance.token);
    }
    while (1)
    {
        scan(&t_instance);
        if (t_instance.token != T_STAR)
            break;
        type = pointer_to(type);
    }
    return type;
}

void global_declarations()
{
    ASTnode *tree;
    Primitive_type type;
    while (1)
    {
        type = parse_type();
        match_ident();
        if (t_instance.token == T_LPAREN)
        {
            tree = function_declaration(type);
            if (DumpAST)
            {
                // dumpAST
                fprintf(stdout, "\n\n");
            }
            genAST(tree, NOLABEL, 0);
        }
        else
        {
            var_declaration(type);
        }
        if (t_instance.token == T_EOF)
            break;
    }
}

ASTnode *function_declaration(Primitive_type type)
{
}

void var_declaration(Primitive_type type)
{
}

ASTnode *compound_statement()
{
}