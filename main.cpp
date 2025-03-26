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

    char* new_leaf = (char*)calloc(COMMAND_LENGTH, sizeof(char));

    read_commands(&info);

    insert_from_file(&info, tree);

    printf("f_dump\n");

    dump(tree, "bata2.dot");

    printf("s_dump\n");

    Node* node = search_node(tree, "cat");
    printf("\n\n\n\n\n\n\n");
    printf("node==%p\n", node);
    printf("node->data==%s\n", node->data);
    printf("\n\n\n\n\n\n\n");

    //dump(tree, "bata3.dot");
    
    //dump(tree, "bata4.dot");

    print_node_definition(tree, "papagey");

    compare_print_node_definition(tree, "dogfabr", "papagey");

    akinator(tree, new_leaf);

    dump(tree, "bata3.dot");

    printf("destroy\n");

    bypass_destroy(tree);

    if (tree != NULL){
        free(tree);
    }

    info_dtor(&info);

    if (new_leaf != NULL){
        free(new_leaf);
    }
    return 0;
}
