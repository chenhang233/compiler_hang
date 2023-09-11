#include "function.h"

// generator.c

// static int genIFAST(struct ASTnode *n)

// int genAST(struct ASTnode *n, int reg, int parentASTop);

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

void genglobsym(char *s)
{
    cgglobsym(s);
}
