#include <stdio.h>

int main()
{

    int a = -15;
    a >>= 2;
    // 1000 1111
    // 1111 0001 --> 1111 1100
    // 1111 1011 --> 1000 0100 -4
    printf("%d\n", a);
    return 0;
}
