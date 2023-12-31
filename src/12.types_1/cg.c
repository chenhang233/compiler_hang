#include "function.h"

static int freereg[4];
static char *reglist[4] = {"%r8", "%r9", "%r10", "%r11"};
static char *breglist[4] = {"%r8b", "%r9b", "%r10b", "%r11b"};

void freeall_registers(void)
{
    freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
}

static void free_register(int reg)
{
    if (reg >= 4)
    {
        custom_error_int("free_register", reg);
    }
    if (freereg[reg] != 0)
        custom_error_int("Error trying to free register", reg);
    freereg[reg] = 1;
}

static int alloc_register(void)
{
    for (int i = 0; i < 4; i++)
    {
        if (freereg[i])
        {
            freereg[i] = 0;
            return (i);
        }
    }
    custom_error_int("Out of registers", -2);
}

void cgpreamble()
{
    freeall_registers();
    fputs("\t.text\n"
          ".LC0:\n"
          "\t.string\t\"%d\\n\"\n"
          "printint:\n"
          "\tpushq\t%rbp\n"
          "\tmovq\t%rsp, %rbp\n"
          "\tsubq\t$16, %rsp\n"
          "\tmovl\t%edi, -4(%rbp)\n"
          "\tmovl\t-4(%rbp), %eax\n"
          "\tmovl\t%eax, %esi\n"
          "\tleaq	.LC0(%rip), %rdi\n"
          "\tmovl	$0, %eax\n"
          "\tcall	printf@PLT\n"
          "\tnop\n"
          "\tleave\n"
          "\tret\n"
          "\n",
          Outfile);
}

void cgfuncpreamble(char *name)
{
    fprintf(Outfile,
            "\t.text\n"
            "\t.globl\t%s\n"
            "\t.type\t%s, @function\n"
            "%s:\n"
            "\tpushq\t%%rbp\n"
            "\tmovq\t%%rsp, %%rbp\n",
            name, name, name);
}

void cgfuncpostamble()
{
    fputs("\tmovl	$0, %eax\n"
          "\tpopq	%rbp\n"
          "\tret\n",
          Outfile);
}

void cgglobsym(int id)
{
    // Choose P_INT or P_CHAR
    if (Gsym[id].type == P_INT)
        fprintf(Outfile, "\t.comm\t%s,8,8\n", Gsym[id].name);
    else
        fprintf(Outfile, "\t.comm\t%s,1,1\n", Gsym[id].name);
}

int cgloadint(int value, int type)
{
    // Get a new register
    int r = alloc_register();

    fprintf(Outfile, "\tmovq\t$%d, %s\n", value, reglist[r]);
    return (r);
}

int cgloadglob(int id)
{
    // Get a new register
    int r = alloc_register();

    // Print out the code to initialise it: P_CHAR or P_INT
    if (Gsym[id].type == P_INT)
        fprintf(Outfile, "\tmovq\t%s(\%%rip), %s\n", Gsym[id].name, reglist[r]);
    else
        fprintf(Outfile, "\tmovzbq\t%s(\%%rip), %s\n", Gsym[id].name, reglist[r]);
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

void cgprintint(int r)
{
    fprintf(Outfile, "\tmovq\t%s, %%rdi\n", reglist[r]);
    fprintf(Outfile, "\tcall\tprintint\n");
    free_register(r);
}

int cgstorglob(int r, int id)
{
    // Choose P_INT or P_CHAR
    if (Gsym[id].type == P_INT)
        fprintf(Outfile, "\tmovq\t%s, %s(\%%rip)\n", reglist[r], Gsym[id].name);
    else
        fprintf(Outfile, "\tmovb\t%s, %s(\%%rip)\n", breglist[r], Gsym[id].name);
    return (r);
}

// List of comparison instructions,
// in AST order: A_EQ, A_NE, A_LT, A_GT, A_LE, A_GE
static char *cmplist[] =
    {"sete", "setne", "setl", "setg", "setle", "setge"};

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
