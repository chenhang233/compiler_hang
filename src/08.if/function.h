#include "data.h"

// custom_error.c
void custom_error_line();
void custom_error_char(char *, char);
void custom_error_chars(char *, char *);
void custom_error_int(char *, int);

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