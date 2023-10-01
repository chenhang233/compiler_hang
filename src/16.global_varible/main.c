#include "function.h"
#include "../cjson/cjson.h"

cJSON *find_next_tree(ASTnode *root, cJSON *json)
{
    if (root != NULL)
    {
        cJSON_AddNumberToObject(json, "AST_node_type", root->op);
        cJSON_AddNumberToObject(json, "Primitive_type", root->type);
        cJSON_AddNumberToObject(json, "v_id", root->v.id);
        cJSON_AddNumberToObject(json, "v_intvalue", root->v.intvalue);

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

int init()
{
    Line = 1;
    cache = 0;
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
        custom_error_chars("argc length", (char *)argv[0]);
    if (!(Infile = fopen(argv[1], "r")))
    {
        custom_error_chars("unable open", (char *)argv[1]);
    }
    if ((Outfile = fopen("out.s", "w")) == NULL)
    {
        custom_error_int("Unable to create out.s", 0);
        exit(1);
    }
    init();
    addglob("printint", P_CHAR, S_FUNCTION, 0);
    scan(&t_instance);
    genpreamble();
    ASTnode *tree;
    global_declarations();
    genpostamble();
    fclose(Outfile);
    return 0;
}
