#include "function.h"

// generator.c

static int label(void)
{
    static int id = 1;
    return (id++);
}

static int genIFAST(AST_node *n)
{
    int F_L, N_L;
    F_L = label();
    if (n->right)
        N_L = label();

    genAST(n->left, F_L, n->op);
    genfreeregs();

    genAST(n->mid, NOREG, n->op);
    genfreeregs();
    if (n->right)
    {
        cgjump(N_L);
    }
    cglabel(F_L);
    if (n->right)
    {
        genAST(n->right, NOREG, n->op);
        genfreeregs();
        cglabel(N_L);
    }
    return NOREG;
}

static int genWhileAST(AST_node *n)
{
    int L1, L2;
    L1 = label();
    L2 = label();
    cglabel(L1);
    genAST(n->left, L2, n->op);
    genfreeregs();
    genAST(n->right, NOREG, n->op);
    genfreeregs();
    cgjump(L1);
    cglabel(L2);
    return NOREG;
}

int genAST(AST_node *n, int reg, AST_type parentASTop)
{
    int leftreg, rightreg;
    switch (n->op)
    {
    case A_IF:
        return genIFAST(n);
    case A_WHILE:
        return genWhileAST(n);
    case A_GLUE:
        genAST(n->left, NOREG, n->op);
        freeall_registers();
        genAST(n->right, NOREG, n->op);
        freeall_registers();
        return NOREG;
    }

    if (n->left)
        leftreg = genAST(n->left, NOREG, n->op);
    if (n->right)
        rightreg = genAST(n->right, leftreg, n->op);

    switch (n->op)
    {
    case A_ADD:
        return (cgadd(leftreg, rightreg));
    case A_SUBTRACT:
        return (cgsub(leftreg, rightreg));
    case A_MULTIPLY:
        return (cgmul(leftreg, rightreg));
    case A_DIVIDE:
        return (cgdiv(leftreg, rightreg));
    case A_EQ:
    case A_NE:
    case A_LT:
    case A_GT:
    case A_LE:
    case A_GE:
        if (parentASTop == A_IF || parentASTop == A_WHILE)
            return cgcompare_and_jump(n->op, leftreg, rightreg, reg);
        else
            return cgcompare_and_set(n->op, leftreg, rightreg);
    case A_INTLIT:
        return (cgloadint(n->v.intv));
    case A_IDENT:
        return (cgloadglob(Gsym[n->v.id].name));
    case A_LVIDENT:
        return (cgstorglob(reg, Gsym[n->v.id].name));
    case A_ASSIGN:
        // The work has already been done, return the result
        return (rightreg);
    case A_PRINT:
        // Print the left-child's value
        // and return no register
        genprintint(leftreg);
        genfreeregs();
        return (NOREG);
    default:
        custom_error_int("Unknown AST operator", n->op);
    }
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

void genglobsym(char *s)
{
    cgglobsym(s);
}
