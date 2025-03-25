#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include "akinator_function.h"
#include "tree_create.h"

static void search_new_line(Info_about_text* info);
static void count_number_lines(Info_about_text* info);
void        calculate_array_size(Info_about_text *info, const char* fname);

void read_commands(Info_about_text* info)
{
    assert(info);

    const char* fname = "TREE_INITIAL_DATA.txt"; 

    FILE * point_to_file = fopen(fname, "rb");

    if (!point_to_file) 
    {
        printf("The file %s does not open\n", fname);

        exit(EXIT_FAILURE);
    }

    calculate_array_size(info, fname);

    int number_successfully_read_chars = fread(info->text, 1, info->size_text, point_to_file);

    if (number_successfully_read_chars + 1 != info->size_text)
    {
        printf("ERROR: fread didn't read enough chars\n"
               "info->size_text = %d\n"
               "number_successfully_read_chars = %d\n",
               info->size_text, number_successfully_read_chars);

        exit(EXIT_FAILURE);
    }
    fclose(point_to_file);
    printf("%s\n", info->text);
    info->text[info->size_text - 1] = '\0';

    return;
}

void init_pointer_array(Info_about_text* info)
{
    assert(info);

    count_number_lines(info);

    search_new_line(info);
}

void calculate_array_size(Info_about_text *info, const char* fname)
{
    assert(info);
    assert(fname);

    struct stat data_for_file = {};

    stat(fname, &data_for_file);

    info->size_text = data_for_file.st_size + 1;

    if (!info->size_text)
    {
        printf("size_text = 0\n");

        exit(EXIT_FAILURE);
    }
    info->text = (char*)calloc(info->size_text, sizeof(char));

    if (info->text == NULL)
    {
        printf("ERROR: calculate_array_size; text = NULL\n");

        exit(EXIT_FAILURE);
    }
}

static void search_new_line(Info_about_text* info)
{
    if (!info)
    {
        printf("structure transfer error\n");

        exit(EXIT_FAILURE);
    }
    assert(info->text);

    info->ptr_line = (char**)calloc(info->max_number_line, sizeof(char*));

    char symbol         = 0;
    size_t line_element = 0;
    size_t number_line  = 1;

    info->ptr_line[0] = info->text;
    
    for(int i = 0; i < info->size_text - 1; i++)
    {

        if (info->text[i] == '\0')
        {
            info->ptr_line[number_line] = (info->text + i + 1);

            number_line++;
        }
    }
}

static void count_number_lines(Info_about_text* info)
{
    assert(info);

    info->max_number_line = 1;

    char symbol = 0;

    for(int line_element = 0; (symbol = info->text[line_element]) &&
        line_element < info->size_text; line_element++)
    {
        if (symbol == '\n')
        {
            info->max_number_line++;
            info->text[line_element] = '\0';
        }
    }
}

void info_dtor(Info_about_text* info)
{
    assert(info);

    if (info->text != NULL){
        free(info->text);
    }
    if (info->ptr_line != NULL){
        free(info->ptr_line);
    }
}

void insert_from_file(Info_about_text* info, Tree* tree) 
{
    if (!info || !tree){
        printf("Invalid input: info or tree is NULL\n");
        return;
    }

    Node* parent              = tree->root;
    Node* node                = NULL      ;
    char  pr_symbol           = 0         ;
    char  symbol              = 0         ;
    int   index_last_sring    = 0         ;

    for (int size = 0; size < info->size_text; size++) 
    {
        while (size < info->size_text && info->text[size] != '(' && info->text[size] != ')' 
               && info->text[size] != ';' && info->text[size] != '\r' && info->text[size] != '\0') 
        {
            size++;
        }
        symbol = info->text[size];  

        if (size < info->size_text && 
            (symbol == '(' && (pr_symbol == 0 || pr_symbol == '(')) || 
            (pr_symbol == '(' && symbol ==';') ||
            (pr_symbol == ')' && symbol == ';'))
        {   
            info->text[size] = '\0';

            if (strcmp(info->text + index_last_sring, "\0") != 0)
            {
                //printf("%s = case 1\n", info->text + index_last_sring);
                if (node == NULL)
                {
                    node = tree->root;
                    node->data = info->text + index_last_sring;
                }
                else
                {
                    node = node_ctor(info->text + index_last_sring, parent);
                    parent->left = node;     
                }
                if (symbol == '('){
                parent = node;
                }
                tree->size++;
            }
            else if (pr_symbol == ')' && symbol == ';'){
                parent = parent->parent;
            }
        } 
        else if (size < info->size_text && pr_symbol == ';' && (symbol == '(' || symbol == ')')) 
        {   
            info->text[size] = '\0';

            //printf("(%s) = case 3\n", info->text + index_last_sring);

            if (strcmp(info->text + index_last_sring, "\0") != 0)
            {
                node = node_ctor(info->text + index_last_sring, parent);

                parent->right = node;
                tree->size++;
            }
            if (symbol == '('){
                parent = node;
            }
        }
        else if (size < info->size_text && symbol == ')' && pr_symbol == ')')
        {
            info->text[size] = '\0';

            if (parent != tree->root){    
                parent = parent->parent;
            }
        }
        pr_symbol = symbol;
        index_last_sring = size + 1;
    }
}

