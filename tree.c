/* GAIBU Marius - 315CB */
#include "tree.h"

#define MAX_CHILDREN 27
#define FIRST_INDEX 96

// Creates and initializes the queue
TQueue create_queue() {
    TQueue q = (TQueue)malloc(sizeof(Queue));
    if (!q) return NULL;

    q->head = q->tail = NULL;
    q->len = 0;

    return q;
}

// Adds a node to the end of the queue
void push(TQueue q, Tree node) {
    TList aux = (TList)malloc(sizeof(TCell));
    aux->node = node;
    aux->next = NULL;

    q->len++;

    if (!q->tail) {
        q->head = q->tail = aux;
    } else {
        q->tail->next = aux;
        q->tail = aux;
    }
}

// Extracts the first node from the queue
Tree pop(TQueue *q) {
    TList aux = (*q)->head;
    Tree node = aux->node;

    (*q)->len--;

    if ((*q)->head == (*q)->tail) {
        (*q)->head = (*q)->tail = NULL;
    } else {
        (*q)->head = (*q)->head->next;
    }
    free(aux);

    return node;
}

// Frees the memory used by the queue
void destroy_queue(TQueue *q) {
    free(*q);
}

// Creates and initializes a node in the tree
Tree create_node(int level, int num_children, char info) {
    Tree node = (Tree)malloc(sizeof(Node));
    if (!node) return NULL;

    node->level = level;
    node->num_children = num_children;
    node->info = (char *)malloc(2 * sizeof(char));
    snprintf(node->info, 2 * sizeof(char), "%c", info);

    for (int i = 0; i < MAX_CHILDREN; i++) {
        node->children[i] = NULL;
    }

    return node;
}

// Destroys the tree
void destroy_tree(Tree *tree) {
    Tree node = *tree;
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i]) {
            destroy_tree(&(node->children[i]));
        }
    }
    free(node->info);
    free(node);
}

// Returns the index of the current character in the children list
int get_index(char current_char) {
    if (current_char == '$') {
        return 0;
    } else {
        return (int)current_char - FIRST_INDEX;
    }
}

// Adds all suffixes of a word to the tree
void add_every_suffix(Tree tree, char *word) {
    word = strtok(word, "\n");
    strcat(word, "$");

    Tree root = tree;
    int len = (int)strlen(word);

    for (int i = 0; i < len; i++) {
        char *suffix = word + i;  // Extracts the current suffix
        tree = root;

        // Adds each character consecutively in the tree
        for (; *suffix != '\0'; suffix++) {
            int idx = get_index(*suffix);

            if (tree->children[idx] == NULL) {
                tree->children[idx] = create_node(tree->level + 1, 0, *suffix);
                tree->num_children++;
            }
            tree = tree->children[idx];
        }
    }
}

// Displays the tree structure by levels
void show_tree(FILE* fout, Tree root) {
    TQueue bfs_queue = create_queue();
    int check_newline = 0;
    push(bfs_queue, root);

    while (bfs_queue->head) {
        Tree node = pop(&bfs_queue);  // Extracts the current node

        // Adds the descendants of the current node to the queue
        for (int i = 0; i < MAX_CHILDREN; i++) {
            if (node->children[i] != NULL) {
                push(bfs_queue, node->children[i]);
            }
        }
        if (node->level == 0) {
            check_newline = bfs_queue->len;
            continue;
        }

        // Displays the current node's info
        fprintf(fout, "%s ", node->info);
        check_newline--;

        if (check_newline == 0) {  // Visually moves to the new level
            fprintf(fout, "\n");
            check_newline = bfs_queue->len;
        }
    }

    destroy_queue(&bfs_queue);
}

/*
    Displays the sought statistics for the tree (number of leaf nodes,
    number of suffixes of length k, maximum number of descendants of a node)
*/
void get_tree_stats(FILE* fout, Tree root, int k) {
    TQueue bfs_queue = create_queue();
    int leaf_count = 0, k_len_suffix = 0, max_descendants = 0;
    push(bfs_queue, root);

    while (bfs_queue->head) {
        Tree node = pop(&bfs_queue);  // Extracts the current node

        // Updates the sought statistics
        if (node->num_children > max_descendants) {
            max_descendants = node->num_children;
        }
        leaf_count += (node->num_children == 0);
        k_len_suffix += (node->children[0] && node->level == k);

        // Adds the descendants of the current node to the queue
        for (int i = 0; i < MAX_CHILDREN; i++) {
            if (node->children[i] != NULL) {
                push(bfs_queue, node->children[i]);
            }
        }
    }

    // Displays the sought statistics
    fprintf(fout, "%d\n", leaf_count);
    fprintf(fout, "%d\n", k_len_suffix);
    fprintf(fout, "%d\n", max_descendants);

    destroy_queue(&bfs_queue);
}

// Checks for the existence of a suffix in the tree
int search_suffix(Tree tree, char *suffix) {
    suffix = strtok(suffix, "\n");

    for (; *suffix != '\0'; suffix++) {
        int idx = get_index(*suffix);

        if (tree->children[idx] == NULL) {
            return 0;
        }
        tree = tree->children[idx];
    }
    return (tree->children[0] != NULL);
}

// Performs compression of the current tree
void compress_tree(Tree *tree) {
    Tree node = *tree;

    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i] != NULL) {
            compress_tree(&(node->children[i]));

            // If the node has a single child, data compression can be performed
            if (node->num_children == 1 && i != 0) {
                Tree temp = node->children[i];

                // Updates the current node's info with the compacted version
                node->info = realloc(node->info, strlen(temp->info) + 2);
                memcpy(node->info + 1, temp->info, strlen(temp->info) + 1);

                // Replaces the current node's descendants with the child's descendants
                memcpy(node->children, temp->children, sizeof(temp->children));
                node->num_children = temp->num_children;

                // Destroys the child node
                free(temp->info);
                free(temp);
            }
        }
    }
}
