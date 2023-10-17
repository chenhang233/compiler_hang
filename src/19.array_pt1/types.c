#include "function.h"

int inttype(Primitive_type type)
{
    if (type == P_CHAR || type == P_INT || type == P_LONG)
        return (1);
    return (0);
}

int ptrtype(Primitive_type type)
{
    if (type == P_VOIDPTR || type == P_CHARPTR ||
        type == P_INTPTR || type == P_LONGPTR)
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

Primitive_type value_at(Primitive_type type)
{
    int newtype;
    switch (type)
    {
    case P_VOIDPTR:
        newtype = P_VOID;
        break;
    case P_CHARPTR:
        newtype = P_CHAR;
        break;
    case P_INTPTR:
        newtype = P_INT;
        break;
    case P_LONGPTR:
        newtype = P_LONG;
        break;
    default:
        custom_error_int("Unrecognised in value_at: type", type);
    }
    return (newtype);
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
    if (ptrtype(ltype))
    {
        if (op == 0 && rtype == ltype)
            return tree;
    }
    if (op == A_ADD || op == A_SUBTRACT)
    {
        if (inttype(ltype) && ptrtype(rtype))
        {
            int rsize = genprimsize(value_at(rtype));
            if (rsize > 1)
                return mkAST_left(A_SCALE, rtype, tree, rsize);
        }
    }
    return NULL;
}