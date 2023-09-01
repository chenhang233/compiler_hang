#include "data.h"

void usage(char *perf, char const *info)
{
    printf("%s ERROR: %s\n", perf, info);
    printf("Line: %d\n", Line);
    exit(1);
}

int next()
{
    int c;
    if (InfileCache)
    {
        c = InfileCache;
        InfileCache = 0;
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
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t')
    {
        c = next();
    }
    return c;
}

int strPosInt(char *s, char c)
{
    char *p = strchr(s, c);
    return p ? p - s : -1;
}

int scanDigit(int c)
{
    int t, v = 0;
    while ((t = strPosInt("0123456789", c)) >= 0)
    {
        v = v * 10 + t;
        c = next();
    }
    InfileCache = c;
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
        t->token = T_MUT;
        break;
    case '/':
        t->token = T_DIV;
        break;
    default:
        if (isdigit(c))
        {
            t->token = T_INTLIT;
            t->intValue = scanDigit(c);
            break;
        }
        usage("Invalid character", (char *)&c);
    }
    return 1;
}

void init()
{
    InfileCache = '\n';
    Line = 1;
    for (int i = 0; i < REG_LEN; i++)
    {
        RegisterStatus[i] = 0;
    }
}

ASTnode *parseInt()
{
    switch (T.token)
    {
    case T_INTLIT:
        return mkASTleaf(A_INTLIT, T.intValue);
    default:
        usage("Invalid Tnt", (char *)&T.token);
    }
}

int calcScore(int i)
{
    int score = tokenWeights[i];
    if (score == 0)
    {
        usage("calcScore", "");
    }
}

ASTtype transType(TokenType tt)
{
    switch (tt)
    {
    case T_ADD:
        return A_ADD;
    case T_SUB:
        return A_SUB;
    case T_MUT:
        return A_MUT;
    case T_DIV:
        return A_DIV;
    default:
        usage("transType", NULL);
    }
}

// ASTnode *parseAST(int prev)
// {
//     ASTnode *l, *r;
//     l = parseInt();
//     scan(&T);
//     if (T.token == T_EOF)
//         return l;
//     while (calcScore(T.token) > prev)
//     {
//         scan(&T);
//         r = parseAST(tokenWeights[T.token]);
//         l = mkASTnode(transType(T.token), l, r, 0);
//         if (T.token == T_EOF)
//             break;
//     }
//     return l;
// }

ASTnode *parseAST(int prev)
{
    ASTnode *l, *r;
    l = parseInt();
    scan(&T);
    TokenType token = T.token;
    if (token == T_EOF)
        return l;
    while (calcScore(token) > prev)
    {
        scan(&T);
        r = parseAST(tokenWeights[token]);
        l = mkASTnode(transType(token), l, r, 0);
        token = T.token;
        if (token == T_EOF)
            break;
    }
    return l;
}

ASTnode *mkASTnode(ASTtype op, struct ASTnode *l, struct ASTnode *r, int v)
{
    ASTnode *root = malloc(sizeof(ASTnode *));
    if (root == NULL)
    {
        usage("malloc", "NULL");
    }
    root->l = l;
    root->r = r;
    root->op = op;
    root->v = v;
    return root;
}

ASTnode *mkASTleaf(ASTtype t, int v)
{
    return mkASTnode(t, NULL, NULL, v);
}

int interpretAST(ASTnode *n)
{
    int l, r;
    if (n->l)
        l = interpretAST(n->l);
    if (n->r)
        r = interpretAST(n->r);
    switch (n->op)
    {
    case A_INTLIT:
        return n->v;
    case A_ADD:
        return l + r;
    case A_SUB:
        return l - r;
    case A_MUT:
        return l * r;
    case A_DIV:
        return l / r;
    default:
        usage("interpretAST", NULL);
    }
}

void RegisterFree(int i)
{
    if (!RegisterStatus[i])
    {
        usage("trying to free register", "");
    }
    RegisterStatus[i] = 0;
}

int RegisterAlloc()
{
    for (int i = 0; i < REG_LEN; i++)
    {
        if (!RegisterStatus[i])
        {
            RegisterStatus[i] = 1;
            return i;
        }
    }
    usage("Out of registers", "");
}

void cgpreamble()
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

void cgpostamble()
{
    fputs(
        "\tmovl	$0, %eax\n"
        "\tpopq	%rbp\n"
        "\tret\n",
        Outfile);
}

void callprintint(int i)
{
    fprintf(Outfile, "\tmovq\t%s, %%rdi\n", RegisterList[i]);
    fprintf(Outfile, "\tcall\tprintint\n");
}

int callLoad(int v)
{
    int i = RegisterAlloc();
    fprintf(Outfile, "\tmovq\t$%d, %s\n", v, RegisterList[i]);
    return i;
};
int callAdd(int i1, int i2)
{
    fprintf(Outfile, "\taddq\t%s, %s\n", RegisterList[i1], RegisterList[i2]);
    RegisterFree(i1);
    return i2;
};
int callSub(int i1, int i2)
{
    fprintf(Outfile, "\tsubq\t%s, %s\n", RegisterList[i1], RegisterList[i2]);
    RegisterFree(i2);
    return i1;
};
int callMut(int i1, int i2)
{
    fprintf(Outfile, "\timulq\t%s, %s\n", RegisterList[i1], RegisterList[i2]);
    RegisterFree(i1);
    return i2;
};
int callDiv(int i1, int i2)
{
    fprintf(Outfile, "\tmovq\t%s, %%rax\n", RegisterList[i1]);
    fprintf(Outfile, "\tcqo\n");
    fprintf(Outfile, "\tidivq, %s\n", RegisterList[i2]);
    fprintf(Outfile, "\tmovq\t%%rax, %s\n", RegisterList[i1]);
    RegisterFree(i2);
    return i1;
};

int generateAST(struct ASTnode *n)
{
    int leftReg, rightReg;
    if (n->l)
    {
        leftReg = generateAST(n->l);
    }
    if (n->r)
    {
        rightReg = generateAST(n->r);
    }
    switch (n->op)
    {
    case A_INTLIT:
        return callLoad(n->v);
    case A_ADD:
        return callAdd(leftReg, rightReg);
    case A_SUB:
        return callSub(leftReg, rightReg);
    case A_MUT:
        return callMut(leftReg, rightReg);
    case A_DIV:
        return callDiv(leftReg, rightReg);
    default:
        usage("Unknown AST operator", NULL);
    }
}

void generatecode(ASTnode *n)
{
    cgpreamble();
    int reg = generateAST(n);
    callprintint(reg);
    cgpostamble();
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        usage("argc length", argv[0]);
    }
    if (!(Infile = fopen(argv[1], "r")))
    {
        usage("open Infile", argv[1]);
    }
    if (!(Outfile = fopen("out.s", "w")))
    {
        usage("open Outfile", "out.s");
    }
    scan(&T);
    ASTnode *n = parseAST(0);
    printf("%d\n", interpretAST(n));
    generatecode(n);
    fclose(Outfile);
    return 0;
}
