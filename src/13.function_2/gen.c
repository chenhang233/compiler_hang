#include "function.h"

int label()
{
    static int label = 1;
    return label++;
}

int genWhile(ASTnode *n)
{
    int start = label(), end = label();
    cglabel(start);
    genAST(n->left, end, n->op);
    genfreeregs();
    genAST(n->right, NOREG, n->op);
    genfreeregs();
    cgjump(start);
    cglabel(end);
    return NOREG;
}

int genIf(ASTnode *n)
{
    int Lfalse, Lend;
    Lfalse = label();
    if (n->right)
        Lend = label();
    genAST(n->left, Lfalse, n->op);
    genfreeregs();
    genAST(n->mid, NOREG, n->op);
    genfreeregs();
    if (n->right)
        cgjump(Lend);
    cglabel(Lfalse);
    if (n->right)
    {
        genAST(n->right, NOREG, n->op);
        genfreeregs();
        cglabel(Lend);
    }
    return NOREG;
}

int genAST(ASTnode *n, int reg, AST_node_type parentASTop)
{
    // printf("n--%d\n", n->op);
    int left_reg, right_reg;
    switch (n->op)
    {
    case A_IF:
        return genIf(n);
    case A_WHILE:
        return genWhile(n);
    case A_GLUE:
        genAST(n->left, NOREG, n->op);
        genfreeregs();
        genAST(n->right, NOREG, n->op);
        genfreeregs();
        return NOREG;
    case A_FUNCTION:
        cgfuncpreamble(n->v.id);
        if (n->left)
            genAST(n->left, NOREG, n->op);
        cgfuncpostamble(n->v.id);
        return (NOREG);
    }
    if (n->left)
        left_reg = genAST(n->left, NOREG, n->op);
    if (n->right)
    {
        right_reg = genAST(n->right, left_reg, n->op);
    }
    switch (n->op)
    {
    case A_ADD:
        return (cgadd(left_reg, right_reg));
    case A_SUBTRACT:
        return (cgsub(left_reg, right_reg));
    case A_MULTIPLY:
        return (cgmul(left_reg, right_reg));
    case A_DIVIDE:
        return (cgdiv(left_reg, right_reg));
    case A_EQ:
    case A_NE:
    case A_LT:
    case A_GT:
    case A_LE:
    case A_GE:
        if (parentASTop == A_IF || parentASTop == A_WHILE)
            return cgcompare_and_jump(n->op, left_reg, right_reg, reg);
        else
            return cgcompare_and_set(n->op, left_reg, right_reg);
    case A_INTLIT:
        // printf("n->v.intvalue=%d type=%d\n", n->v.intvalue, n->type);
        return cgloadint(n->v.intvalue, n->type);
    case A_ASSIGN:
        // printf("A_ASSIGN=%d\n", right_reg);
        return right_reg;
    case A_IDENT:
        return cgloadglob(n->v.id);
    case A_LVIDENT:
        return cgstorglob(reg, n->v.id);
    case A_WIDEN:
        // printf("A_WIDEN=%d\n", left_reg);
        return cgwiden(left_reg, n->left->type, n->type);
    case A_PRINT:
        cgprintint(left_reg);
        genfreeregs();
        return NOREG;
    case A_RETURN:
        cgreturn(left_reg, Functionid);
        return (NOREG);
    case A_FUNCCALL:
        return (cgcall(left_reg, n->v.id));
    default:
        custom_error_int("Unknown AST operator", n->op);
    }
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
int genprimsize(int type)
{
    return (cgprimsize(type));
}