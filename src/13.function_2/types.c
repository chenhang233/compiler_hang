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