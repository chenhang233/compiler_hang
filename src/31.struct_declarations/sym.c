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
        custom_error_int("Either head, tail or node is NULL in appendsym", 0);
    if (*tail)
    {
        (*tail)->next = node;
        *tail = node;
    }
    else
        *head = *tail = node;
}

symtable *newsym(char *name, Primitive_type type, symtable *ctype, Structural_type stype,
                 Storage_class class, int size, int posn)
{
    symtable *node = malloc(sizeof(symtable));
    if (node == NULL)
        custom_error_chars("Unable to malloc a symbol table node in newsym", "newsym()");
    node->name = my_strdup(name);
    node->type = type;
    node->ctype = ctype;
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

symtable *addglob(char *name, Primitive_type type, symtable *ctype,
                  Structural_type stype, int size)
{
    symtable *sym = newsym(name, type, ctype, stype, C_GLOBAL, size, 0);
    appendsym(&Globhead, &Globtail, sym);
    return sym;
}

symtable *addlocl(char *name, Primitive_type type, symtable *ctype, Structural_type stype,
                  int size)
{
    symtable *sym = newsym(name, type, ctype, stype, C_LOCAL, size, 0);
    appendsym(&Loclhead, &Locltail, sym);
    return (sym);
}

symtable *addparm(char *name, Primitive_type type, symtable *ctype, Structural_type stype,
                  int size)
{
    symtable *sym = newsym(name, type, ctype, stype, C_PARAM, size, 0);
    appendsym(&Parmhead, &Parmtail, sym);
    return (sym);
}

// Add a symbol to the temporary member list
symtable *addmemb(char *name, Primitive_type type, symtable *ctype,
                  Structural_type stype, int size)
{
    symtable *sym = newsym(name, type, ctype, stype, C_MEMBER, size, 0);
    appendsym(&Membhead, &Membtail, sym);
    return (sym);
}

symtable *addstruct(char *name, Primitive_type type, symtable *ctype,
                    Structural_type stype, int size)
{
    symtable *sym = newsym(name, type, ctype, stype, C_STRUCT, size, 0);
    appendsym(&Structhead, &Structhead, sym);
    return (sym);
}

static struct symtable *findsyminlist(char *s, struct symtable *list)
{
    for (; list != NULL; list = list->next)
        if ((list->name != NULL) && !strcmp(s, list->name))
            return (list);
    return (NULL);
}

// Determine if the symbol s is in the local symbol table.
// Return its slot position or -1 if not found.
symtable *findlocl(char *s)
{
    symtable *node;
    if (Functionid)
    {
        node = findsyminlist(s, Functionid->member);
        if (node)
            return node;
    }
    return findsyminlist(s, Loclhead);
}

symtable *findglob(char *s)
{
    return (findsyminlist(s, Globhead));
}

symtable *findsymbol(char *s)
{
    symtable *node;

    if (Functionid)
    {
        node = findsyminlist(s, Functionid->member);
        if (node)
            return (node);
    }
    node = findsyminlist(s, Loclhead);
    if (node)
        return (node);
    return (findsyminlist(s, Globhead));
}

// Find a member in the member list
// Return a pointer to the found node or NULL if not found.
symtable *findmember(char *s)
{
    return (findsyminlist(s, Membhead));
}

// Find a struct in the struct list
// Return a pointer to the found node or NULL if not found.
symtable *findstruct(char *s)
{
    return (findsyminlist(s, Structhead));
}

void clear_symtable(void)
{
    Globhead = Globtail = NULL;
    Loclhead = Locltail = NULL;
    Parmhead = Parmtail = NULL;
    Membhead = Membtail = NULL;
    Structhead = Structtail = NULL;
}

// Clear all the entries in the local symbol table
void freeloclsyms(void)
{
    Loclhead = Locltail = NULL;
    Parmhead = Parmtail = NULL;
    Functionid = NULL;
}
