#include "function.h"

// Flag to say which section were are outputting in to
enum
{
    no_seg,
    text_seg,
    data_seg
} currSeg = no_seg;

void cgtextseg()
{
    if (currSeg != text_seg)
    {
        fputs("\t.text\n", Outfile);
        currSeg = text_seg;
    }
}

void cgdataseg()
{
    if (currSeg != data_seg)
    {
        fputs("\t.data\n", Outfile);
        currSeg = data_seg;
    }
}

static int localOffset;
static int stackOffset;

// Reset the position of new local variables when parsing a new function
void cgresetlocals(void)
{
    localOffset = 0;
}

// Get the position of the next local variable.
int newlocaloffset(int type)
{
    localOffset += cgprimsize(type) > 4 ? cgprimsize(type) : 4;
    return -localOffset;
}

#define NUMFREEREGS 4
#define FIRSTPARAMREG 9 // Position of first parameter register
static int freereg[NUMFREEREGS];
static char *reglist[] =
    {"%r10", "%r11", "%r12", "%r13", "%r9", "%r8", "%rcx", "%rdx",
     "%rsi", "%rdi"};
static char *breglist[] =
    {"%r10b", "%r11b", "%r12b", "%r13b", "%r9b", "%r8b", "%cl",
     "%dl", "%sil", "%dil"};
static char *dreglist[] =
    {"%r10d", "%r11d", "%r12d", "%r13d", "%r9d", "%r8d", "%ecx",
     "%edx", "%esi", "%edi"};

void freeall_registers(void)
{
    freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
}

static int alloc_register(void)
{
    for (int i = 0; i < NUMFREEREGS; i++)
    {
        if (freereg[i])
        {
            freereg[i] = 0;
            return (i);
        }
    }
    custom_error_int("Out of registers", 0);
}

static void free_register(int reg)
{
    if (freereg[reg] != 0)
        custom_error_int("Error trying to free register", reg);
    freereg[reg] = 1;
}

void cgpreamble()
{
    freeall_registers();
}

void cgpostamble()
{
}

void cgfuncpreamble(int id)
{
    int i;
    char *name = Gsym[id].name;
    int paramOffset = 16;
    int paramReg = FIRSTPARAMREG;
    cgtextseg();
    localOffset = 0;
    fprintf(Outfile,
            "\t.globl\t%s\n"
            "\t.type\t%s, @function\n"
            "%s:\n"
            "\tpushq\t%%rbp\n"
            "\tmovq\t%%rsp, %%rbp\n",
            name, name, name);
    // Copy any in-register parameters to the stack
    // Stop after no more than six parameter registers
    for (i = NSYMBOLS - 1; i > Locls; i--)
    {
        if (Gsym[i].class != C_PARAM)
            break;
        if (i < NSYMBOLS - 6)
            break;
        Gsym[i].posn = newlocaloffset(Gsym[i].type);
        cgstorlocal(paramReg--, i);
    }
    // For the remainder, if they are a parameter then they are
    // already on the stack. If only a local, make a stack position.  for (; i > Locls; i--)
    for (; i > Locls; i--)
    {
        if (Gsym[i].class == C_PARAM)
        {
            Gsym[i].posn = paramOffset;
            paramOffset += 8;
        }
        else
        {
            Gsym[i].posn = newlocaloffset(Gsym[i].type);
        }
    }
    // Align the stack pointer to be a multiple of 16
    // less than its previous value
    stackOffset = (localOffset + 15) & ~15;
    fprintf(Outfile, "\taddq\t$%d,%%rsp\n", -stackOffset);
}

void cgfuncpostamble(int id)
{
    cglabel(Gsym[id].endlabel);
    fprintf(Outfile, "\taddq\t$%d,%%rsp\n", stackOffset);
    fputs("\tpopq	%rbp\n"
          "\tret\n",
          Outfile);
}

int cgloadint(int value, int type)
{
    // Get a new register
    int r = alloc_register();

    fprintf(Outfile, "\tmovq\t$%d, %s\n", value, reglist[r]);
    return (r);
}

