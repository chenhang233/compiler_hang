#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma once

// cmds and default filenames
#define AOUT "a.out"
#define ASCMD "as -o "
#define LDCMD "cc -o "

#define TEXTLEN 1024
#define NSYMBOLS 1024 // Number of symbol table entries

#define NOREG -1
#define NOLABEL 0 // Use NOLABEL when we have no label to

typedef struct symtable symtable;

typedef enum Token_type
{
    T_EOF, //  eof
    // Binary operators
    T_ASSIGN, // assign =
    T_LOGOR,  // ||
    T_LOGAND, // &&
    T_OR,     // |
    T_XOR,    //^
    T_AMPER,  // &
    T_EQ,     // ==
    T_NE,     // !=
    T_LT,     // <
    T_GT,     // >
    T_LE,     // <=
    T_GE,     //  >=
    T_LSHIFT, // <<
    T_RSHIFT, // >>

    T_PLUS,  //  +
    T_MINUS, // -
    T_STAR,  //  *
    T_SLASH, //  /

    // Other operators
    T_INC,    //++
    T_DEC,    //--
    T_INVERT, // ~
    T_LOGNOT, // !
    // Type keywords
    T_VOID, // void
    T_CHAR, // char
    T_INT,  // int
    T_LONG, // long

    // Other keywords
    T_IF,
    T_ELSE,
    T_WHILE,
    T_FOR,
    T_RETURN,

    // Structural tokens
    T_INTLIT,   // digit
    T_STRLIT,   // string
    T_SEMI,     // ;
    T_IDENT,    //  identify
    T_LBRACE,   // {
    T_RBRACE,   // }
    T_LPAREN,   // (
    T_RPAREN,   // )
    T_LBRACKET, // [
    T_RBRACKET, // ]
    T_COMMA     // ,
} Token_type;

typedef enum Structural_type
{
    S_VARIABLE,
    S_FUNCTION,
    S_ARRAY
} Structural_type;

typedef enum Primitive_type
{
    P_NONE,
    P_VOID = 16,
    P_CHAR = 32,
    P_INT = 48,
    P_LONG = 64
} Primitive_type;

typedef enum AST_node_type
{
    A_ASSIGN = 1,
    A_LOGOR,
    A_LOGAND,
    A_OR,
    A_XOR,
    A_AND,
    A_EQ,
    A_NE,
    A_LT,
    A_GT,
    A_LE,
    A_GE,
    A_LSHIFT,
    A_RSHIFT,
    A_ADD,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_INTLIT,
    A_STRLIT,
    A_IDENT,
    A_GLUE,
    A_IF,
    A_WHILE,
    A_FUNCTION,
    A_WIDEN,
    A_RETURN,
    A_FUNCCALL,
    A_DEREF,
    A_ADDR,
    A_SCALE,
    A_PREINC,
    A_PREDEC,
    A_POSTINC,
    A_POSTDEC,
    A_NEGATE,
    A_INVERT,
    A_LOGNOT,
    A_TOBOOL
} AST_node_type;

typedef struct ASTnode
{
    AST_node_type op;
    Primitive_type type;  // Type of any expression this tree generates
    int rvalue;           // True if the node is an rvalue
    struct ASTnode *left; // Left, middle and right child trees
    struct ASTnode *mid;
    struct ASTnode *right;
    symtable *sym; // a global symbol pointer A_FUNCTION A_ADDR
    union
    {
        int intvalue; // For A_INTLIT, the integer value
        int id;       // For A_IDENT, the symbol slot number
        int size;     // For A_SCALE, the size to scale by
        // For A_FUNCCALL, the symbol slot number
    } v;
} ASTnode;

typedef struct Token
{
    Token_type token;
    int intvalue;
} Token;

// Storage classes
typedef enum storage_class
{
    C_GLOBAL = 1, // Globally visible symbol
    C_LOCAL,      // Locally visible symbol
    C_PARAM       // Locally visible function parameter
} Storage_class;

typedef struct symtable
{
    // Name of a symbol
    char *name;
    /*   Primitive type for the symbol
        For S_FUNCTIONs:  return value type
    */
    Primitive_type type;
    // Structural type for the symbol
    Structural_type stype;
    // Storage class for the symbol
    Storage_class class;
    union
    {
        int size;     // Number of elements in the symbol
        int endlabel; // For functions, the end label
    };
    union
    {
        int nelems; // For functions, # of params
        int posn;   // For locals, the negative offset
                    // from the stack base pointer
    };
    symtable *next;   // Next symbol in one list
    symtable *member; // First member of a function, struct,union or enum
} symtable;

int Line;
int cache;
char Text[TEXTLEN];
// -------------------------------------------------------------
int O_dumpAST;  // -T If true, dump the AST trees
int O_keepasm;  // -S If true, keep any assembly files
int O_assemble; // -c If true, assemble the assembly files
int O_dolink;   // -o If true, link the object files
int O_verbose;  // -v If true, print info on compilation stages

Token t_instance;
FILE *Infile;
FILE *Outfile;
FILE *Gsym_dump_file;
symtable Gsym[NSYMBOLS]; // global symbol table

symtable *Functionid;          // Symbol ptr of the current function
symtable *Globhead, *Globtail; // Global variables and functions
symtable *Loclhead, *Locltail; // Local variables
symtable *Parmhead, *Parmtail; // Local parameters
symtable *Comphead, *Comptail; // Composite types
