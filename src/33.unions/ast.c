#include "function.h"

ASTnode *mkAST_node(AST_node_type op, Primitive_type type,
                    ASTnode *left, ASTnode *mid, ASTnode *right,
                    symtable *sym, int intv)
{
    ASTnode *n = malloc(sizeof(ASTnode));
    if (!n)
        custom_error_char("unable malloc", 0);
    n->op = op;
    n->type = type;
    n->left = left;
    n->right = right;
    n->mid = mid;
    n->sym = sym;
    n->v.intvalue = intv;
    return n;
};
ASTnode *mkAST_leaf(AST_node_type op, Primitive_type type, symtable *sym, int id)
{
    return mkAST_node(op, type, NULL, NULL, NULL, sym, id);
};
ASTnode *mkAST_left(AST_node_type op, Primitive_type type, ASTnode *left,
                    symtable *sym, int v)
{
    return mkAST_node(op, type, left, NULL, NULL, sym, v);
};

static int OpPrec[] = {
    0, 10, 20, 30,  // T_EOF, T_ASSIGN, T_LOGOR, T_LOGAND
    40, 50, 60,     // T_OR, T_XOR, T_AMPER
    70, 70,         // T_EQ, T_NE
    80, 80, 80, 80, // T_LT, T_GT, T_LE, T_GE
    90, 90,         // T_LSHIFT, T_RSHIFT
    100, 100,       // T_PLUS, T_MINUS
    110, 110        // T_STAR, T_SLASH
};

static int op_precedence(Token_type tokentype)
{
    if (tokentype > T_SLASH)
        custom_error_int("Token with no precedence in op_precedence:", tokentype);
    int prec = OpPrec[tokentype];
    if (prec == 0)
        custom_error_int("op_precedence Syntax error, token", tokentype);
    return (prec);
}

static int rightassoc(Token_type tokentype)
{
    if (tokentype == T_ASSIGN)
        return (1);
    return (0);
}
//  binary ast operator
static AST_node_type binastop(Token_type tokentype)
{
    if (tokentype > T_EOF && tokentype < T_INTLIT)
        return (tokentype);
    custom_error_int("binastop Syntax error, token", tokentype);
}
// binary operator.
ASTnode *binexpr(int ptp)
{
    ASTnode *left, *right;
    ASTnode *l_temp, *r_temp;
    AST_node_type ast_type;
    left = prefix();
    // printf("v=%d %d\n", left->v.intvalue, left->op);
    Token_type type = t_instance.token;
    if (type == T_SEMI || type == T_RPAREN || type == T_RBRACKET || type == T_COMMA)
    {
        left->rvalue = 1;
        return left;
    }
    while (op_precedence(type) > ptp || (rightassoc(type) && op_precedence(type) == ptp))
    {
        scan(&t_instance);
        right = binexpr(OpPrec[type]);
        ast_type = binastop(type);
        if (ast_type == A_ASSIGN)
        {
            right->rvalue = 1;
            right = modify_type(right, left->type, 0);
            if (!right)
                custom_error_char("Incompatible expression in assignment", 0);
            l_temp = left;
            left = right;
            right = l_temp;
        }
        else
        {
            left->rvalue = 1;
            right->rvalue = 1;
            l_temp = modify_type(left, right->type, ast_type);
            r_temp = modify_type(right, left->type, ast_type);
            if (!l_temp && !r_temp)
            {
                custom_error_int("Incompatible types in binary expression", 0);
            }
            if (l_temp)
                left = l_temp;
            if (r_temp)
                right = r_temp;
        }
        left = mkAST_node(binastop(type), left->type, left, NULL, right, NULL, 0);
        type = t_instance.token;
        if (type == T_SEMI || type == T_RPAREN || type == T_RBRACKET || type == T_COMMA)
        {
            left->rvalue = 1;
            return left;
        }
    }
    left->rvalue = 1;
    return left;
}

