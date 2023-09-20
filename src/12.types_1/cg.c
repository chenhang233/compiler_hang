#include "function.h"

static int freereg[4];
static char *reglist[4] = {"%r8", "%r9", "%r10", "%r11"};
static char *breglist[4] = {"%r8b", "%r9b", "%r10b", "%r11b"};

void freeall_registers(void)
{
    freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
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
    fatal("Out of registers");
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

void cgprintint(int r)
{
    fprintf(Outfile, "\tmovq\t%s, %%rdi\n", reglist[r]);
    fprintf(Outfile, "\tcall\tprintint\n");
    free_register(r);
}

void cgglobsym(int id)
{
    // Choose P_INT or P_CHAR
    if (Gsym[id].type == P_INT)
        fprintf(Outfile, "\t.comm\t%s,8,8\n", Gsym[id].name);
    else
        fprintf(Outfile, "\t.comm\t%s,1,1\n", Gsym[id].name);
}