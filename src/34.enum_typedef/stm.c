#include "function.h"
// Parse an strcut / union
static symtable *composite_declaration(Primitive_type type);
// Parse an enum declaration
static void enum_declaration(void);
static Primitive_type typedef_declaration(symtable **ctype);
static Primitive_type type_of_typedef(char *name, struct symtable **ctype);

Primitive_type parse_type(symtable **ctype)
{
    // if we have a ';' ,parsing then there is no type, so return -1
    Primitive_type type;
    // printf("Token.token=%d\n", t_instance.token);
    switch (t_instance.token)
    {
    case T_VOID:
        type = P_VOID;
        scan(&t_instance);
        break;
    case T_CHAR:
        type = P_CHAR;
        scan(&t_instance);
        break;
    case T_INT:
        type = P_INT;
        scan(&t_instance);
        break;
    case T_LONG:
        type = P_LONG;
        scan(&t_instance);
        break;
    case T_STRUCT:
        type = P_STRUCT;
        *ctype = composite_declaration(P_STRUCT);
        if (t_instance.token == T_SEMI)
            type = -1;
        break;
    case T_UNION:
        type = P_UNION;
        *ctype = composite_declaration(P_UNION);
        if (t_instance.token == T_SEMI)
            type = -1;
        break;
    case T_ENUM:
        type = P_INT; // Enums are really ints
        enum_declaration();
        if (t_instance.token == T_SEMI)
            type = -1;
        break;
    case T_TYPEDEF:
        type = typedef_declaration(ctype);
        if (t_instance.token == T_SEMI)
            type = -1;
        break;
    case T_IDENT:
        type = type_of_typedef(Text, ctype);
        break;
    default:
        custom_error_int("Illegal type, token", t_instance.token);
    }
    // printf("end %d\n", t_instance.token);
    while (1)
    {
        if (t_instance.token != T_STAR)
            break;
        type = pointer_to(type);
        scan(&t_instance);
    }
    return type;
}

// Given a typedef name, return the type it represents
static Primitive_type type_of_typedef(char *name, struct symtable **ctype)
{
    struct symtable *t;

    t = findtypedef(name);
    if (t == NULL)
        custom_error_chars("unknown type", name);
    scan(&t_instance);
    *ctype = t->ctype;
    return (t->type);
}

void global_declarations()
{
    ASTnode *tree;
    Primitive_type type;
    symtable *ctype;
    while (1)
    {
        if (t_instance.token == T_EOF)
            break;
        type = parse_type(&ctype);
        if (type == -1)
        {
            match_semi();
            continue;
        }
        match_ident();
        // printf("Text=%s\n", Text);
        if (t_instance.token == T_LPAREN)
        {
            tree = function_declaration(type);
            if (!tree)
                continue;

            if (O_dumpAST)
            {
                jsonify_tree(tree, "dump.json");
                // dumpAST
                dumpAST(tree, NOLABEL, 0);
                fprintf(stdout, "\n\n");
            }
            genAST(tree, NOLABEL, 0);
            // Free up local space
            freeloclsyms();
        }
        else
        {
            var_declaration(type, ctype, C_GLOBAL);
            match_semi();
        }
        //  printf("end t_instance.token=%d\n", t_instance.token);
    }
    Gsym_dump("gsym_demp.txt");
}

static int var_declaration_list(symtable *funcsym, Storage_class class,
                                Token_type separate_token, Token_type end_token)
{
    Primitive_type t_type;
    int param_count = 0;
    symtable *ctype;
    symtable *protoptr = NULL;

    if (funcsym != NULL)
        protoptr = funcsym->member;

    while (t_instance.token != end_token)
    {
        t_type = parse_type(&ctype);
        match_ident();
        if (protoptr)
        {
            if (t_type != protoptr->type)
                custom_error_int("type doesn't match prototype for parameter", param_count);
            protoptr = protoptr->next;
        }
        else
        {
            var_declaration(t_type, ctype, class);
        }
        param_count++;
        if ((t_instance.token != separate_token) && (t_instance.token != end_token))
            custom_error_int("Unexpected token in parameter list", t_instance.token);
        if (t_instance.token == separate_token)
            scan(&t_instance);
    }
    if (funcsym && funcsym->nelems != param_count)
    {
        custom_error_chars("Parameter count mismatch for function", funcsym->name);
    }

    return param_count;
}