static ASTnode *array_access(void)
{
    ASTnode *left, *right;
    symtable *aryptr;
    if ((aryptr = findsymbol(Text)) == NULL || aryptr->stype != S_ARRAY)
        custom_error_chars("Undeclared array", Text);
    left = mkAST_leaf(A_ADDR, aryptr->type, aryptr, 0);
    match_lbracket();
    right = binexpr(0);
    match_rbracket();
    if (!inttype(right->type))
        custom_error_int("Array index is not of integer type", right->type);

    right = modify_type(right, left->type, A_ADD);

    left = mkAST_node(A_ADD, aryptr->type, left, NULL, right, NULL, 0);
    left = mkAST_left(A_DEREF, value_at(left->type), left, NULL, 0);
    return left;
}
// withpointer  0: . or 1: ->
static ASTnode *member_access(int withpointer)
{
    ASTnode *left, *right;
    symtable *compvar;
    symtable *typeptr;
    symtable *m;

    if ((compvar = findsymbol(Text)) == NULL)
        custom_error_chars("Undeclared variable", Text);
    if (withpointer && compvar->type != pointer_to(P_STRUCT))
        custom_error_chars("Undeclared variable", Text);
    if (!withpointer && compvar->type != P_STRUCT)
        custom_error_chars("Undeclared variable", Text);

    if (withpointer)
        left = mkAST_leaf(A_IDENT, pointer_to(P_STRUCT), compvar, 0);
    else
        left = mkAST_leaf(A_ADDR, P_STRUCT, compvar, 0);
    left->rvalue = 1;
    typeptr = compvar->ctype;

    scan(&t_instance);
    match_ident();

    for (m = typeptr->member; m != NULL; m = m->next)
        if (!strcmp(m->name, Text))
            break;
    if (m == NULL)
        custom_error_chars("No member found in struct/union: ", Text);
    right = mkAST_leaf(A_INTLIT, P_INT, NULL, m->posn);
    left = mkAST_node(A_ADD, pointer_to(m->type), left, NULL, right, NULL, 0);
    left = mkAST_left(A_DEREF, m->type, left, NULL, 0);
    return left;
}

static ASTnode *postfix(void)
{
    ASTnode *n;
    symtable *varptr;
    scan(&t_instance);
    if (t_instance.token == T_LPAREN)
        return funccall();
    if (t_instance.token == T_LBRACKET)
        return array_access();

    // Access into a struct or union
    if (t_instance.token == T_DOT)
        return (member_access(0));
    if (t_instance.token == T_ARROW)
        return (member_access(1));

    varptr = findsymbol(Text);
    if (varptr == NULL || varptr->stype != S_VARIABLE)
        custom_error_chars("unknown variable", Text);

    switch (t_instance.token)
    {
    case T_INC:
        scan(&t_instance);
        n = mkAST_leaf(A_POSTINC, varptr->type, varptr, 0);
        break;
    case T_DEC:
        scan(&t_instance);
        n = mkAST_leaf(A_POSTDEC, varptr->type, varptr, 0);
        break;
    default:
        n = mkAST_leaf(A_IDENT, varptr->type, varptr, 0);
    }
    return n;
}

static ASTnode *primary(void)
{
    ASTnode *n;
    int v, id;
    // printf("t_instance.token=%d\n", t_instance.token);

    switch (t_instance.token)
    {
    case T_INTLIT:
        v = t_instance.intvalue;
        if (v >= 0 && v < 256)
            n = mkAST_leaf(A_INTLIT, P_CHAR, NULL, v);
        else
            n = mkAST_leaf(A_INTLIT, P_INT, NULL, v);
        break;
    case T_STRLIT:
        id = genglobstr(Text);
        n = mkAST_leaf(A_STRLIT, pointer_to(P_CHAR), NULL, id);
        break;
    case T_IDENT:
        return postfix();
    case T_LPAREN:
        match_lparen();
        n = binexpr(0);
        match_rparen();
        return n;
    default:
        custom_error_int("primary syntax error, token: ", t_instance.token);
    }
    scan(&t_instance);
    return n;
}

