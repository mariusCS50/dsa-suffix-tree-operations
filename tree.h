/* GAIBU Marius - 315CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHILDREN 27

// Structure representing the suffix tree
typedef struct node {
    int level;
    int num_children;
    char *info;
    struct node *children[MAX_CHILDREN];
} Node, *Tree;

// Structure representing a singly linked list
typedef struct cell {
    Tree node;
    struct cell *next;
} TCell, *TList;

// Structure representing the queue
typedef struct queue {
    int len;
    TList head, tail;
} Queue, *TQueue;

// Functions related to the queue
TQueue create_queue();
void push(TQueue q, Tree node);
Tree pop(TQueue *q);
void destroy_queue(TQueue *q);

// Functions related to the suffix tree
Tree create_node(int level, int num_children, char info);
void destroy_tree(Tree *tree);

void add_every_suffix(Tree tree, char *word);
void show_tree(FILE* fout, Tree tree);
void get_tree_stats(FILE* fout, Tree tree, int K);
int search_suffix(Tree tree, char *suffix);
void compress_tree(Tree *tree);
