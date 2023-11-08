#include "function.h"

int genlabel(void)
{
    static int id = 1;
    return (id++);
}

void genresetlocals(void)
{
    cgresetlocals();
}

// int gengetlocaloffset(int type, int isparam)
// {
//     return (cggetlocaloffset(type, isparam));
// }

int genIf(ASTnode *n)
{
    int Lfalse, Lend;

    Lfalse = genlabel();
    if (n->right)
        Lend = genlabel();

    genAST(n->left, Lfalse, n->op);
    genfreeregs();

    genAST(n->mid, NOLABEL, n->op);
    genfreeregs();

    if (n->right)
    {
        genAST(n->right, NOLABEL, n->op);
        genfreeregs();
        cglabel(Lend);
    }

    cglabel(Lfalse);
    return (NOREG);
}

int genWhile(ASTnode *n)
{
    int Lstart, Lend;

    Lstart = genlabel();
    Lend = genlabel();
    cglabel(Lstart);

    genAST(n->left, Lend, n->op);
    genfreeregs();

    genAST(n->right, NOLABEL, n->op);
    genfreeregs();

    cgjump(Lstart);
    cglabel(Lend);
    return (NOREG);
}

static int gen_funccall(ASTnode *n)
{
    ASTnode *glue_tree = n->left;
    int arg_num = 0;
    int reg;
    while (glue_tree)
    {
        reg = genAST(glue_tree->right, NOLABEL, glue_tree->op);
        cgcopyarg(reg, glue_tree->v.size);
        if (arg_num == 0)
            arg_num = glue_tree->v.size;
        genfreeregs();
        glue_tree = glue_tree->left;
    }

    return cgcall(n->v.id, arg_num);
}

int genAST(ASTnode *n, int label, AST_node_type parentASTop)
{
    int leftreg, rightreg;
    // printf("n=%p\n", n);
    switch (n->op)
    {
    case A_IF:
        return genIf(n);
    case A_WHILE:
        return (genWhile(n));
    case A_FUNCCALL:
        return (gen_funccall(n));
    case A_GLUE:
        genAST(n->left, NOLABEL, n->op);
        genfreeregs();
        genAST(n->right, NOLABEL, n->op);
        genfreeregs();
        return (NOREG);
    case A_FUNCTION:
        cgfuncpreamble(n->sym);
        genAST(n->left, NOLABEL, n->op);
        cgfuncpostamble(n->sym);
        return (NOREG);
    }

    if (n->left)
        leftreg = genAST(n->left, NOLABEL, n->op);
    if (n->right)
        rightreg = genAST(n->right, NOLABEL, n->op);

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
    case A_AND:
        return (cgand(leftreg, rightreg));
    case A_OR:
        return (cgor(leftreg, rightreg));
    case A_XOR:
        return (cgxor(leftreg, rightreg));
    case A_LSHIFT:
        return (cgshl(leftreg, rightreg));
    case A_RSHIFT:
        return (cgshr(leftreg, rightreg));
    case A_EQ:
    case A_NE:
    case A_LT:
    case A_GT:
    case A_LE:
    case A_GE:

        if (parentASTop == A_IF || parentASTop == A_WHILE)
            return (cgcompare_and_jump(n->op, leftreg, rightreg, label));
        else
            return (cgcompare_and_set(n->op, leftreg, rightreg));
    case A_INTLIT:
        return (cgloadint(n->v.intvalue, n->type));
    case A_STRLIT:
        return (cgloadglobstr(n->v.id));
    case A_IDENT:
        // Load our value if we are an rvalue
        // or we are being dereferenced
        if (n->rvalue || parentASTop == A_DEREF)
            if (Gsym[n->v.id].class == C_GLOBAL)
                return (cgloadglob(n->v.id, n->op));
            else
                return (cgloadlocal(n->v.id, n->op));
        else
            return (NOREG);
    case A_ASSIGN:
        // Are we assigning to an identifier or through a pointer?
        switch (n->right->op)
        {
        case A_IDENT:
            if (Gsym[n->right->v.id].class == C_GLOBAL)
                return cgstorglob(leftreg, n->right->v.id);
            else
                return cgstorlocal(leftreg, n->right->v.id);
        case A_DEREF:
            return (cgstorderef(leftreg, rightreg, n->right->type));
        default:
            custom_error_int("Can't A_ASSIGN in genAST(), op", n->op);
        }
    case A_WIDEN:
        // Widen the child's type to the parent's type
        return (cgwiden(leftreg, n->left->type, n->type));
    case A_RETURN:
        cgreturn(leftreg, Functionid);
        return (NOREG);
    case A_ADDR:
        return (cgaddress(n->v.id));
    case A_DEREF:
        // If we are an rvalue, dereference to get the value we point at,
        // otherwise leave it for A_ASSIGN to store through the pointer
        if (n->rvalue)
            return (cgderef(leftreg, n->left->type));
        else
            return (leftreg);
    case A_SCALE:
        // Small optimisation: use shift if the
        // scale value is a known power of two
        switch (n->v.size)
        {
        case 2:
            return (cgshlconst(leftreg, 1));
        case 4:
            return (cgshlconst(leftreg, 2));
        case 8:
            return (cgshlconst(leftreg, 3));
        default:
            rightreg = cgloadint(n->v.size, P_INT);
            return (cgmul(leftreg, rightreg));
        }
    case A_POSTINC:
    case A_POSTDEC:
        if (Gsym[n->v.id].class == C_GLOBAL)
            return (cgloadglob(n->v.id, n->op));
        else
            return (cgloadlocal(n->v.id, n->op));
    case A_PREINC:
        if (Gsym[n->v.id].class == C_GLOBAL)
            return (cgloadglob(n->left->v.id, n->op));
        else
            return (cgloadlocal(n->left->v.id, n->op));
    case A_NEGATE: // -digit
        return (cgnegate(leftreg));
    case A_INVERT:
        return (cginvert(leftreg));
    case A_LOGNOT:
        return (cglognot(leftreg));
    case A_TOBOOL:

        return (cgboolean(leftreg, parentASTop, label));
    default:
        custom_error_int("Unknown AST operator", n->op);
    }
    return (NOREG);
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

int genglobstr(char *strvalue)
{
    int l = genlabel();
    cgglobstr(l, strvalue);
    return l;
}