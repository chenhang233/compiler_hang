#include "function.h"

void genpreamble()
{
    cgpreamble();
}
void genpostamble()
{
    cgpostamble();
}
void genfreeregs()
{
    freeall_registers();
}
void genprintint(int reg)
{
    cgprintint(reg);
}
void genglobsym(int id)
{
    cgglobsym(id);
}
int genprimsize(int type)
{
    return (cgprimsize(type));
}
