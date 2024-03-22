#ifndef TREE_LIB_H
#define TREE_LIB_H

#include "fileLib.h"

typedef char Data;

const size_t MAX_SIZE_OF_BUFFER = 1000;
const size_t MAX_SIZE_OF_DATA   = 50;

typedef struct Node
{
    Data  data[MAX_SIZE_OF_DATA];
    Node* left;
    Node* right;
    Node* parent;
} Node;

void tree_print(Node* ptr_node, FILE* file);
Node* create_node(Node* ptr_node, const char* parent_direction);
char* make_tree(Node* node, char* buffer);
void tree_dtor(Node* ptr_node);
void load_tree(Node* node);
void tree_dump(Node* ptr_node, FILE* file);
void skip_spaces(char** buffer);

// void node_insert(Node* ptr_node, Data value);
#endif // TREE_H