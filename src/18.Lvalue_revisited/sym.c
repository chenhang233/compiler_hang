#include "function.h"

static int global = 0;

int findglob(char *s)
{
    for (int i = 0; i < global; i++)
    {
        if (*Gsym[i].name == *s && !strcmp(Gsym[i].name, s))
            return i;
    }
    return -1;
}

char *my_strdup(const char *source)
{
    if (source == NULL)
        custom_error_int("NULL source", 18);
    int len = strlen(source) + 1;
    char *dest = malloc(len * sizeof(char));
    strcpy(dest, source);
    return dest;
}

static int newglob()
{
    int p;
    if ((p = global++) == TEXTLEN)
    {
        custom_error_int("out of range TEXTLEN", TEXTLEN);
    }
    return p;
}

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
}