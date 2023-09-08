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

int code_generator_load_int(int v)
{
    int r = alloc_register();
    fprintf(Outfile, "\tmovq\t$%d, %s\n", v, register_list[r]);
    return (r);
}

int code_generator_load_global(char *identifier)
{
    int r = alloc_register();
    fprintf(Outfile, "\tmovq\t%s(\%%rip), %s\n", identifier, register_list[r]);
    return (r);
}

int code_generator_global_symbol(char *symbol)
{
    fprintf(Outfile, "\t.comm\t%s,8,8\n", symbol);
}

int code_generator_store_register(int r, char *identifier)
{
    fprintf(Outfile, "\tmovq\t%s, %s(\%%rip)\n", register_list[r], identifier);
    return r;
}

int code_generator_add(int r1, int r2)
{
    fprintf(Outfile, "\taddq\t%s, %s\n", register_list[r1], register_list[r2]);
    free_register(r1);
    return (r2);
}

int code_generator_sub(int r1, int r2)
{
    fprintf(Outfile, "\tsubq\t%s, %s\n", register_list[r2], register_list[r1]);
    free_register(r2);
    return (r1);
}

int code_generator_mul(int r1, int r2)
{
    fprintf(Outfile, "\timulq\t%s, %s\n", register_list[r1], register_list[r2]);
    free_register(r1);
    return (r2);
}

int code_generator_div(int r1, int r2)
{
    fprintf(Outfile, "\tmovq\t%s,%%rax\n", register_list[r1]);
    fprintf(Outfile, "\tcqo\n");
    fprintf(Outfile, "\tidivq\t%s\n", register_list[r2]);
    fprintf(Outfile, "\tmovq\t%%rax,%s\n", register_list[r1]);
    free_register(r2);
    return (r1);
}

int code_generator_printint(int r)
{
    fprintf(Outfile, "\tmovq\t%s, %%rdi\n", register_list[r]);
    fprintf(Outfile, "\tcall\tprintint\n");
    free_register(r);
}
