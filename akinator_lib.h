#ifndef AKINATOR_LIB_H
#define AKINATOR_LIB_H

#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "colors.h"
#include "tree_lib.h"

const size_t MAX_SIZE_OF_ANSWER = 3;

enum akinator_error
{
    akinator_ok         = 0,
    akinator_bad_answer = 1,
    akinator_bad_alloc  = 2,
};

typedef struct Akinator
{
    Node* root; 
    Node* ptr_node;
} Akinator;

akinator_error interface(Akinator* akinator);
akinator_error start_game(Akinator* akinator);
akinator_error game(Akinator* akinator);
akinator_error end_game(Akinator* akinator);
void akinator_dtor(Node* ptr_node);
void error_processing(akinator_error error);
void clean_buffer();

//akinator_error node_insert(Akinator* akinator, Data text);

#endif // AKINATOR_LIB_H