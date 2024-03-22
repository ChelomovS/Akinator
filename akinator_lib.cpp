#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "akinator_lib.h"
#include "debug.h"
#include "colors.h"
#include "tree_lib.h"

akinator_error interface(Akinator* akinator)
{
    ASSERT(akinator != nullptr);

    char answer = ' ';

    printf("Привет, это акинатор! Я могу угадать любого персонажа.\n");
    printf("Мои режимы работы: \n");
    printf("1) Введите [l], чтобы загрузить дерево из текстового файла ");
    printf("и выбрать еще один режим.\n");
    printf("2) Введите [g], чтобы начать игру с пустым деревом.\n");
    printf("3) Введите [c], чтобы сравнить по характеристикам двух персонжей\n");
    printf("4) Введите [d], чтобы вывести в stdin дерево.\n");
    printf("5) Введите [e], чтобы завершить игру.\n");

    scanf("%c", &answer);

    akinator_error error = akinator_ok;

    switch(answer)
    {
        case 'l':
        {
            Node* root_node = (Node*)calloc(1, sizeof(Node));
            if (root_node == nullptr)
                return akinator_bad_alloc;
            
            akinator->root = root_node;
            akinator->ptr_node = root_node;

            load_tree(akinator->root);

            error = interface(akinator);
            return error;
            break;
        }

        case 'g':
        {
            error = start_game(akinator);
            return error;
            break;
        }

        case 'd':
        {
            tree_print(akinator->root, stdin);
            interface(akinator);
        }

        default:
        {
            printf("Неккоректный ответ!\n");
            error = interface(akinator);
            return error;
            break;
        }
    }
}

akinator_error start_game(Akinator* akinator)
{   
    ASSERT(akinator != nullptr);

    Node* node = (Node*)calloc(1, sizeof(Node));
    if (node == nullptr)
        return akinator_bad_alloc;

    Node* first_node = (Node*)calloc(1, sizeof(Node));
    if (node == nullptr)
        return akinator_bad_alloc;

    Data text[MAX_SIZE_OF_DATA] = "Неизвестно кто";
    memcpy(node->data, text, sizeof(node->data));
    node->parent = first_node;
    first_node->left = node;
    first_node->parent = nullptr;
    akinator->ptr_node = node;
    akinator->ptr_node->parent = first_node;
    akinator->root = first_node;

    printf("Отвечай на мои вопросы ""Yes"", если я прав, и ""No"", если нет. \n");
    
    akinator_error error = game(akinator);
    return error;
}

akinator_error game(Akinator* akinator)
{
    ASSERT(akinator != nullptr);

    char answer[MAX_SIZE_OF_ANSWER + 1] = {};
    // Если указатель на правый и левый узел = nullptr, то это узел с персонажем
    while (akinator->ptr_node->right != nullptr)
    {
        printf("%s? \n", akinator->ptr_node->data); // Выводит характристику персонажа

        scanf("%s", answer);

        if (strcmp(answer, "Yes") == 0)
        {
            fprintf(stderr, "Ответ: Yes\n");
            fprintf(stderr, "Перехожу в правый узел\n");
            akinator->ptr_node->parent = akinator->ptr_node;
            akinator->ptr_node = akinator->ptr_node->right; // Переходим в правый узел
        }
        else 
        if (strcmp(answer, "No") == 0)
        {
            fprintf(stderr, "Ответ: No\n");
            fprintf(stderr, "Перехожу в левый узел\n");
            akinator->ptr_node->parent = akinator->ptr_node;
            akinator->ptr_node = akinator->ptr_node->left; // Переходим в левый узел
        }
        else
        {
            return akinator_bad_answer;
        }
    }

    printf("Это %s? \n", akinator->ptr_node->data); // Выводит персонажа

    scanf("%s", answer);

    if (strcmp(answer, "Yes") == 0)
    {
        fprintf(stdin, "Опять я победил :) \n");
    }
    else 
    if (strcmp(answer, "No") == 0)
    {
        Node* new_person = (Node*)calloc(1, sizeof(Node)); 
        if (new_person == nullptr)
            return akinator_bad_alloc;

        printf("А кто это? :( \n");
        clean_buffer();
        fgets(new_person->data, MAX_SIZE_OF_DATA, stdin); 
        printf("name: %s", new_person->data);

        Node* new_characteristic = (Node*)calloc(1, sizeof(Node));
        if (new_characteristic == nullptr)
            return akinator_bad_alloc;

        printf("Тогда чем он отличается от %s?\n", akinator->ptr_node->data);
        fgets(new_characteristic->data, MAX_SIZE_OF_DATA, stdin);
        printf("new characteristic: %s \n", new_characteristic->data);
        printf("Окей, я запомнил...\n");
        printf("Cейчас в птр ноде: %s", akinator->ptr_node->data);
        printf("ptr node parent %s\n", akinator->ptr_node->parent->data);
        // перепривязка узлов
        // ptr_node сейчас указывает на перса который не подошел
        Node* old_pers = akinator->ptr_node;
        old_pers->parent = new_characteristic;
        new_person->parent = new_characteristic;
        new_characteristic->left = old_pers;
        new_characteristic->right = new_person;
        new_characteristic->parent = akinator->ptr_node->parent;
        akinator->ptr_node->parent->right = new_characteristic;
        akinator->ptr_node = new_person;
        fprintf(stderr, "old pers: %s", old_pers->data);
    }
    else 
    {
        return akinator_bad_answer;
    }

    if (akinator->ptr_node->parent != nullptr && akinator->ptr_node != nullptr)
    {
        while (akinator->ptr_node->parent != nullptr)
        {
            printf("Зашел в цикл\n");
            akinator->ptr_node = akinator->ptr_node->parent;
        }
    }

    akinator->root = akinator->ptr_node;
    akinator_error error = end_game(akinator);
    return error;
}


akinator_error end_game(Akinator* akinator)
{
    akinator_error error = akinator_ok;
    printf("Сыграем еще раз? \n");
    char answer[MAX_SIZE_OF_ANSWER + 1] = {};
    scanf("%s", answer);

    if (strcmp(answer, "Yes") == 0)
    {
        error = game(akinator);
    }
    else 
    if (strcmp(answer, "No") == 0)
    {
        akinator_dtor(akinator->root);
        return akinator_ok;
    }
    else
    {
        akinator_dtor(akinator->root);
        return akinator_bad_answer;
    }

    return error;
}

void akinator_dtor(Node* ptr_node)
{
    ASSERT(ptr_node != nullptr);

    if (ptr_node->left != NULL)
    {
        akinator_dtor(ptr_node->left);
    }

    if (ptr_node->right != NULL)
    {
        akinator_dtor(ptr_node->right);
    }

    free(ptr_node);
}

void error_processing(akinator_error error)
{
    switch (error)
    {
        case akinator_ok:
        break;

        case akinator_bad_alloc:
            fprintf(stderr, RED "Bad allocation \n" RESET);
            break;

        case akinator_bad_answer:
            fprintf(stderr, RED "Bad answer \n" RESET);
            break;

        default:
            ASSERT(0 && "UNKNOWN ERROR");
            break;
    }
}

void clean_buffer()
{
    while (getchar() != '\n');
}