static symtable *composite_declaration(Primitive_type type)
{
    symtable *ctype = NULL;
    symtable *m;
    int offset = 0;
    scan(&t_instance);
    if (t_instance.token == T_IDENT)
    {
        if (type == P_STRUCT)
            ctype = findstruct(Text);
        else
            ctype = findunion(Text);
        scan(&t_instance);
    }
    if (t_instance.token != T_LBRACE)
    {
        if (ctype == NULL)
            custom_error_chars("unknown struct type", Text);
        return ctype;
    }
    if (ctype)
        custom_error_chars("previously defined struct", Text);
    if (type == P_STRUCT)
        ctype = addstruct(Text, P_STRUCT, NULL, 0, 0);
    else
        ctype = addunion(Text, P_UNION, NULL, 0, 0);
    scan(&t_instance);
    var_declaration_list(NULL, C_MEMBER, T_SEMI, T_RBRACE);
    match_rbrace();
    ctype->member = Membhead;
    Membhead = Membtail = NULL;
    // printf("111 %p %d\n", ctype, sizeof(ctype));

    m = ctype->member;
    m->posn = 0;
    offset = typesize(m->type, m->ctype);

    for (m = m->next; m; m = m->next)
    {
        if (type == P_STRUCT)
            m->posn = genalign(m->type, offset, 1);
        else
            m->posn = 0;
        offset += typesize(m->type, m->ctype);
        // printf("m.name=%s next offset=%d\n", m->name, offset);
    }
    ctype->size = offset;
    return ctype;
}

static void enum_declaration(void)
{
    symtable *etype = NULL;
    char *name;
    int intval = 0;
    match_enum();
    if (t_instance.token == T_IDENT)
    {
        etype = findenumtype(Text);
        name = my_strdup(Text);
        match_ident();
    }
    if (t_instance.token != T_LBRACE)
    {
        if (etype == NULL)
            custom_error_chars("undeclared enum type:", name);
        return;
    }
    match_lbrace();

    if (etype)
        custom_error_chars("enum type redeclared:", etype->name);
    else
        addenum(name, C_ENUMTYPE, 0);

    while (1)
    {
        match_ident();
        name = my_strdup(Text);
        etype = findenumval(name);
        if (etype)
            custom_error_chars("enum value redeclared:", Text);
        if (t_instance.token == T_ASSIGN)
        {
            match_assign();
            if (t_instance.token != T_INTLIT)
                custom_error_chars("Expected int literal after '='", "");
            intval = t_instance.intvalue;
            scan(&t_instance);
        }
        addenum(name, C_ENUMVAL, intval++);
        if (t_instance.token == T_RBRACE)
            break;
        match_comma();
    }
    scan(&t_instance);
}

static Primitive_type typedef_declaration(struct symtable **ctype)
{
    Primitive_type type;

    scan(&t_instance);

    type = parse_type(ctype);

    if (findtypedef(Text) != NULL)
        custom_error_chars("redefinition of typedef", Text);

    addtypedef(Text, type, *ctype, 0, 0);
    scan(&t_instance);
    return (type);
}

ASTnode *function_declaration(Primitive_type type)
{
    ASTnode *tree, *final_stm;
    symtable *oldfuncsym, *newfuncsym = NULL;
    int endlabel, paramcnt;
    if ((oldfuncsym = findglob(Text)) != NULL)
        if (oldfuncsym->stype != S_FUNCTION)
            oldfuncsym = NULL;
    if (oldfuncsym == NULL)
    {
        endlabel = genlabel();
        newfuncsym = addglob(Text, type, NULL, S_FUNCTION, endlabel);
    }

    match_lparen();
    paramcnt = var_declaration_list(oldfuncsym, C_PARAM, T_COMMA, T_RPAREN);
    match_rparen();
    if (newfuncsym)
    {
        newfuncsym->nelems = paramcnt;
        newfuncsym->member = Parmhead;
        oldfuncsym = newfuncsym;
    }
    // Clear out the parameter list
    Parmhead = Parmtail = NULL;
    if (t_instance.token == T_SEMI)
    {
        match_semi();
        return NULL;
    }
    // This is not just a prototype.
    // Set the Functionid global to the function's symbol pointer
    Functionid = oldfuncsym;
    tree = compound_statement();
    if (type != P_VOID)
    {
        if (!tree)
            custom_error_int("No statements in function with non-void type", 0);
        final_stm = tree->op == A_GLUE ? tree->right : tree;
        if (!final_stm || final_stm->op != A_RETURN)
            custom_error_int("No return for function with non-void type", 0);
    }
    return mkAST_left(A_FUNCTION, type, tree, oldfuncsym, endlabel);
}