int cgloadglob(int id, int op)
{
    int r = alloc_register();

    if (cgprimsize(Gsym[id].type) == 8)
    {
        if (op == A_PREINC)
            fprintf(Outfile, "\tincq\t%s(%%rip)\n", Gsym[id].name);
        if (op == A_PREDEC)
            fprintf(Outfile, "\tdecq\t%s(%%rip)\n", Gsym[id].name);
        fprintf(Outfile, "\tmovq\t%s(%%rip), %s\n", Gsym[id].name,
                reglist[r]);
        if (op == A_POSTINC)
            fprintf(Outfile, "\tincq\t%s(%%rip)\n", Gsym[id].name);
        if (op == A_POSTDEC)
            fprintf(Outfile, "\tdecq\t%s(%%rip)\n", Gsym[id].name);
    }
    else
        // Print out the code to initialise it
        switch (Gsym[id].type)
        {
        case P_CHAR:
            if (op == A_PREINC)
                fprintf(Outfile, "\tincb\t%s(%%rip)\n", Gsym[id].name);
            if (op == A_PREDEC)
                fprintf(Outfile, "\tdecb\t%s(%%rip)\n", Gsym[id].name);
            fprintf(Outfile, "\tmovzbq\t%s(%%rip), %s\n", Gsym[id].name,
                    reglist[r]);
            if (op == A_POSTINC)
                fprintf(Outfile, "\tincb\t%s(%%rip)\n", Gsym[id].name);
            if (op == A_POSTDEC)
                fprintf(Outfile, "\tdecb\t%s(%%rip)\n", Gsym[id].name);
            break;
        case P_INT:
            if (op == A_PREINC)
                fprintf(Outfile, "\tincl\t%s(%%rip)\n", Gsym[id].name);
            if (op == A_PREDEC)
                fprintf(Outfile, "\tdecl\t%s(%%rip)\n", Gsym[id].name);
            fprintf(Outfile, "\tmovslq\t%s(%%rip), %s\n", Gsym[id].name,
                    reglist[r]);
            if (op == A_POSTINC)
                fprintf(Outfile, "\tincl\t%s(%%rip)\n", Gsym[id].name);
            if (op == A_POSTDEC)
                fprintf(Outfile, "\tdecl\t%s(%%rip)\n", Gsym[id].name);
            break;
        default:
            custom_error_int("Bad type in cgloadglob:", Gsym[id].type);
        }
    return (r);
}

// Load a value from a local variable into a register.
// Return the number of the register. If the
// operation is pre- or post-increment/decrement,
// also perform this action.
int cgloadlocal(int id, int op)
{
    // Get a new register
    int r = alloc_register();

    // Print out the code to initialise it
    if (cgprimsize(Gsym[id].type) == 8)
    {
        if (op == A_PREINC)
            fprintf(Outfile, "\tincq\t%d(%%rbp)\n", Gsym[id].posn);
        if (op == A_PREDEC)
            fprintf(Outfile, "\tdecq\t%d(%%rbp)\n", Gsym[id].posn);
        fprintf(Outfile, "\tmovq\t%d(%%rbp), %s\n", Gsym[id].posn,
                reglist[r]);
        if (op == A_POSTINC)
            fprintf(Outfile, "\tincq\t%d(%%rbp)\n", Gsym[id].posn);
        if (op == A_POSTDEC)
            fprintf(Outfile, "\tdecq\t%d(%%rbp)\n", Gsym[id].posn);
    }
    else
        switch (Gsym[id].type)
        {
        case P_CHAR:
            if (op == A_PREINC)
                fprintf(Outfile, "\tincb\t%d(%%rbp)\n", Gsym[id].posn);
            if (op == A_PREDEC)
                fprintf(Outfile, "\tdecb\t%d(%%rbp)\n", Gsym[id].posn);
            fprintf(Outfile, "\tmovzbq\t%d(%%rbp), %s\n", Gsym[id].posn,
                    reglist[r]);
            if (op == A_POSTINC)
                fprintf(Outfile, "\tincb\t%d(%%rbp)\n", Gsym[id].posn);
            if (op == A_POSTDEC)
                fprintf(Outfile, "\tdecb\t%d(%%rbp)\n", Gsym[id].posn);
            break;
        case P_INT:
            if (op == A_PREINC)
                fprintf(Outfile, "\tincl\t%d(%%rbp)\n", Gsym[id].posn);
            if (op == A_PREDEC)
                fprintf(Outfile, "\tdecl\t%d(%%rbp)\n", Gsym[id].posn);
            fprintf(Outfile, "\tmovslq\t%d(%%rbp), %s\n", Gsym[id].posn,
                    reglist[r]);
            if (op == A_POSTINC)
                fprintf(Outfile, "\tincl\t%d(%%rbp)\n", Gsym[id].posn);
            if (op == A_POSTDEC)
                fprintf(Outfile, "\tdecl\t%d(%%rbp)\n", Gsym[id].posn);
            break;
        default:
            custom_error_int("Bad type in cgloadlocal:", Gsym[id].type);
        }
    return (r);
}

