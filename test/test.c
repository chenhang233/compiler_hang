
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