symtable *var_declaration(Primitive_type type, symtable *ctype, Storage_class class)
{
    symtable *sym = NULL;
    switch (class)
    {
    case C_GLOBAL:
        if (findglob(Text) != NULL)
            custom_error_chars("Duplicate global variable declaration", Text);
    case C_LOCAL:
    case C_PARAM:
        if (findlocl(Text) != NULL)
            custom_error_chars("Duplicate local variable declaration", Text);
    case C_MEMBER:
        if (findmember(Text) != NULL)
            custom_error_chars("Duplicate struct/union member declaration", Text);
    }
    if (t_instance.token == T_LBRACKET)
    {
        match_lbracket();
        if (t_instance.token == T_INTLIT)
        {
            switch (class)
            {
            case C_GLOBAL:
                sym =
                    addglob(Text, pointer_to(type), ctype, S_ARRAY, t_instance.intvalue);
                break;
            case C_LOCAL:
            case C_PARAM:
            case C_MEMBER:
                custom_error_int("For now, declaration of local arrays is not implemented", class);
            }
        }
        scan(&t_instance);
        match_rbracket();
    }
    else
    {
        switch (class)
        {
        case C_GLOBAL:
            sym = addglob(Text, type, ctype, S_VARIABLE, 1);
            break;
        case C_LOCAL:
            sym = addlocl(Text, type, ctype, S_VARIABLE, 1);
            break;
        case C_PARAM:
            sym = addparm(Text, type, ctype, S_VARIABLE, 1);
            break;
        case C_MEMBER:
            sym = addmemb(Text, type, ctype, S_VARIABLE, 1);
            break;
        }
    }
    return sym;
}

ASTnode *compound_statement()
{
    ASTnode *tree, *left = NULL;

    match_lbrace();
    // printf("100---%d\n", t_instance.token);
    while (1)
    {
        tree = single_statement();
        if (tree && (tree->op == A_RETURN || tree->op == A_FUNCCALL || tree->op == A_ASSIGN))
        {
            match_semi();
        }

        if (tree)
        {
            if (!left)
            {
                left = tree;
            }
            else
            {
                left = mkAST_node(A_GLUE, P_NONE, left, NULL, tree, NULL, 0);
            }
        }

        if (t_instance.token == T_RBRACE)
        {
            match_rbrace();
            return left;
        }
    }
}

static ASTnode *single_statement(void)
{
    Primitive_type type;
    symtable *ctype;
    switch (t_instance.token)
    {
    case T_CHAR:
    case T_INT:
    case T_LONG:
        type = parse_type(&ctype);
        match_ident();
        var_declaration(type, ctype, C_LOCAL);
        match_semi();
        return NULL;
    case T_IF:
        return if_statement();
    case T_WHILE:
        return while_statement();
    case T_FOR:
        return for_statement();
    case T_RETURN:
        return return_statement();
    default:
        return binexpr(0);
    }
    return NULL;
}

static ASTnode *if_statement(void)
{
    ASTnode *cond, *trueAST, *falseAST = NULL;
    match_if();
    match_lparen();
    cond = binexpr(0);
    // if (cond->op < T_EQ || cond->op > T_GE)
    //     custom_error_int("Bad comparison operator", cond->op);
    if (cond->op < A_EQ || cond->op > A_GE)
        cond = mkAST_left(A_TOBOOL, cond->type, cond, NULL, 0);
    match_rparen();

    trueAST = compound_statement();
    if (t_instance.token == T_ELSE)
    {
        scan(&t_instance);
        falseAST = compound_statement();
    }
    return mkAST_node(A_IF, P_NONE, cond, trueAST, falseAST, NULL, 0);
};
static ASTnode *while_statement()
{
    ASTnode *cond, *body;
    match_while();
    match_lparen();
    cond = binexpr(0);
    // if (cond->op < T_EQ || cond->op > T_GE)
    //     custom_error_int("Bad comparison operator", cond->op);
    if (cond->op < A_EQ || cond->op > A_GE)
        cond = mkAST_left(A_TOBOOL, cond->type, cond, NULL, 0);
    match_rparen();

    body = compound_statement();
    return mkAST_node(A_WHILE, P_NONE, cond, NULL, body, NULL, 0);
};
static ASTnode *for_statement()
{
    ASTnode *prev, *cond, *post, *body;
    ASTnode *left;
    match_for();
    match_lparen();
    prev = single_statement();
    match_semi();

    cond = binexpr(0);
    // if (cond->op < T_EQ || cond->op > T_GE)
    //     custom_error_int("Bad comparison operator", cond->op);
    if (cond->op < A_EQ || cond->op > A_GE)
        cond = mkAST_left(A_TOBOOL, cond->type, cond, NULL, 0);
    match_semi();

    post = single_statement();
    match_rparen();

    body = compound_statement();
    left = mkAST_node(A_GLUE, P_NONE, body, NULL, post, NULL, 0);
    left = mkAST_node(A_WHILE, P_NONE, cond, NULL, left, NULL, 0);
    left = mkAST_node(A_GLUE, P_NONE, prev, NULL, left, NULL, 0);
    return left;
};
static ASTnode *return_statement(void)
{
    ASTnode *tree;
    Primitive_type need_type = Functionid->type;
    if (need_type == P_VOID)
        custom_error_int("can not return from a void function", 1);
    match_return();
    match_lparen();
    tree = binexpr(0);

    tree = modify_type(tree, need_type, 0);
    if (!tree)
        custom_error_int("Incompatible type to return", 0);
    tree = mkAST_left(A_RETURN, tree->type, tree, NULL, 0);
    match_rparen();
    return tree;
};