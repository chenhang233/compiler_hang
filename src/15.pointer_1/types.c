#include "function.h"

int type_compatible(Primitive_type *left, Primitive_type *right, int onlyright)
{
    int leftsize, rightsize;
    if (*left == *right)
    {
        *left = *right = 0;
        return 1;
    }

    leftsize = genprimsize(*left);
    rightsize = genprimsize(*right);

    if ((leftsize == 0) || (rightsize == 0))
        return (0);

    if (leftsize < rightsize)
    {
        *left = A_WIDEN;
        *right = 0;
        return (1);
    }
    if (rightsize < leftsize)
    {
        if (onlyright)
            return (0);
        *left = 0;
        *right = A_WIDEN;
        return (1);
    }
    *left = *right = 0;
    return 1;
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
    return (newtype);
}

Primitive_type value_at(Primitive_type type)
{
    Primitive_type new_type;
    switch (type)
    {
    case P_VOIDPTR:
        new_type = P_VOID;
        break;
    case P_CHARPTR:
        new_type = P_CHAR;
        break;
    case P_INTPTR:
        new_type = P_INT;
        break;
    case P_LONGPTR:
        new_type = P_LONG;
        break;
    default:
        custom_error_int("Unrecognised in value_at: type", type);
        break;
    }
    return new_type;
}