ASTnode *prefix(void)
{
    ASTnode *tree;
    Primitive_type type;
    // printf("t_instance.token=%d\n", t_instance.token);
    switch (t_instance.token)
    {
    case T_AMPER:
        scan(&t_instance);
        tree = prefix();
        if (tree->op != A_IDENT && tree->op != A_ADDR)
            custom_error_int("Get an address that is not an A_IDENT", tree->op);
        tree->op = A_ADDR;
        tree->type = pointer_to(tree->type);
        break;
    case T_STAR:
        scan(&t_instance);
        tree = prefix();
        if (tree->op != A_IDENT && tree->op != A_DEREF)
            custom_error_int("Get an address that is not an A_IDENT", tree->op);
        tree = mkAST_left(A_DEREF, value_at(tree->type), tree, NULL, 0);
        break;
    case T_MINUS:
        scan(&t_instance);
        tree = prefix();
        tree->rvalue = 1;
        tree = modify_type(tree, P_INT, 0);
        tree = mkAST_left(A_NEGATE, tree->type, tree, NULL, 0);
        break;
    case T_INVERT:
        scan(&t_instance);
        tree = prefix();
        tree->rvalue = 1;
        tree = mkAST_left(A_INVERT, tree->type, tree, NULL, 0);
        break;
    case T_LOGNOT:
        scan(&t_instance);
        tree = prefix();
        tree->rvalue = 1;
        tree = mkAST_left(A_LOGNOT, tree->type, tree, NULL, 0);
        break;
    case T_INC:
        scan(&t_instance);
        tree = prefix();
        if (tree->op != A_IDENT)
            custom_error_int("operator must be followed by an identifier", tree->op);
        tree = mkAST_left(A_PREINC, tree->type, tree, NULL, 0);
        break;
    case T_DEC:
        scan(&t_instance);
        tree = prefix();
        if (tree->op != A_IDENT)
            custom_error_int("operator must be followed by an identifier", tree->op);
        tree = mkAST_left(A_PREDEC, tree->type, tree, NULL, 0);
        break;
    default:
        tree = primary();
    }
    return tree;
}

static ASTnode *expression_list(void)
{
    ASTnode *child = NULL;
    ASTnode *tree = NULL;
    int expr_count = 0;
    while (t_instance.token != T_RPAREN)
    {
        child = binexpr(0);
        expr_count++;
        tree = mkAST_node(A_GLUE, P_NONE, tree, NULL, child, NULL, expr_count);
        switch (t_instance.token)
        {
        case T_COMMA:
            match_comma();
            break;
        case T_RPAREN:
            break;
        default:
            custom_error_char("Unexpected token in expression list", t_instance.token);
        }
    }
    return tree;
}

ASTnode *funccall(void)
{
    ASTnode *tree;
    symtable *funcptr;
    if ((funcptr = findsymbol(Text)) == NULL || funcptr->stype != S_FUNCTION)
        custom_error_chars("undefined function", Text);

    match_lparen();
    tree = expression_list();
    tree = mkAST_left(A_FUNCCALL, funcptr->type, tree, funcptr, 0);
    match_rparen();

    return tree;
}

static int gendumplabel(void)
{
    static int id = 1;
    return (id++);
}

