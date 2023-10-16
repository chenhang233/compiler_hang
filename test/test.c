#include <stdio.h>

int main(int argc, char const *argv[])
{
    int arr[2];
    arr[0] = 10;
    arr[1] = 20;

    int *p;
    p = arr;
    printf("p=%p\n", p);
    printf("p=%p\n", p + 1);
    p = &arr[0];
    printf("p=%p\n", p);
    printf("p=%p\n", p + 1);
    p = &arr;
    printf("p=%p\n", p);
    printf("p=%p\n", p + 1);

    return 0;
}
