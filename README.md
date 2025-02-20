# DSA Suffix Tree Operations

This project implements various operations on suffix trees (tries) in C as part of a university assignment. The program builds a suffix tree from input words and performs one of the following tasks:

- **Display Tree**: Print the tree structure level by level.
- **Tree Statistics**: Compute statistics such as the number of leaf nodes, the count of suffixes with a specified length, and the maximum number of descendants of any node.
- **Suffix Search**: Check whether given substrings appear as valid suffixes in the tree.
- **Tree Compression**: Compress the tree by merging nodes in chains.

## Features

- **Suffix Tree Construction**
  Every suffix of each input word is added to the tree. For example, for "banana" the construction includes:
  `banana$ → anana$ → nana$ → ana$ → na$ → a$ → $`

- **Viewing the Tree Structure**
  The tree is displayed level by level using a breadth-first search implemented with a custom queue.

- **Statistics Calculation**
  Traversal of the tree computes:
  - The number of leaf nodes.
  - The number of suffixes of a specified length (using the terminal character '$').
  - The maximum number of children (descendants) of any node.

- **Suffix Existence Check**
  The program can search for a suffix in the tree and report whether it is valid.

- **Tree Compression**
  The tree can be compressed by merging nodes that have a single child (except those marking the end of a suffix), thereby reducing its size.

## Files Overview

- **[tema2.c](tema2.c)**
  Contains the `main()` function, processes command-line arguments, and selects the appropriate task:
  - `-c1` for displaying the tree.
  - `-c2` for computing tree statistics (with additional parameter `k`).
  - `-c3` for suffix search.
  - `-c4` for tree compression.

- **[tree.c](tree.c)** and **[tree.h](tree.h)**
  Implement the suffix tree and queue data structures along with functions to:
  - Create, destroy, and display the tree.
  - Add every suffix for each word.
  - Compute tree statistics.
  - Search for a suffix.
  - Compress the tree.

- **[Makefile](Makefile)**
  A simple Makefile is provided to build the project and clean up build artifacts.

- **[checker.sh](checker.sh)**
  A shell script to run tests (provided in the `tests/` directory) and perform Valgrind checks for memory leaks.

- **tests/**
  Contains input and reference output files for the different tasks (cerinta1 through cerinta4).

## Building the Project

Use the provided Makefile to build the project:

```bash
make build   # Builds the executable 'tema2'
make clean   # Removes build artifacts
```

## Running the Program

The program supports four commands corresponding to its functionalities. After building, run the executable with the following formats:

1. **Display Tree (Cerinta 1)**
   ```bash
   ./tema2 -c1 input_file output_file
   ```

2. **Tree Statistics (Cerinta 2)**
   Note that an additional integer (`k`) is required (the number appears at a different position in the argument list):
   ```bash
   ./tema2 -c2 k input_file output_file
   ```

3. **Suffix Search (Cerinta 3)**
   ```bash
   ./tema2 -c3 input_file output_file
   ```

4. **Tree Compression (Cerinta 4)**
   ```bash
   ./tema2 -c4 input_file output_file
   ```

For example, to display the suffix tree built from [tests/input/cerinta1/test0.in](tests/input/cerinta1/test0.in) and write the output to [tests/output/cerinta1/test.out](tests/output/cerinta1/test.out), run:
```bash
./tema2 -c1 tests/input/cerinta1/test0.in tests/output/cerinta1/test.out
```

## Implementation Details

- **Suffix Tree Construction & Traversal**
  - The function [`add_every_suffix`](tree.c) generates all suffixes by appending a terminal character (`$`) to each word.
  - [`show_tree`](tree.c) uses a breadth-first search with a custom queue (implemented via the types defined in [`tree.h`](tree.h)) to print tree levels.

- **Statistics Calculation**
  - The function [`get_tree_stats`](tree.c) traverses the tree to compute the number of leaf nodes, count the suffixes of a given length (`k`), and determine the maximum value of [`num_children`](tree.h) in any node.

- **Suffix Search & Tree Compression**
  - [`search_suffix`](tree.c) iteratively checks for the presence of each character in the suffix.
  - [`compress_tree`](tree.c) recursively compresses the tree by merging nodes with a single child, reallocating the node information to reflect the concatenated labels.

- **Memory Management**
  - Dynamic memory allocation is used throughout for nodes and the queue. Proper cleanup is ensured in [`destroy_tree`](tree.c) and [`destroy_queue`](tree.c).

## Testing

A comprehensive set of test cases is provided in the [`tests`](tests) directory, organized by task (cerinta1, cerinta2, cerinta3, cerinta4). Run the provided [`checker.sh`](checker.sh) script to execute tests and check for memory leaks using Valgrind.

---

This repository contains all source code and test files needed to build, run, and validate the suffix tree operations implemented for this assignment.