void dumpAST(struct ASTnode *n, int label, int level)
{
    int Lfalse, Lstart, Lend;

    switch (n->op)
    {
    case A_IF:
        Lfalse = gendumplabel();
        for (int i = 0; i < level; i++)
            fprintf(stdout, " ");
        fprintf(stdout, "A_IF");
        if (n->right)
        {
            Lend = gendumplabel();
            fprintf(stdout, ", end L%d", Lend);
        }
        fprintf(stdout, "\n");
        dumpAST(n->left, Lfalse, level + 2);
        dumpAST(n->mid, NOLABEL, level + 2);
        if (n->right)
            dumpAST(n->right, NOLABEL, level + 2);
        return;
    case A_WHILE:
        Lstart = gendumplabel();
        for (int i = 0; i < level; i++)
            fprintf(stdout, " ");
        fprintf(stdout, "A_WHILE, start L%d\n", Lstart);
        Lend = gendumplabel();
        dumpAST(n->left, Lend, level + 2);
        dumpAST(n->right, NOLABEL, level + 2);
        return;
    }

    // Reset level to -2 for A_GLUE
    if (n->op == A_GLUE)
        level = -2;

    // General AST node handling
    if (n->left)
        dumpAST(n->left, NOLABEL, level + 2);
    if (n->right)
        dumpAST(n->right, NOLABEL, level + 2);

    for (int i = 0; i < level; i++)
        fprintf(stdout, " ");
    switch (n->op)
    {
    case A_GLUE:
        fprintf(stdout, "\n\n");
        return;
    case A_FUNCTION:
        fprintf(stdout, "A_FUNCTION %s\n", n->sym->name);
        return;
    case A_ADD:
        fprintf(stdout, "A_ADD\n");
        return;
    case A_SUBTRACT:
        fprintf(stdout, "A_SUBTRACT\n");
        return;
    case A_MULTIPLY:
        fprintf(stdout, "A_MULTIPLY\n");
        return;
    case A_DIVIDE:
        fprintf(stdout, "A_DIVIDE\n");
        return;
    case A_EQ:
        fprintf(stdout, "A_EQ\n");
        return;
    case A_NE:
        fprintf(stdout, "A_NE\n");
        return;
    case A_LT:
        fprintf(stdout, "A_LE\n");
        return;
    case A_GT:
        fprintf(stdout, "A_GT\n");
        return;
    case A_LE:
        fprintf(stdout, "A_LE\n");
        return;
    case A_GE:
        fprintf(stdout, "A_GE\n");
        return;
    case A_INTLIT:
        fprintf(stdout, "A_INTLIT %d\n", n->v.intvalue);
        return;
    case A_STRLIT:
        fprintf(stdout, "A_STRLIT rval label L%d\n", n->v.id);
        return;
    case A_IDENT:
        if (n->rvalue)
            fprintf(stdout, "A_IDENT rval %s\n", n->sym->name);
        else
            fprintf(stdout, "A_IDENT %s\n", n->sym->name);
        return;
    case A_ASSIGN:
        fprintf(stdout, "A_ASSIGN\n");
        return;
    case A_WIDEN:
        fprintf(stdout, "A_WIDEN\n");
        return;
    case A_RETURN:
        fprintf(stdout, "A_RETURN\n");
        return;
    case A_FUNCCALL:
        fprintf(stdout, "A_FUNCCALL %s\n", n->sym->name);
        return;
    case A_ADDR:
        fprintf(stdout, "A_ADDR %s\n", n->sym->name);
        return;
    case A_DEREF:
        if (n->rvalue)
            fprintf(stdout, "A_DEREF rval\n");
        else
            fprintf(stdout, "A_DEREF\n");
        return;
    case A_SCALE:
        fprintf(stdout, "A_SCALE %d\n", n->v.size);
        return;
    case A_PREINC:
        fprintf(stdout, "A_PREINC %s\n", n->sym->name);
        return;
    case A_PREDEC:
        fprintf(stdout, "A_PREDEC %s\n", n->sym->name);
        return;
    case A_POSTINC:
        fprintf(stdout, "A_POSTINC\n");
        return;
    case A_POSTDEC:
        fprintf(stdout, "A_POSTDEC\n");
        return;
    case A_NEGATE:
        fprintf(stdout, "A_NEGATE\n");
        return;
    default:
        custom_error_int("Unknown dumpAST operator", n->op);
    }
}