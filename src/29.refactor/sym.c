#include "function.h"

char *my_strdup(const char *source)
{
    if (source == NULL)
        return NULL;
    int len = strlen(source) + 1;
    char *dest = malloc(len * sizeof(char));
    strcpy(dest, source);
    return dest;
}

void appendsym(symtable **head, symtable **tail,
               symtable *node)
{
    if (head == NULL || tail == NULL || node == NULL)
        custom_error_chars("Either head, tail or node is NULL in appendsym");
    if (*tail)
    {
        (*tail)->next = node;
        *tail = node;
    }
    else
        *head = *tail = node;
}

symtable *newsym(char *name, Primitive_type type, Structural_type stype,
                 Storage_class class, int size, int posn)
{
    symtable *node = malloc(sizeof(symtable));
    if (node == NULL)
        custom_error_chars("Unable to malloc a symbol table node in newsym", "newsym()");
    node->name = my_strdup(name);
    node->type = type;
    node->stype = stype;
    node->class = class;
    node->size = size;
    node->posn = posn;
    node->next = NULL;
    node->member = NULL;
    if (class == C_GLOBAL)
        genglobsym(node);
    return node;
}

symtable *addglob(char *name, Primitive_type type, Structural_type stype,
                  Storage_class class, int size)
{
    symtable *sym = newsym(name, type, stype, class, size, 0);
    appendsym(&Globhead, &Globtail, sym);
    return sym;
}

symtable *addlocl(char *name, Primitive_type type, Structural_type stype,
                  Storage_class class, int size)
{
    struct symtable *sym = newsym(name, type, stype, class, size, 0);
    appendsym(&Loclhead, &Locltail, sym);
    return (sym);
}

symtable *addparm(char *name, Primitive_type type, Structural_type stype,
                  Storage_class class, int size)
{
    struct symtable *sym = newsym(name, type, stype, class, size, 0);
    appendsym(&Parmhead, &Parmtail, sym);
    return (sym);
}

static struct symtable *findsyminlist(char *s, struct symtable *list)
{
    for (; list != NULL; list = list->next)
        if ((list->name != NULL) && !strcmp(s, list->name))
            return (list);
    return (NULL);
}

symtable *findglob(char *name)
{
    return (findsyminlist(name, Globhead));
}

// Determine if the symbol s is in the local symbol table.
// Return its slot position or -1 if not found.
symtable *findlocl(char *s)
{
    int i;
    // printf("Locls=%d\n", Locls);
    for (i = Locls + 1; i < NSYMBOLS; i++)
    {
        if (*s == *Gsym[i].name && !strcmp(s, Gsym[i].name))
            return (i);
    }
    return (-1);
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

void clear_symtable(void)
{
    Globals = 0;
    Locls = NSYMBOLS - 1;
}

// Clear all the entries in the local symbol table
void freeloclsyms(void)
{
    Locls = NSYMBOLS - 1;
}
