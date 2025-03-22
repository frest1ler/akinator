#include <stdlib.h>
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
void        fill_value(char* adr, int value);

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

void fill_value(char* adr, int value)
{
    for(int i = 0; i < COMMAND_LENGTH; i++)
    {
        adr[i] = value;
    }
}

void insert_from_file(Info_about_text* info, Tree* tree)
{
    Node* node                 = (Node*)tree;
    Node* parent               = tree->root ;
    char  cmd[COMMAND_LENGTH]  = {}         ;
    int   i                    = 0          ;
    int   descent_height       = 0          ;

    for(int size = 0; size < info->size_text; size++)
    {
        while(info->text[size] != '(' && info->text[size] != ')' && info->text[size] != ';' &&
              size < info->size_text)
        {
            cmd[i] = info->text[size];
            size++;
            i++; 
        }
        i = 0;

        if (info->text[size] == '(')
        {   
            descent_height++;

            if (node != (Node*)tree)
            {   
                node = node_ctor(cmd, parent);
                parent->left = node;
                parent = node;

                //insert_string(node, cmd);
                tree->size++;
                fill_value(cmd, 0);
            }
            else
            {
                node->data = cmd;
                node = tree->root;

                fill_value(cmd, 0);
            }
        }
        else if (info->text[size] == ';')
        {
            node = node_ctor(cmd, parent);
            parent->left = node;
            parent = node;

            //insert_string(node, cmd);
            tree->size++;
            fill_value(cmd, 0);
        }
        else if (info->text[size] == ')')
        {   
            //descent_height--; нужно функцию подъёма до нужной высоты

            node = node_ctor(cmd, parent);
            parent->right = node;
            parent = node;

            //insert_string(node, cmd);
            tree->size++;
            fill_value(cmd, 0);
        }
    }
}

// void insert_string(Node* node, char* cmd)
// {   
//     if (node == NULL){
//         perror("you don't have a tree\n");
//     }

//     int   branch = POISON    ;
//     Node* parent = 0         ;
//     Node* node   = tree->root;

//     while(node != NULL && node->data != POISON)
//     {
//         parent = node;

//         if (value < node->data){
//             branch = LEFT;
//             node = node->left;
//         }
//         else{
//             branch = RIGHT;
//             node = node->right;
//         }
//     }
    
//     if (branch == LEFT || branch == RIGHT){
//         node = node_ctor(value, parent);

//         if (branch == LEFT){
//             parent->left = node;
//         }
//         else{
//             parent->right = node;
//         }   
//     }
//     else{ //branch == POISON
//         node->data = value;
//     }
//     (tree->size)++;
// }

// if (info->text[size] == '(')
//         {
//             if (node != tree->root)
//             {   
//                 node = node_ctor(cmd, parent);
//                 parent->left = node;
//                 parent = node;

//                 insert_string(node, cmd);
//                 tree->size++;
//                 fill_value(cmd, 0);
//                 i = 0;
//             }
//             else
//             {
//                 node->data = cmd;
//                 fill_value(cmd, 0);
//                 i = 0;
//             }
//         }
//         else if (info->text[size] == ';')
//         {

//         }
//         else if (info->text[size] == ')')
//         {

//         }