#ifndef AKINATOR_FUNCTION_H
#define AKINATOR_FUNCTION_H

const int COMMAND_LENGTH = 50;
const int MAX_LEVEL_TREE = 10;
const int LEFT_NODE      = 1 ;
const int RIGHT_NODE     = 2 ; 
#include "main.h"

struct Info_about_text
{
    int    max_number_line;
    int    size_text      ;
    char*  text           ;
    char** ptr_line       ;
};

void  read_commands(Info_about_text* info);
void  init_pointer_array(Info_about_text* info);
void  info_dtor(Info_about_text* info);
void  insert_from_file(Info_about_text* info, Tree* tree);
void  print_node_definition(Tree* tree, char* data);
void  compare_print_node_definition(Tree* tree, char* data_1, char* data_2);
void print_subdefinition(Node* node, int* definition, int i);

Node* descent(Node* node, int* definition, int i);

int  remember_way(Tree* tree, Node* node, int* definition);

#endif /*AKINATOR_FUNCTION_H*/