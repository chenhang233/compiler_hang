#include <stdio.h>
#include <string.h>

static int strIntPos(char *str, int c)
{
    char *p;
    p = strchr(str, c);
    printf("p=%d --str=%d\n", p, str);
    return p ? p - str : -1;
}

int main(int argc, char const *argv[])
{
    strIntPos("0123456789", '0');
    strIntPos("0123456789", '9');
    return 0;
}
