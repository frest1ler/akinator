#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "dump.h"
#include "tree_create.h"
#include "akinator_function.h"

int main()
{
    Tree* tree = ctor_tree();

    dump(tree, "bata.dot");

    Info_about_text info = {};

    read_commands(&info);

    insert_from_file(&info, tree);

    printf("f_dump\n");

    dump(tree, "bata2.dot");

    printf("s_dump\n");

    dump(tree, "bata3.dot");
    
    dump(tree, "bata4.dot");

    printf("destroy\n");

    bypass_destroy(tree);

    if (tree != NULL){
        free(tree);
    }

    info_dtor(&info);
    return 0;
}
