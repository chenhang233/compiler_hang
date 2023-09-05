#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define REG_LEN 4
#define TEXTLEN 512

typedef enum Token_type
{
    T_EOF,
    T_ADD,
    T_SUB,
    T_MUL,
    T_DIV,
    T_INTLIT,
    T_SEMI,
    T_PRINT
} Token_type;

typedef enum AST_type
{
    A_ADD,
    A_SUB,
    A_MUL,
    A_DIV,
    A_INTLIT
} AST_type;

typedef struct Token
{
    Token_type token;
    int v;
} Token;

typedef struct AST_node
{
    struct AST_node *l;
    struct AST_node *r;
    AST_type op;
    int v;
} AST_node;

int Line;
int Input_cache;
Token T;
FILE *Infile;
FILE *Outfile;
char Text[TEXTLEN + 1];

int priority_symbol[] = {0, 10, 10, 20, 20, 0};
const char *allocate_register_arr[] = {"%r8", "%r9", "%r10", "%r11"};
int allocate_register_free_lock[] = {0, 0, 0, 0};

void usage(char *);
void init();
int scan(Token *);
int next();
int skip();
int scan_digit(int);
int str_digit_index(char *, char);
int scan_ident(int, char *, int);
Token_type keyword(char *);

AST_node *mk_AST_node(AST_type, AST_node *, AST_node *, int);
AST_node *mk_AST_leaf(AST_type, int);
AST_node *parse_AST(int);
AST_node *parse_INTLIT();
int get_priority_symbol(Token_type);
AST_type get_AST_type(Token_type);
int interpret_AST(AST_node *);

void generate_before();
int generate_ast(AST_node *);
void generate_print_int(int);
void generate_after();

int call_load(int);
int call_add(int, int);
int call_sub(int, int);
int call_mul(int, int);
int call_div(int, int);

int register_alloc();
int register_free(int);
void register_free_all();

void match(Token_type, char *);
void semi();
void statements();