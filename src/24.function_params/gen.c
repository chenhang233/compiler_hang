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

    // Generate two labels: one for the
    // false compound statement, and one
    // for the end of the overall IF statement.
    // When there is no ELSE clause, Lfalse _is_
    // the ending label!
    Lfalse = genlabel();
    if (n->right)
        Lend = genlabel();

    // Generate the condition code followed
    // by a jump to the false label.
    genAST(n->left, Lfalse, n->op);
    genfreeregs();

    // Generate the true compound statement
    genAST(n->mid, NOLABEL, n->op);
    genfreeregs();

    // If there is an optional ELSE clause,
    // generate the jump to skip to the end
    if (n->right)
    {
        genAST(n->right, NOLABEL, n->op);
        genfreeregs();
        cglabel(Lend);
    }

    // Now the false label
    cglabel(Lfalse);
}

int genWhile(ASTnode *n)
{
    int Lstart, Lend;

    // Generate the start and end labels
    // and output the start label
    Lstart = genlabel();
    Lend = genlabel();
    cglabel(Lstart);

    // Generate the condition code followed
    // by a jump to the end label.
    genAST(n->left, Lend, n->op);
    genfreeregs();

    // Generate the compound statement for the body
    genAST(n->right, NOLABEL, n->op);
    genfreeregs();

    // Finally output the jump back to the condition,
    // and the end label
    cgjump(Lstart);
    cglabel(Lend);
    return (NOREG);
}

int genAST(ASTnode *n, int label, AST_node_type parentASTop)
{
    int leftreg, rightreg;
    switch (n->op)
    {
    case A_IF:
        return genIf(n);
    case A_WHILE:
        return (genWhile(n));
    case A_GLUE:
        // Do each child statement, and free the
        // registers after each child
        genAST(n->left, NOLABEL, n->op);
        genfreeregs();
        genAST(n->right, NOLABEL, n->op);
        genfreeregs();
        return (NOREG);
    case A_FUNCTION:
        // Generate the function's preamble before the code
        // in the child sub-tree
        cgfuncpreamble(n->v.id);
        genAST(n->left, NOLABEL, n->op);
        cgfuncpostamble(n->v.id);
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
        // If the parent AST node is an A_IF or A_WHILE, generate
        // a compare followed by a jump. Otherwise, compare registers
        // and set one to 1 or 0 based on the comparison.
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
            {
                return (cgloadglob(n->v.id, n->op));
            }
            else
            {
                return (cgloadlocal(n->v.id, n->op));
            }
        else
            return (NOREG);
    case A_ASSIGN:
        // Are we assigning to an identifier or through a pointer?
        switch (n->right->op)
        {
        case A_IDENT:
            if (Gsym[n->right->v.id].class == C_LOCAL)
                return cgstorlocal(leftreg, n->right->v.id);
            else
                return cgstorglob(leftreg, n->right->v.id);
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
    case A_FUNCCALL:
        return (cgcall(leftreg, n->v.id));
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
            // Load a register with the size and
            // multiply the leftreg by this size
            rightreg = cgloadint(n->v.size, P_INT);
            return (cgmul(leftreg, rightreg));
        }
    case A_POSTINC:
        // Load the variable's value into a register,
        // then increment it
        return (cgloadglob(n->v.id, n->op));
    case A_POSTDEC:
        // Load the variable's value into a register,
        // then decrement it
        return (cgloadglob(n->v.id, n->op));
    case A_PREINC:
        // Load and increment the variable's value into a register
        return (cgloadglob(n->left->v.id, n->op));
    case A_PREDEC:
        // Load and decrement the variable's value into a register
        return (cgloadglob(n->left->v.id, n->op));
    case A_NEGATE: // -digit
        return (cgnegate(leftreg));
    case A_INVERT:
        return (cginvert(leftreg));
    case A_LOGNOT:
        return (cglognot(leftreg));
    case A_TOBOOL:
        // If the parent AST node is an A_IF or A_WHILE, generate
        // a compare followed by a jump. Otherwise, set the register
        // to 0 or 1 based on it's zeroeness or non-zeroeness
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