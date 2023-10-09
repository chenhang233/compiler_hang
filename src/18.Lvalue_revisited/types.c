#include "function.h"

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
}