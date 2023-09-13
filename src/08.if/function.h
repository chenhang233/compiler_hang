#include "data.h"

// custom_error.c
void custom_error_line();
void custom_error_char(char *, char);
void custom_error_chars(char *, char *);
void custom_error_int(char *, int);
void match(AST_type, char *);
void match_semi();
void match_lbrace();
void match_rbrace();
void match_lparen();
void match_rparen();
void match_ident();
void match_int();
void match_assign();

// cg.c
void cgpreamble();
void cgpostamble();
int cgloadint(int value);
int cgloadglob(char *identifier);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);
int cgstorglob(int r, char *identifier);
void cgglobsym(char *sym);
int cgcompare_and_set(int ASTop, int r1, int r2);
void cglabel(int l);
void cgjump(int l);
int cgcompare_and_jump(int ASTop, int r1, int r2, int label);

// gen.c
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int);
void genglobsym(char *);

// scan.c
int scan(Token *);
int skip();
int next();
int scanint(int);
int int_pos(char *, int);
int scanident(int, char *, int);
int keyword(char *);

// stm.c
AST_node *compound_statement(void);
AST_node *print_statement(void);
AST_node *assignment_statement(void);
AST_node *if_statement(void);
void var_declaration(void);

// ast.c
AST_node *mkAST_node(AST_type, AST_node *, AST_node *, AST_node *, int);
AST_node *mkAST_leaf(AST_type, int);
AST_node *parse_ast_expr(int);
AST_node *primary();
static int op_precedence(Token_type);
static AST_type arithop(Token_type);
// sym.c
int findglob(char *);
static int newglob();
int addglob(char *);
char *my_strdup(const char *);