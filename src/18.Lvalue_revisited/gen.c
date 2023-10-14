#include "function.h"

int genlabel(void)
{
    static int id = 1;
    return (id++);
}

int genAST(ASTnode *n, int reg, AST_node_type parentASTop)
{
}

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
