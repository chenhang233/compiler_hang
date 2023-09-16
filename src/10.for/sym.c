#include "function.h"

// symbol table.c

static int global = 0;

int findglob(char *s)
{
    int i;
    for (i = 0; i < global; i++)
    {
        if (*s == *Gsym[i].name && !strcmp(s, Gsym[i].name))
            return i;
    }
    return -1;
};

char *my_strdup(const char *source)
{
    if (source == NULL)
        custom_error_chars("NULL source", (char *)source);
    size_t len = strlen(source) + 1;
    char *copy = (char *)malloc(len * sizeof(char));
    if (copy == NULL)
        custom_error_chars("malloc copy", copy);
    strcpy(copy, source);
    return copy;
}

static int newglob()
{
    int p;
    if ((p = global++) == TEXT_LEN)
    {
        custom_error_int("too long", TEXT_LEN);
    };
    return p;
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
    return i;
}