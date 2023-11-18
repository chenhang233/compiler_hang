#include "function.h"

int inttype(Primitive_type type)
{
    return ((type & 0xf) == 0);
}

int ptrtype(Primitive_type type)
{
    return ((type & 0xf) != 0);
}

Primitive_type pointer_to(Primitive_type type)
{ // 0000 0000   位值  64 48 32 16 8 4 2 1
    if ((type & 0xf) == 0xf)
        custom_error_int("Unrecognised in pointer_to: type", type);
    return (type + 1);
}

Primitive_type value_at(Primitive_type type)
{
    if ((type & 0xf) == 0x0)
        custom_error_int("Unrecognised in value_at: type", type);
    return (type - 1);
}

int typesize(Primitive_type type, struct symtable *ctype)
{
    if (type == P_STRUCT || type == P_UNION)
        return ctype->size;
    return genprimsize(type);
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
        lsize = typesize(ltype, NULL);
        rsize = typesize(rtype, NULL);
        if (lsize > rsize)
            return NULL;
        if (rsize > lsize)
            return mkAST_left(A_WIDEN, rtype, tree, NULL, 0);
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
                return mkAST_left(A_SCALE, rtype, tree, NULL, rsize);
            else
                return tree;
        }
    }
    return NULL;
}