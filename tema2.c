/* GAIBU Marius - 315CB */
#include "tree.h"

#define MAX_BUFFER_SIZE 256

// Funția care realizează cerința 1
void build_and_show(FILE* fin, FILE* fout) {
    Tree tree = create_node(0, 0, '0');
    int num_words = 0;
    fscanf(fin, "%d\n", &num_words);

    while (num_words--) {
        char buffer[MAX_BUFFER_SIZE];
        fgets(buffer, MAX_BUFFER_SIZE, fin);
        add_every_suffix(tree, buffer);
    }

    show_tree(fout, tree);
    destroy_tree(&tree);
}

// Funția care realizează cerința 2
void build_and_get_stats(FILE* fin, FILE* fout, int k) {
    Tree tree = create_node(0, 0, '0');
    int num_words = 0;
    fscanf(fin, "%d\n", &num_words);

    while (num_words--) {
        char buffer[MAX_BUFFER_SIZE];
        fgets(buffer, MAX_BUFFER_SIZE, fin);
        add_every_suffix(tree, buffer);
    }

    get_tree_stats(fout, tree, k);
    destroy_tree(&tree);
}

// Funția care realizează cerința 3
void build_and_search_suffix(FILE* fin, FILE* fout) {
    Tree tree = create_node(0, 0, '0');
    int num_words = 0, num_suffix = 0;
    fscanf(fin, "%d %d\n", &num_words, &num_suffix);

    while (num_words--) {
        char buffer[MAX_BUFFER_SIZE];
        fgets(buffer, MAX_BUFFER_SIZE, fin);
        add_every_suffix(tree, buffer);
    }

    while (num_suffix--) {
        char buffer[MAX_BUFFER_SIZE];
        fgets(buffer, MAX_BUFFER_SIZE, fin);
        fprintf(fout, "%d\n", search_suffix(tree, buffer));
    }

    destroy_tree(&tree);
}

// Funția care realizează cerința 4
void build_and_compress(FILE* fin, FILE* fout) {
    Tree tree = create_node(0, 0, '0');
    int num_words = 0;
    fscanf(fin, "%d\n", &num_words);

    while (num_words--) {
        char buffer[MAX_BUFFER_SIZE];
        fgets(buffer, MAX_BUFFER_SIZE, fin);
        add_every_suffix(tree, buffer);
    }

    compress_tree(&tree);
    show_tree(fout, tree);
    destroy_tree(&tree);
}

int main(int argc, char *argv[]) {
    (void)argc;

    int task_number = *(argv[1] + 2) - '0';  // Identificarea numărului cerinței

    // Deschiderea fișierelor pentru I/O
    FILE* fin = fopen(argv[2 + (task_number == 2)], "rt");
    FILE* fout = fopen(argv[3 + (task_number == 2)], "wt");

    // Executarea cerinței
    switch (task_number) {
        case 1: build_and_show(fin, fout); break;
        case 2: build_and_get_stats(fin, fout, atoi(argv[2])); break;
        case 3: build_and_search_suffix(fin, fout); break;
        case 4: build_and_compress(fin, fout); break;
    }

    // Închiderea fișierelor pentru I/O
    fclose(fin);
    fclose(fout);

    return 0;
}