// Given the label number of a global string,
// load its address into a new register
int cgloadglobstr(int id)
{
    // Get a new register
    int r = alloc_register();
    fprintf(Outfile, "\tleaq\tL%d(\%%rip), %s\n", id, reglist[r]);
    return (r);
}

// Add two registers together and return
// the number of the register with the result
int cgadd(int r1, int r2)
{
    fprintf(Outfile, "\taddq\t%s, %s\n", reglist[r1], reglist[r2]);
    free_register(r1);
    return (r2);
}

// Subtract the second register from the first and
// return the number of the register with the result
int cgsub(int r1, int r2)
{
    fprintf(Outfile, "\tsubq\t%s, %s\n", reglist[r2], reglist[r1]);
    free_register(r2);
    return (r1);
}

// Multiply two registers together and return
// the number of the register with the result
int cgmul(int r1, int r2)
{
    fprintf(Outfile, "\timulq\t%s, %s\n", reglist[r1], reglist[r2]);
    free_register(r1);
    return (r2);
}

// Divide the first register by the second and
// return the number of the register with the result
int cgdiv(int r1, int r2)
{
    fprintf(Outfile, "\tmovq\t%s,%%rax\n", reglist[r1]);
    fprintf(Outfile, "\tcqo\n");
    fprintf(Outfile, "\tidivq\t%s\n", reglist[r2]);
    fprintf(Outfile, "\tmovq\t%%rax,%s\n", reglist[r1]);
    free_register(r2);
    return (r1);
}

int cgand(int r1, int r2)
{
    fprintf(Outfile, "\tandq\t%s, %s\n", reglist[r1], reglist[r2]);
    free_register(r1);
    return (r2);
}

int cgor(int r1, int r2)
{
    fprintf(Outfile, "\torq\t%s, %s\n", reglist[r1], reglist[r2]);
    free_register(r1);
    return (r2);
}

int cgxor(int r1, int r2)
{
    fprintf(Outfile, "\txorq\t%s, %s\n", reglist[r1], reglist[r2]);
    free_register(r1);
    return (r2);
}

int cgshl(int r1, int r2)
{
    fprintf(Outfile, "\tmovb\t%s, %%cl\n", breglist[r2]);
    fprintf(Outfile, "\tshlq\t%%cl, %s\n", reglist[r1]);
    free_register(r2);
    return (r1);
}

int cgshr(int r1, int r2)
{
    fprintf(Outfile, "\tmovb\t%s, %%cl\n", breglist[r2]);
    fprintf(Outfile, "\tshrq\t%%cl, %s\n", reglist[r1]);
    free_register(r2);
    return (r1);
}

// Negate a register's value
int cgnegate(int r)
{
    fprintf(Outfile, "\tnegq\t%s\n", reglist[r]);
    return (r);
}
// Invert a register's value
int cginvert(int r)
{
    fprintf(Outfile, "\tnotq\t%s\n", reglist[r]);
    return (r);
}

// Convert an integer value to a boolean value. Jump if
// it's an IF or WHILE operation
int cgboolean(int r, int op, int label)
{
    fprintf(Outfile, "\ttest\t%s, %s\n", reglist[r], reglist[r]);
    if (op == A_IF || op == A_WHILE)
        fprintf(Outfile, "\tje\tL%d\n", label);
    else
    {
        fprintf(Outfile, "\tsetnz\t%s\n", breglist[r]);
        fprintf(Outfile, "\tmovzbq\t%s, %s\n", breglist[r], reglist[r]);
    }
    return (r);
}

// Call printint() with the given register
void cgprintint(int r)
{
    fprintf(Outfile, "\tmovq\t%s, %%rdi\n", reglist[r]);
    fprintf(Outfile, "\tcall\tprintint\n");
    free_register(r);
}

// Logically negate a register's value
int cglognot(int r)
{
    fprintf(Outfile, "\ttest\t%s, %s\n", reglist[r], reglist[r]);
    fprintf(Outfile, "\tsete\t%s\n", breglist[r]);
    fprintf(Outfile, "\tmovzbq\t%s, %s\n", breglist[r], reglist[r]);
    return (r);
}

// Call a function with the given symbol id
// Pop off any arguments pushed on the stack
// Return the register with the result
int cgcall(int id, int numargs)
{
    // Get a new register
    int outr = alloc_register();
    fprintf(Outfile, "\tcall\t%s@PLT\n", Gsym[id].name);
    // Remove any arguments pushed on the stack
    if (numargs > 6)
        fprintf(Outfile, "\taddq\t$%d, %%rsp\n", 8 * (numargs - 6));
    // and copy the return value into our register
    fprintf(Outfile, "\tmovq\t%%rax, %s\n", reglist[outr]);
    return (outr);
}

