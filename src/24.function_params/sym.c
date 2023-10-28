#include "function.h"

int findglob(char *s)
{
    for (int i = 0; i < Globals; i++)
    {
        if (*Gsym[i].name == *s && !strcmp(Gsym[i].name, s))
            return i;
    }
    return -1;
}

// Determine if the symbol s is in the local symbol table.
// Return its slot position or -1 if not found.
int findlocl(char *s)
{
    int i;

    for (i = Locls + 1; i < NSYMBOLS; i++)
    {
        if (*s == *Gsym[i].name && !strcmp(s, Gsym[i].name))
            return (i);
    }
    return (-1);
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
    if ((p = Globals++) == NSYMBOLS)
    {
        custom_error_int("out of range NSYMBOLS", NSYMBOLS);
    }
    return p;
}

// Get the position of a new local symbol slot, or die
// if we've run out of positions.
static int newlocl(void)
{
    int p;

    if ((p = Locls--) <= Globals)
        custom_error_int("Too many local symbols", 0);
    return (p);
}

// Clear all the entries in the
// local symbol table
void freeloclsyms(void)
{
    Locls = NSYMBOLS - 1;
}

// Update a symbol at the given slot number in the symbol table. Set up its:
// + type: char, int etc.
// + structural type: var, function, array etc.
// + size: number of elements
// + endlabel: if this is a function
// + posn: Position information for local symbols
static void updatesym(int id, char *name, Primitive_type type, Structural_type stype,
                      Storage_class class, int endlabel, int size, int posn)
{
    if (id < 0 || id >= NSYMBOLS)
        custom_error_int("Invalid symbol slot number in updatesym()", 0);
    Gsym[id].name = my_strdup(name);
    Gsym[id].type = type;
    Gsym[id].stype = stype;
    Gsym[id].class = class;
    Gsym[id].endlabel = endlabel;
    Gsym[id].size = size;
    Gsym[id].posn = posn;
}

int addglob(char *name, Primitive_type type, Structural_type stype,
            int endlabel, int size)
{
    int id;
    if ((id = findglob(name)) != -1)
        return id;
    id = newglob();
    updatesym(id, name, type, stype, C_Globals, endlabel, size, 0);
    genglobsym(id);
    return id;
}

int addlocl(char *name, Primitive_type type, Structural_type stype,
            int isparam, int size)
{
    int id, posn;
    // If this is already in the symbol table, return the existing slot
    if ((id = findlocl(name)) != -1)
        return id;

    id = newlocl();
    //--------------------------------
    // posn = gengetlocaloffset(type, 0);
    // printf("posn=%d\n", posn);
    updatesym(id, name, type, stype, C_LOCAL, 0, size, 0);
    return id;
}

// Determine if the symbol s is in the symbol table.
// Return its slot position or -1 if not found.
int findsymbol(char *s)
{
    int slot;

    slot = findlocl(s);
    if (slot == -1)
        slot = findglob(s);
    return (slot);
}
