/* GAIBU Marius - 315CB */
#include "tree.h"

#define MAX_CHILDREN 27
#define FIRST_INDEX 96

// Crează și inițializează coada
TQueue create_queue() {
    TQueue q = (TQueue)malloc(sizeof(Queue));
    if (!q) return NULL;

    q->head = q->tail = NULL;
    q->len = 0;

    return q;
}

// Adaugă un nod la sfârșitul cozii
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

// Extrage primul nod din coadă
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

// Eliberează memoria ocupată de coadă
void destroy_queue(TQueue *q) {
    free(*q);
}

// Crează și inițializează un nod din arbore
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

// Distruge arborele
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

// Returnează index-ul caracterului curent în lista de copii
int get_index(char current_char) {
    if (current_char == '$') {
        return 0;
    } else {
        return (int)current_char - FIRST_INDEX;
    }
}

// Adaugă toate sufixele unui cuvând în arbore
void add_every_suffix(Tree tree, char *word) {
    word = strtok(word, "\n");
    strcat(word, "$");

    Tree root = tree;
    int len = (int)strlen(word);

    for (int i = 0; i < len; i++) {
        char *suffix = word + i;  // Extrage sufixul curent
        tree = root;

        // Adaugă fiecare caracter în cascadă în arbore
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

// Afișează structura arborelui pe nivele
void show_tree(FILE* fout, Tree root) {
    TQueue bfs_queue = create_queue();
    int check_newline = 0;
    push(bfs_queue, root);

    while (bfs_queue->head) {
        Tree node = pop(&bfs_queue);  // Extrage nodul curent

        // Adaugă descendenții nodului curent în coadă
        for (int i = 0; i < MAX_CHILDREN; i++) {
            if (node->children[i] != NULL) {
                push(bfs_queue, node->children[i]);
            }
        }
        if (node->level == 0) {
            check_newline = bfs_queue->len;
            continue;
        }

        // Afișează informația nodului curent
        fprintf(fout, "%s ", node->info);
        check_newline--;

        if (check_newline == 0) {  // Trece vizual pe noul nivel
            fprintf(fout, "\n");
            check_newline = bfs_queue->len;
        }
    }

    destroy_queue(&bfs_queue);
}

/*
    Afișează statisticile căutate pentru arbore (numărul de noduri frunză,
    numărul de sufixe de lungime k, numărul maxim de descendenți ale unui nod)
*/
void get_tree_stats(FILE* fout, Tree root, int k) {
    TQueue bfs_queue = create_queue();
    int leaf_count = 0, k_len_suffix = 0, max_descendants = 0;
    push(bfs_queue, root);

    while (bfs_queue->head) {
        Tree node = pop(&bfs_queue);  // Extrage nodul curent

        // Actualizează statisticile căutate
        if (node->num_children > max_descendants) {
            max_descendants = node->num_children;
        }
        leaf_count += (node->num_children == 0);
        k_len_suffix += (node->children[0] && node->level == k);

        // Adaugă descendenții nodului curent în coadă
        for (int i = 0; i < MAX_CHILDREN; i++) {
            if (node->children[i] != NULL) {
                push(bfs_queue, node->children[i]);
            }
        }
    }

    // Afișează statisticile căutate
    fprintf(fout, "%d\n", leaf_count);
    fprintf(fout, "%d\n", k_len_suffix);
    fprintf(fout, "%d\n", max_descendants);

    destroy_queue(&bfs_queue);
}

// Verifică existența unui sufix în arbore
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

// Realizează compresia arborelui curent
void compress_tree(Tree *tree) {
    Tree node = *tree;

    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i] != NULL) {
            compress_tree(&(node->children[i]));

            // Dacă nodul are un singur fiu, se poate face compresia datelor
            if (node->num_children == 1 && i != 0) {
                Tree temp = node->children[i];

                // Actualizează informația nodului curent cu versiunea compactă
                node->info = realloc(node->info, strlen(temp->info) + 2);
                memcpy(node->info + 1, temp->info, strlen(temp->info) + 1);

                // Modifică descendenții nodului curent cu cei ai fiului
                memcpy(node->children, temp->children, sizeof(temp->children));
                node->num_children = temp->num_children;

                // Distruge nodul fiu
                free(temp->info);
                free(temp);
            }
        }
    }
}