// Shift a register left by a constant
int cgshlconst(int r, int val)
{
    fprintf(Outfile, "\tsalq\t$%d, %s\n", val, reglist[r]);
    return (r);
}

// Store a register's value into a variable
int cgstorglob(int r, int id)
{
    if (cgprimsize(Gsym[id].type) == 8)
    {
        fprintf(Outfile, "\tmovq\t%s, %s(%%rip)\n", reglist[r],
                Gsym[id].name);
    }
    else
        switch (Gsym[id].type)
        {
        case P_CHAR:
            fprintf(Outfile, "\tmovb\t%s, %s(%%rip)\n", breglist[r],
                    Gsym[id].name);
            break;
        case P_INT:
            fprintf(Outfile, "\tmovl\t%s, %s(%%rip)\n", dreglist[r],
                    Gsym[id].name);
            break;
        default:
            custom_error_int("Bad type in cgstorglob:", Gsym[id].type);
        }
    return (r);
}

// Store a register's value into a local variable
int cgstorlocal(int r, int id)
{
    if (cgprimsize(Gsym[id].type) == 8)
    {
        fprintf(Outfile, "\tmovq\t%s, %d(%%rbp)\n", reglist[r],
                Gsym[id].posn);
    }
    else
        switch (Gsym[id].type)
        {
        case P_CHAR:
            fprintf(Outfile, "\tmovb\t%s, %d(%%rbp)\n", breglist[r],
                    Gsym[id].posn);
            break;
        case P_INT:
            fprintf(Outfile, "\tmovl\t%s, %d(%%rbp)\n", dreglist[r],
                    Gsym[id].posn);
            break;
        default:
            custom_error_int("Bad type in cgstorlocal:", Gsym[id].type);
        }
    return (r);
}

// Array of type sizes in P_XXX order.
// 0 means no size.
static int psize[] = {0, 0, 1, 4, 8, 8, 8, 8, 8};

// Given a P_XXX type value, return the
// size of a primitive type in bytes.
int cgprimsize(Primitive_type type)
{
    if (ptrtype(type))
        return (8);
    switch (type)
    {
    case P_CHAR:
        return (1);
    case P_INT:
        return (4);
    case P_LONG:
        return (8);
    default:
        custom_error_int("Bad type in cgprimsize:", type);
    }
    return (0); // Keep -Wall happy
}

// Generate a global symbol
void cgglobsym(int id)
{
    int typesize;

    if (Gsym[id].stype == S_FUNCTION)
        return;
    // Get the size of the type
    typesize = cgprimsize(Gsym[id].type);
    // Generate the global identity and the label
    cgdataseg();
    fprintf(Outfile, "\t.globl\t%s\n", Gsym[id].name);
    fprintf(Outfile, "%s:", Gsym[id].name);

    for (int i = 0; i < Gsym[id].size; i++)
    {
        switch (typesize)
        {
        case 1:
            fprintf(Outfile, "\t.byte\t0\n");
            break;
        case 4:
            fprintf(Outfile, "\t.long\t0\n");
            break;
        case 8:
            fprintf(Outfile, "\t.quad\t0\n");
            break;
        default:
            custom_error_int("Unknown typesize in cgglobsym: ", typesize);
        }
    }
}

// Generate a global string and its start label
void cgglobstr(int l, char *strvalue)
{
    char *cptr;
    cglabel(l);
    for (cptr = strvalue; *cptr; cptr++)
    {
        fprintf(Outfile, "\t.byte\t%d\n", *cptr);
    }
    fprintf(Outfile, "\t.byte\t0\n");
}

// List of comparison instructions,
// in AST order: A_EQ, A_NE, A_LT, A_GT, A_LE, A_GE
static char *cmplist[] =
    {"sete", "setne", "setl", "setg", "setle", "setge"};

// Compare two registers and set if true.
int cgcompare_and_set(AST_node_type ASTop, int r1, int r2)
{

    // Check the range of the AST operation
    if (ASTop < A_EQ || ASTop > A_GE)
        custom_error_int("Bad ASTop in cgcompare_and_set()", ASTop);

    fprintf(Outfile, "\tcmpq\t%s, %s\n", reglist[r2], reglist[r1]);
    fprintf(Outfile, "\t%s\t%s\n", cmplist[ASTop - A_EQ], breglist[r2]);
    fprintf(Outfile, "\tmovzbq\t%s, %s\n", breglist[r2], reglist[r2]);
    free_register(r1);
    return (r2);
}

