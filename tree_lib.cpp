#include <stdio.h>
#include <ctype.h>

#include "tree_lib.h"
#include "debug.h"
#include "string.h"
#include "fileLib.h"

Node* create_node(Node* ptr_node, const char* parent_direction)
{
    ASSERT(ptr_node != nullptr);

    Node* tmp_node = (Node*)calloc(1, sizeof(Node));
    ASSERT(tmp_node != nullptr);

    if (strcmp(parent_direction, "right") == 0) // родитель справа
    {
        tmp_node->parent = ptr_node;
        ptr_node->left = tmp_node; // левый узел - ребенок
    }
    else 
    if (strcmp(parent_direction, "left") == 0) // родитель слева 
    {
        tmp_node->parent = ptr_node;
        ptr_node->right = tmp_node; // ребенок справа
    }

    return tmp_node;
}

void tree_dump(Node* ptr_node, FILE* file)
{
    if (ptr_node == nullptr)
    {
        return;
    }
    fprintf(file, "Указатель на данный узел: %p \n", ptr_node);
    fprintf(file, "Данные в узле:            %s \n", ptr_node->data);
    fprintf(file, "Связь узлов: \n");
    fprintf(file, "Левый подузел:            %p \n", ptr_node->left);
    fprintf(file, "Правый подузел:           %p \n", ptr_node->right);
    fprintf(file, "Родитель узла:            %p \n\n\n", ptr_node->parent);
    tree_dump(ptr_node->left, file);
    tree_dump(ptr_node->right, file);
}

void tree_dtor(Node* ptr_node)
{
    ASSERT(ptr_node != nullptr);

    if (ptr_node->left != NULL)
    {
        tree_dtor(ptr_node->left);
    }

    if (ptr_node->right != NULL)
    {
        tree_dtor(ptr_node->right);
    }

    free(ptr_node);
}

void load_tree(Node* node) // функция загружает дерево из текстового файла
{
    FILE* tree_file = fopen("tree.txt", "r");
    ASSERT(tree_file != nullptr);

    size_t file_size = GetFileSize(tree_file);
    char* buffer = (char*)calloc(file_size + 1, sizeof(char));

    fread(buffer, sizeof(char), file_size, tree_file);

    fclose(tree_file);
    make_tree(node, buffer);
    tree_print(node, stdout);
}

char* make_tree(Node* node, char* buffer) // обработка массива с "деревом"
{
    ASSERT(node != nullptr);
    ASSERT(buffer != nullptr);

    skip_spaces(&buffer);

    if(*buffer == '{') // открытие узла 
    {
        buffer++;
    }
    skip_spaces(&buffer);

    for (size_t move = 0; (*buffer != '}') && (*buffer != '\0'); buffer++)
    {
        skip_spaces(&buffer);
        if (*buffer == '{')
        {
            skip_spaces(&buffer);
            printf("finded {\n");
            buffer = make_tree(create_node(node, "right"), buffer + 1);
            skip_spaces(&buffer);
            buffer = make_tree(create_node(node, "left"), buffer + 1);
            skip_spaces(&buffer);
        }
        else
        {
            skip_spaces(&buffer);
            node->data[move] = *buffer;
            move++;
            skip_spaces(&buffer);
        }
        skip_spaces(&buffer);
    }
    skip_spaces(&buffer);
    return buffer;
}

void tree_print(Node* ptr_node, FILE* file)
{
    if (ptr_node == nullptr)
    {
        return;
    }
    
    fprintf(file, "{%s", ptr_node->data);
    tree_print(ptr_node->left, file);
    tree_print(ptr_node->right, file);

    fprintf(file, "}");
}

void skip_spaces(char** buffer)
{
    ASSERT(buffer != nullptr)

    while ((isspace(**buffer) != 0) || ((**buffer) == '\n'))
        (*buffer)++;   
}


// void node_insert(Node* ptr_node, Data value) пока не используется
// {     
//     ASSERT(ptr_node != nullptr);
//     Node* tmp = (Node*)calloc(1, sizeof(Node));
//     tmp->data = value;
//     while(true)
//     {
//         if (value >= ptr_node->data)
//         {
//             if (ptr_node->right == NULL)
//             {
//                 tmp->parent = ptr_node;
//                 ptr_node->right = tmp;
//                 break;
//             }
//             else 
//             {
//                 ptr_node = ptr_node->right;
//             }
//         }
//         else 
//         {
//             if (ptr_node->left == NULL)
//             {
//                 tmp->parent = ptr_node;
//                 ptr_node->left = tmp;
//                 break;
//             }
//             else
//             {
//                 ptr_node = ptr_node->left;
//             }
//         }
//     }
// }
