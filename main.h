#ifndef TREE_H
#define TREE_H

struct Node
{
    char* data   ;
    Node* left   ;
    Node* right  ;
    Node* pointer;
    Node* parent ;
};

struct Tree
{
    int   size;
    Node* root;
};

#endif /*TREE_H*/