// Generate a label
void cglabel(int l)
{
    fprintf(Outfile, "L%d:\n", l);
}

// Generate a jump to a label
void cgjump(int l)
{
    fprintf(Outfile, "\tjmp\tL%d\n", l);
}

// List of inverted jump instructions,
// in AST order: A_EQ, A_NE, A_LT, A_GT, A_LE, A_GE
static char *invcmplist[] = {"jne", "je", "jge", "jle", "jg", "jl"};

// Compare two registers and jump if false.
int cgcompare_and_jump(AST_node_type ASTop, int r1, int r2, int label)
{

    // Check the range of the AST operation
    if (ASTop < A_EQ || ASTop > A_GE)
        custom_error_int("Bad ASTop in cgcompare_and_set()", ASTop);

    fprintf(Outfile, "\tcmpq\t%s, %s\n", reglist[r2], reglist[r1]);
    fprintf(Outfile, "\t%s\tL%d\n", invcmplist[ASTop - A_EQ], label);
    freeall_registers();
    return (NOREG);
}

// Widen the value in the register from the old
// to the new type, and return a register with
// this new value
int cgwiden(int r, int oldtype, int newtype)
{
    // Nothing to do
    return (r);
}

// Generate code to return a value from a function
void cgreturn(int reg, int id)
{
    // Generate code depending on the function's type
    switch (Gsym[id].type)
    {
    case P_CHAR:
        fprintf(Outfile, "\tmovzbl\t%s, %%eax\n", breglist[reg]);
        break;
    case P_INT:
        fprintf(Outfile, "\tmovl\t%s, %%eax\n", dreglist[reg]);
        break;
    case P_LONG:
        fprintf(Outfile, "\tmovq\t%s, %%rax\n", reglist[reg]);
        break;
    default:
        custom_error_int("Bad function type in cgreturn:", Gsym[id].type);
    }
    // printf("Gsym[id].endlabel=%d %s\n", Gsym[id].endlabel, Gsym[id].name);
    cgjump(Gsym[id].endlabel);
}

// Generate code to load the address of a global
// identifier into a variable. Return a new register
int cgaddress(int id)
{
    int r = alloc_register();

    if (Gsym[id].class == C_LOCAL)
        fprintf(Outfile, "\tleaq\t%d(%%rbp), %s\n", Gsym[id].posn,
                reglist[r]);
    else
        fprintf(Outfile, "\tleaq\t%s(%%rip), %s\n", Gsym[id].name, reglist[r]);
    return (r);
}

// Dereference a pointer to get the value it
// pointing at into the same register
int cgderef(int r, int type)
{
    Primitive_type newtype = value_at(type);
    int size = cgprimsize(newtype);
    switch (size)
    {
    case 1:
        fprintf(Outfile, "\tmovzbq\t(%s), %s\n", reglist[r], reglist[r]);
        break;
    case 2:
        fprintf(Outfile, "\tmovslq\t(%s), %s\n", reglist[r], reglist[r]);
        break;
    case 4:
    case 8:
        fprintf(Outfile, "\tmovq\t(%s), %s\n", reglist[r], reglist[r]);
        break;
    default:
        custom_error_int("Can't cgderef on type:", type);
    }
    return (r);
}

// Store through a dereferenced pointer
int cgstorderef(int r1, int r2, int type)
{
    switch (type)
    {
    case P_CHAR:
        fprintf(Outfile, "\tmovb\t%s, (%s)\n", breglist[r1], reglist[r2]);
        break;
    case P_INT:
        fprintf(Outfile, "\tmovq\t%s, (%s)\n", reglist[r1], reglist[r2]);
        break;
    case P_LONG:
        fprintf(Outfile, "\tmovq\t%s, (%s)\n", reglist[r1], reglist[r2]);
        break;
    default:
        custom_error_int("Can't cgstoderef on type:", type);
    }
    return (r1);
}

// Given a register with an argument value,
// copy this argument into the argposn'th
// parameter in preparation for a future function
// call. Note that argposn is 1, 2, 3, 4, ..., never zero.
void cgcopyarg(int r, int argposn)
{
    if (argposn > 6)
    {
        fprintf(Outfile, "\tpushq\t%s\n", reglist[r]);
    }
    else
    {
        fprintf(Outfile, "\tmovq\t%s, %s\n", reglist[r],
                reglist[FIRSTPARAMREG - argposn + 1]);
    }
}