void print_node_definition(Tree* tree, char* data)
{
    Node* node                       = search_node(tree, data);
    Node* parent                     = NULL                   ;
    int   definition[MAX_LEVEL_TREE] = {}                     ;
    int   i                          = 0                      ;

    if (node->right != NULL || node->left != NULL){
        printf("%s : this is not a leaf\n", node->data);
        return;
    }

    i = remember_way(tree, node, definition);

    for(int k = 0; k < MAX_LEVEL_TREE; k++){
        printf(" %d ", definition[k]);
    }

    printf("print_node_definition\n");
    printf("%s = ", node->data);

    if (definition[i] == LEFT_NODE){
        printf(" %s->", tree->root->data);
    }
    else{
        printf(" no %s->", tree->root->data);
    }
    node = tree->root;

    while(i > 0)
    {
        node = descent(node, definition, i);
        i--;

        if (i != 0)
        {
            if (definition[i] == LEFT_NODE){
                printf("%s->", node->data);
            }
            else{
                printf("no %s->", node->data);
            }
        }
        else
        {
            if (definition[i] == LEFT_NODE){
                printf("%s\n", node->data);    
            }
            else{
                printf("no %s\n", node->data);
            }
        }
        
    }
}

int remember_way(Tree* tree, Node* node, int* definition)
{   
    int   i      = 0   ;
    Node* parent = NULL;

    if (node != tree->root){
        parent = node->parent;
    }

    while(parent != tree->root)
    {   
        if (i == MAX_LEVEL_TREE){
            perror("trees higher than 10 are not yet provided\n");
        }

        if (parent->left == node){
            //printf("parent->left=%p\nparent->right=%p\nnode=%p\n", parent->left, parent->right, node);
            definition[i] = LEFT_NODE;
            //printf("data = %s, left\n", node->data);
        }
        else{
            definition[i] = RIGHT_NODE;
            //printf("parent->left=%p\nparent->right=%p\nnode=%p\n", parent->left, parent->right, node);
            //printf("data = %s, right\n", node->data);
        }
        node = parent;
        parent = parent->parent;
        i++;
    }
    if (parent == tree->root){
        if (parent->left == node){
            //printf("parent->left=%p\nparent->right=%p\nnode=%p\n", parent->left, parent->right, node);
            definition[i] = LEFT_NODE;
            //printf("data = %s, left\n", node->data);
        }
        else{
            //printf("parent->left=%p\nparent->right=%p\nnode=%p\n", parent->left, parent->right, node);
            definition[i] = RIGHT_NODE;
            //printf("data = %s, right\n", node->data);
        }
    }
    return i;
}
Node* descent(Node* node, int* definition, int i)
{
    if (definition[i] == LEFT_NODE){
        node = node->left;
    }
    else{
        node = node->right;
    }
    return node;
}

void compare_print_node_definition(Tree* tree, char* data_1, char* data_2)
{
    Node* node_1                       = search_node(tree, data_1);
    Node* node_2                       = search_node(tree, data_2);
    Node* parent_1                     = NULL                     ;
    Node* parent_2                     = NULL                     ;
    int   definition_1[MAX_LEVEL_TREE] = {}                       ;
    int   definition_2[MAX_LEVEL_TREE] = {}                       ;
    int   i_1                          = 0                        ;
    int   i_2                          = 0                        ;

    if (node_1->right != NULL || node_1->left != NULL){
        printf("%s : this is not a leaf\n", node_1->data);
        return;
    }
    if (node_2->right != NULL || node_2->left != NULL){
        printf("%s : this is not a leaf\n", node_2->data);
        return;
    }

    i_1 = remember_way(tree, node_1, definition_1);
    i_2 = remember_way(tree, node_2, definition_2);

    printf("common in definition: ");
    while(strcmp(node_1->data, node_2->data) == 0)
    {
        node_1 = descent(node_1, definition_1, i_1);
        node_2 = descent(node_2, definition_2, i_2);
        i_1--;
        i_2--;

        if (i_1 != 0)
        {
            if (definition_1[i_1] == LEFT_NODE){
                printf("%s->", node_1->data);
            }
            else{
                printf("no %s->", node_1->data);
            }
        }
        else
        {
            if (definition_1[i_1] == LEFT_NODE){
                printf("%s\n", node_1->data);    
            }
            else{
                printf("no %s\n", node_1->data);
            }
        }
    }    
}