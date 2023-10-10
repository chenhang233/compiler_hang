#include "function.h"

int inttype(Primitive_type type)
{
    if (type == P_CHAR || type == P_INT || type == P_LONG)
        return (1);
    return (0);
}

Primitive_type pointer_to(Primitive_type type)
{
    Primitive_type newtype;
    switch (type)
    {
    case P_VOID:
        newtype = P_VOIDPTR;
        break;
    case P_CHAR:
        newtype = P_CHARPTR;
        break;
    case P_INT:
        newtype = P_INTPTR;
        break;
    case P_LONG:
        newtype = P_LONGPTR;
        break;
    default:
        custom_error_int("Unrecognised in pointer_to: type", type);
    }
    return newtype;
}

ASTnode *modify_type(ASTnode *tree, Primitive_type rtype, AST_node_type op)
{
    int ltype;
    int lsize, rsize;

    ltype = tree->type;
    if (inttype(ltype) && inttype(rtype))
    {
        if (ltype == rtype)
            return tree;
        lsize = genprimsize(ltype);
        rsize = genprimsize(rtype);
        if (lsize > rsize)
            return NULL;
        if (rsize > lsize)
            return mkAST_left(A_WIDEN, rtype, tree, 0);
    }

    return NULL;
}