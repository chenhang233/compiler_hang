#include "function.h"

int genAST(ASTnode *n, int reg, AST_node_type parentASTop)
{
}

void genpreamble()
{
    cgpreamble();
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
