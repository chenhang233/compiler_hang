#include "function.h"

int type_compatible(Primitive_type *left, Primitive_type *right, int onlyright)
{
    if ((*left == P_VOID) || (*right == P_VOID))
        return (0);
    if (*left == *right)
    {
        *left = *right = 0;
        return 1;
    }
    if (*left == P_CHAR && *right == P_INT)
    {
        *left = A_WIDEN;
        *right = 0;
        return 1;
    }
    if (*left == P_INT && *right == P_CHAR)
    {
        if (onlyright)
            return 0;
        *right = A_WIDEN;
        *left = 0;
    }
    *left = *right = 0;
    return 1;
}