#include "function.h"

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
        custom_error_char(" source", *source);
    int size = strlen(source) + 1;
    char *dest = malloc(size * sizeof(char));
    strcpy(dest, source);
    return dest;
};

static int newglob()
{
    int p;
    if ((p = global++) == TEXTLEN)
        custom_error_int("out of range global", p);
    return p;
};

int addglob(char *name, Primitive_type type, Structural_type stype, int endlabel)
{
    int id;
    if ((id = findglob(name)) != -1)
        return id;
    id = newglob();
    Gsym[id].name = my_strdup(name);
    Gsym[id].type = type;
    Gsym[id].stype = stype;
    Gsym[id].endlabel = endlabel;
    return id;
};