#include "function.h"

/*
    symbol table function
 */

static int Global = 0;

int global_find(char *s)
{
    int i = 0;
    while (i < Global)
    {
        if (*s == *Global_symbol_map[i].name && !strcmp(s, Global_symbol_map[i].name))
        {
            return i;
        }
        i++;
    }
    return -1;
}

static int global_new()
{
    int p = Global++;
    if (Global >= GLOBAL_SYMBOOL_LEN)
    {
        custom_error_int("Too many global symbols", p);
    }
    return p;
}

int global_add(char *s)
{
    int i;
    if ((i = global_find(s)) != -1)
    {
        return i;
    }
    i = global_new();
    Global_symbol_map[i].name = strdup(s);
    return i;
}