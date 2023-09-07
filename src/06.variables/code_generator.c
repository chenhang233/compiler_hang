#include "function.h"

/*
    code generator for X86-64
 */

static char *register_list[] = {"%r8", "%r9", "%r10", "%r11"};
static int free_register_list[4] = {0};

void free_all_registers()
{
    free_register_list[0] = free_register_list[1] = free_register_list[2] = free_register_list[3] = 0;
}

void free_register(int reg)
{
    if (!free_register_list[reg])
    {
        custom_error_int("Repeat release register", reg);
    }
    free_register_list[reg] = 0;
}

int alloc_register()
{
    int i;
    for (i = 0; i < 4; i++)
    {
        if (!free_register_list[i])
        {
            return i;
        }
    }
    custom_error_int("No registers are available", i);
}

void code_generator_preamble()
{
    free_all_registers();
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
          "\n"
          "\t.globl\tmain\n"
          "\t.type\tmain, @function\n"
          "main:\n"
          "\tpushq\t%rbp\n"
          "\tmovq	%rsp, %rbp\n",
          Outfile);
}

void code_generator_postamble()
{
    fputs("\tmovl	$0, %eax\n"
          "\tpopq	%rbp\n"
          "\tret\n",
          Outfile);
}
