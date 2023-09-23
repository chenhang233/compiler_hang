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