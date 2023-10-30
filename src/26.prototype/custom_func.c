#include "function.h"
#include "../cjson/cjson.h"

void custom_error_line()
{
    printf("The source code error is on line %d\n", Line);
    exit(1);
}

void custom_error_char(char *t, char c)
{
    printf("ERROR: %s ,char: %c\n", t, c);
    custom_error_line();
}

void custom_error_chars(char *c1, char *c2)
{
    printf("ERROR: %s ,info: %s\n", c1, c2);
    custom_error_line();
};

void custom_error_int(char *c1, int i)
{
    printf("ERROR: %s ,int: %d\n", c1, i);
    custom_error_line();
}

void match(Token_type t, char *str)
{
    if (t_instance.token == t)
    {
        scan(&t_instance);
    }
    else
    {
        printf("need: %d, your t_instance.token: %d\n", t, t_instance.token);
        custom_error_chars("connot match", str);
    }
}

void match_semi()
{
    return match(T_SEMI, ";");
}

void match_lbrace()
{
    match(T_LBRACE, "{");
}

void match_rbrace()
{
    match(T_RBRACE, "}");
}

void match_lparen()
{
    match(T_LPAREN, "(");
}

void match_rparen()
{
    match(T_RPAREN, ")");
}

void match_ident()
{
    match(T_IDENT, "identifier");
}

void match_int()
{
    match(T_INT, "int");
}

void match_assign()
{
    match(T_ASSIGN, "assign");
}

void match_if()
{
    match(T_IF, "if");
}

void match_while()
{
    match(T_WHILE, "while");
}

void match_for()
{
    match(T_FOR, "for");
}

void match_void()
{
    match(T_VOID, "void");
}

void match_return()
{
    match(T_RETURN, "return");
}

void match_lbracket()
{
    match(T_LBRACKET, "[");
}

void match_rbracket()
{
    match(T_RBRACKET, "]");
}

void match_comma()
{
    match(T_COMMA, ",");
}

static char *AST_node_type_keys[] = {
    "A_ASSIGN",
    "A_LOGOR",
    "A_LOGAND",
    "A_OR",
    "A_XOR",
    "A_AND",
    "A_EQ",
    "A_NE",
    "A_LT",
    "A_GT",
    "A_LE",
    "A_GE",
    "A_LSHIFT",
    "A_RSHIFT",
    "A_ADD",
    "A_SUBTRACT",
    "A_MULTIPLY",
    "A_DIVIDE",
    "A_INTLIT",
    "A_STRLIT",
    "A_IDENT",
    "A_GLUE",
    "A_IF",
    "A_WHILE",
    "A_FUNCTION",
    "A_WIDEN",
    "A_RETURN",
    "A_FUNCCALL",
    "A_DEREF",
    "A_ADDR",
    "A_SCALE",
    "A_PREINC",
    "A_PREDEC",
    "A_POSTINC",
    "A_POSTDEC",
    "A_NEGATE",
    "A_INVERT",
    "A_LOGNOT",
    "A_TOBOOL"};
static char *Primitive_type_keys[] = {
    "P_NONE",
    "P_VOID",
    "P_CHAR",
    "P_INT",
    "P_LONG",
    "P_VOIDPTR",
    "P_CHARPTR",
    "P_INTPTR",
    "P_LONGPTR"};
static cJSON *find_next_tree(ASTnode *root, cJSON *json)
{
    if (root != NULL)
    {
        // cJSON_AddNumberToObject(json, "AST_node_type", root->op);
        cJSON_AddStringToObject(json, "AST_node_type", AST_node_type_keys[root->op - 1]);
        cJSON_AddStringToObject(json, "Primitive_type", Primitive_type_keys[root->type]);
        cJSON_AddNumberToObject(json, "v_id", root->v.id);
        cJSON_AddNumberToObject(json, "rvalue", root->rvalue);

        cJSON *left = find_next_tree(root->left, cJSON_CreateObject());
        cJSON *mid = find_next_tree(root->mid, cJSON_CreateObject());
        cJSON *right = find_next_tree(root->right, cJSON_CreateObject());

        cJSON_AddItemToObject(json, "left", left);
        cJSON_AddItemToObject(json, "mid", mid);
        cJSON_AddItemToObject(json, "right", right);
        return json;
    }
    return NULL;
}

void jsonify_tree(ASTnode *root, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Failed to open file %s\n", filename);
        return;
    }
    cJSON *json = cJSON_CreateObject();
    if (json == NULL)
    {
        printf("Failed to create cJSON object\n");
        fclose(file);
        return;
    }
    find_next_tree(root, json);
    char *json_str = cJSON_Print(json);
    fprintf(file, "%s", json_str);
    cJSON_Delete(json);
    free(json_str);
    fclose(file);
}

void Gsym_dump(char *name)
{
    if (!(Gsym_dump_file = fopen(name, "w")))
        custom_error_int("open Gsym_dump_file", 0);
    for (int i = 0; i < NSYMBOLS; i++)
    {
        fprintf(Gsym_dump_file,
                "\tname=%s\t \tPrimitive_type=%s\t  \
         \tStorage_class=%s\t \tendlabel=%d\t",
                Gsym[i].name);
    }
}