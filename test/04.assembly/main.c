#include "data.h"

void usage(char *ei)
{
    printf("ERROR: %s, source code line: %d\n", ei, Line);
    exit(1);
}

void init()
{
    Line = 1;
    Input_cache = '\n';
}

int next()
{
    int c;
    if (Input_cache)
    {
        c = Input_cache;
        Input_cache = 0;
        return c;
    }
    c = fgetc(Infile);
    if (c == '\n')
    {
        Line++;
    }
    return c;
}

int skip()
{
    int c = next();
    while (c == '\n' || c == ' ' || c == '\r' || c == '\t')
    {
        c = next();
    }
    return c;
}

int str_digit_index(char *s, char c)
{
    char *p = strchr(s, c);
    return p ? p - s : -1;
}

int scan_digit(int c)
{
    int t, v = 0;
    while ((t = str_digit_index("0123456789", c)) >= 0)
    {
        v = v * 10 + t;
        c = next();
    }
    Input_cache = c;
    return v;
}

int scan(Token *t)
{
    int c = skip();
    switch (c)
    {
    case EOF:
        t->token = T_EOF;
        return 0;
    case '+':
        t->token = T_ADD;
        break;
    case '-':
        t->token = T_SUB;
        break;
    case '*':
        t->token = T_MUL;
        break;
    case '/':
        t->token = T_DIV;
        break;
    default:
        if (isdigit(c))
        {
            t->token = T_INTLIT;
            t->v = scan_digit(c);
            break;
        }
        usage("Unknown character");
    }
    return 1;
}

AST_node *mk_AST_node(AST_type op, AST_node *l, AST_node *r, int v)
{
    AST_node *node = malloc(sizeof(AST_node));
    node->l = l;
    node->r = r;
    node->op = op;
    node->v = v;
    return node;
}

AST_node *mk_AST_leaf(AST_type op, int v)
{
    return mk_AST_node(op, NULL, NULL, v);
}

AST_node *parse_INTLIT()
{
    switch (T.token)
    {
    case T_INTLIT:
        return mk_AST_leaf(A_INTLIT, T.v);
    default:
        usage("A number is needed, but something else is entered");
    }
}

int get_priority_symbol(Token_type i)
{
    int priority = priority_symbol[i];
    if (priority == 0)
    {
        printf("i=%d\n", i);
        usage("Need operator");
    }
    return priority;
}
AST_type get_AST_type(Token_type t)
{
    switch (t)
    {
    case T_ADD:
        return A_ADD;
    case T_SUB:
        return A_SUB;
    case T_MUL:
        return A_MUL;
    case T_DIV:
        return A_DIV;
    default:
        usage("get_AST_type");
    }
}

AST_node *parse_AST(int prev)
{
    AST_node *l, *r;
    Token_type t;
    l = parse_INTLIT();
    scan(&T);
    t = T.token;
    if (t == T_EOF)
    {
        return l;
    }
    while (get_priority_symbol(t) > prev)
    {
        scan(&T);
        r = parse_AST(priority_symbol[t]);
        l = mk_AST_node(get_AST_type(t), l, r, 0);
        t = T.token;
        if (t == T_EOF)
        {
            break;
        }
    }
    return l;
}

int interpret_AST(AST_node *n)
{
    int l, r;
    if (n->l)
        l = interpret_AST(n->l);
    if (n->r)
        r = interpret_AST(n->r);
    switch (n->op)
    {
    case A_INTLIT:
        return n->v;
    case A_ADD:
        return l + r;
    case A_SUB:
        return l - r;
    case A_MUL:
        return l * r;
    case A_DIV:
        return l / r;
    default:
        usage("interpret_AST");
    }
}

void generate_before()
{
    fputs(
        "\t.text\n"
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

int register_alloc()
{
    for (int i = 0; i < REG_LEN; i++)
    {
        if (!allocate_register_free_lock[i])
        {
            allocate_register_free_lock[i] = 1;
            return i;
        }
    }
    usage("no registers available");
}

int register_free(int reg)
{
    if (!allocate_register_free_lock[reg])
    {
        usage("free unused register");
    }
    allocate_register_free_lock[reg] = 0;
}

int generate_ast(AST_node *ast)
{
    int l_reg, r_reg;
    if (ast->l)
        l_reg = generate_ast(ast->l);
    if (ast->r)
        r_reg = generate_ast(ast->r);
    switch (ast->op)
    {
    case A_INTLIT:
        return call_load(ast->v);
    case A_ADD:
        return call_add(l_reg, r_reg);
    case A_SUB:
        return call_sub(l_reg, r_reg);
    case A_MUL:
        return call_mul(l_reg, r_reg);
    case A_DIV:
        return call_div(l_reg, r_reg);
    default:
        break;
    }
    usage("uncatch generate_ast()");
}

void generate_print_int(int reg)
{
    fprintf(Outfile, "\tmovq\t%s, %%rdi\n", allocate_register_arr[reg]);
    fprintf(Outfile, "\tcall\tprintint\n");
}

void generate_after()
{
    fputs(
        "\tmovl	$0, %eax\n"
        "\tpopq	%rbp\n"
        "\tret\n",
        Outfile);
}

int call_load(int v)
{
    int reg = register_alloc();
    fprintf(Outfile, "\tmovq\t%d, %s\n", v, allocate_register_arr[reg]);
    return reg;
}

int call_add(int reg_a, int reg_b)
{
    fprintf(Outfile, "\taddq\t%s, %s\n", allocate_register_arr[reg_a], allocate_register_arr[reg_b]);
    register_free(reg_a);
    return reg_b;
}

int call_sub(int reg_a, int reg_b)
{
    fprintf(Outfile, "\tsubq\t%s, %s\n", allocate_register_arr[reg_a], allocate_register_arr[reg_b]);
    register_free(reg_b);
    return reg_a;
}

int call_mul(int reg_a, int reg_b)
{
    fprintf(Outfile, "\timulq\t%s, %s\n", allocate_register_arr[reg_a], allocate_register_arr[reg_b]);
    register_free(reg_a);
    return reg_b;
}

int call_div(int reg_a, int reg_b)
{
    fprintf(Outfile, "\tmovq\t%s, %%rax\n", allocate_register_arr[reg_a]);
    fprintf(Outfile, "\tcqo\n");
    fprintf(Outfile, "\tidivq,\t%s\n", allocate_register_arr[reg_b]);
    fprintf(Outfile, "\tmovq\t%%rax, %s\n", allocate_register_arr[reg_a]);
    register_free(reg_b);
    return reg_a;
}

void generate_assembly(AST_node *ast)
{
    generate_before();
    int reg = generate_ast(ast);
    generate_print_int(reg);
    generate_after();
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        usage("argc length");
    }
    if (!(Infile = fopen(argv[1], "r")))
    {
        usage("open infile");
    }
    if (!(Outfile = fopen("out.s", "w")))
    {
        usage("open Outfile");
    }
    init();
    scan(&T);
    AST_node *ast = parse_AST(0);
    printf("%d\n", interpret_AST(ast));
    generate_assembly(ast);

    fclose(Outfile);
    return 0;
}
