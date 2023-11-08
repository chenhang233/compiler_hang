#include "data.h"

#pragma once

// custom_func.c
void custom_error_line();
void custom_error_char(char *t, char c);
void custom_error_chars(char *c1, char *c2);
void custom_error_int(char *c1, int i);
void match(Token_type t, char *str);
void match_semi();
void match_lbrace();
void match_rbrace();
void match_lparen();
void match_rparen();
void match_ident();
void match_int();
void match_assign();
void match_if();
void match_while();
void match_for();
void match_void();
void match_return();
void match_lbracket();
void match_rbracket();
void match_comma();
void jsonify_tree(ASTnode *root, const char *filename);
void Gsym_dump(char *name);

// scan.c
int next();
int skip();
int scan(Token *);
int int_pos(char *str, int s);
int scanint(int c);
static int scanident(char c, char *str, int strlen);
int scankey(char *s);
void reject_token(Token *t);
static int scan_ch(void);
static int scanstr(char *buf);

// gen.c
void genpreamble();
void genfreeregs();
void genprintint(int reg);
void genglobsym(int id);
int genAST(ASTnode *n, int label, AST_node_type parentASTop);
int genIf(ASTnode *n);
int genWhile(ASTnode *n);
int label();
int genprimsize(int type);
void genpostamble();
int genlabel(void);
int genglobstr(char *strvalue);
// int gengetlocaloffset(int type, int isparam);
void genresetlocals(void);

// cg.c
void freeall_registers(void);
static void free_register(int reg);
void cgpreamble();
static int alloc_register(void);
void cgfuncpreamble(int id);
void cgfuncpostamble(int id);
void cgprintint(int r);
void cgglobsym(int id);
void cgglobstr(int l, char *strvalue);
int cgloadint(int value, int type);
int cgloadglob(int id, int op);
int cgloadglobstr(int id);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);
int cgstorglob(int r, int id);
int cgstorderef(int r1, int r2, int type);
int cgstorlocal(int r, int id);
int cgcompare_and_set(AST_node_type ASTop, int r1, int r2);
void cglabel(int l);
void cgjump(int l);
int cgcompare_and_jump(AST_node_type ASTop, int r1, int r2, int label);
int cgwiden(int r, int oldtype, int newtype);
int cgprimsize(Primitive_type type);
void cgreturn(int reg, int id);
int cgcall(int id, int numargs);
int cgaddress(int id);
int cgderef(int r, int type);
void cgpostamble();
int cgshlconst(int r, int val);
int cgand(int r1, int r2);
int cgor(int r1, int r2);
int cgxor(int r1, int r2);
int cgshl(int r1, int r2);
int cgshr(int r1, int r2);
int cgnegate(int r);
int cginvert(int r);
int cglognot(int r);
int cgboolean(int r, int op, int label);
void cgresetlocals(void);
int newlocaloffset(int type);
void cgtextseg();
int cgloadlocal(int id, int op);
void cgdataseg();
void cgcopyarg(int r, int argposn);

// stm.c
ASTnode *function_declaration(Primitive_type type);
ASTnode *compound_statement();
static ASTnode *single_statement(void);
static ASTnode *print_statement(void);
symtable *var_declaration(Primitive_type type, Storage_class class);
static ASTnode *assignment_statement(void);
static ASTnode *if_statement(void);
static ASTnode *while_statement();
static ASTnode *for_statement();
static ASTnode *return_statement(void);
Primitive_type parse_type();
void global_declarations();
static int param_declaration(symtable *funcsym);

// ast.c
ASTnode *mkAST_node(AST_node_type op, Primitive_type type, ASTnode *left, ASTnode *mid, ASTnode *right,
                    symtable *sym, int intv);
ASTnode *mkAST_leaf(AST_node_type op, Primitive_type type,
                    symtable *sym, int id);
ASTnode *mkAST_left(AST_node_type op, Primitive_type type, ASTnode *left,
                    symtable *sym, int v);
ASTnode *binexpr(int ptp);
static ASTnode *primary(void);
static ASTnode *array_access(void);
static int op_precedence(Token_type tokentype);
static int rightassoc(Token_type tokentype);
static AST_node_type binastop(Token_type tokentype);
static int arithop(int tokentype);
ASTnode *funccall(void);
ASTnode *prefix(void);
static ASTnode *postfix(void);
static int gendumplabel(void);
void dumpAST(ASTnode *n, int label, int level);
static ASTnode *expression_list(void);
static int gen_funccall(ASTnode *n);

// sym.c
void appendsym(symtable **head, symtable **tail, symtable *node);
symtable *newsym(char *name, Primitive_type type, Structural_type stype,
                 Storage_class class, int size, int posn);
char *my_strdup(const char *source);
symtable *addglob(char *name, Primitive_type type, Structural_type stype,
                  Storage_class class, int size);

symtable *addlocl(char *name, Primitive_type type, Structural_type stype,
                  Storage_class class, int size);
symtable *addparm(char *name, Primitive_type type, Structural_type stype,
                  Storage_class class, int size);
symtable *findglob(char *name);
symtable *findcomposite(char *s);
symtable *findlocl(char *name);
symtable *findsymbol(char *s);
void clear_symtable(void);
void freeloclsyms(void);

// types.c
int type_compatible(Primitive_type *left, Primitive_type *right, int onlyright);
Primitive_type pointer_to(Primitive_type type);
Primitive_type value_at(Primitive_type type);
ASTnode *modify_type(ASTnode *tree, Primitive_type rtype, AST_node_type op);
int inttype(Primitive_type type);
int ptrtype(Primitive_type type);