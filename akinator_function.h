#ifndef AKINATOR_FUNCTION_H
#define AKINATOR_FUNCTION_H

const int COMMAND_LENGTH  = 50;

#include "main.h"

struct Info_about_text
{
    int    max_number_line;
    int    size_text      ;
    char*  text           ;
    char** ptr_line       ;
};

void read_commands(Info_about_text* info);
void init_pointer_array(Info_about_text* info);
void info_dtor(Info_about_text* info);
void insert_from_file(Info_about_text* info, Tree* tree);

#endif /*AKINATOR_FUNCTION_H*/