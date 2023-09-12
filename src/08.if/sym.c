#include "function.h"

// symbol table.c

static int global = 0;

int findglob(char *s)
{
    int i;
    for (i = 0; i < global; i++)
    {
        if (*s == *Gsym[i].name && !strcmp(s, Gsym[i].name))
        {
            return i;
        }
    }
    return -1;
};

char *my_strdup(const char *source)
{
    if (source == NULL)
    {
        return NULL;
    }

    size_t len = strlen(source) + 1;
    char *copy = (char *)malloc(len * sizeof(char));
    if (copy == NULL)
    {
        return NULL;
    }

    strcpy(copy, source);
    return copy;
}

static int newglob()
{
    if (++global == TEXT_LEN)
    {
        custom_error_int("too long", TEXT_LEN);
    };
    return global;
}

int addglob(char *name)
{
    int i;
    if ((i = findglob(name)) != -1)
    {
        return i;
    }
    i = newglob();
    Gsym[i].name = my_strdup(name);
    return;
}