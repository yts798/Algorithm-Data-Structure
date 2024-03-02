/* * * * * * *
 * Park Ranger module for Assignment 1
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by <Yutong Sun>
 */

#ifndef PARKRANGER_H
#define PARKRANGER_H

#include <stdbool.h>


// This function must read in a ski slope map and determine whether or not
// it is possible for the park ranger to trim all of the trees on the ski slope
// in a single run starting from the top of the mountain.
//
// The ski slope map is provided via stdin in the following format:
//
//   n m
//   from to
//   from to
//   ...
//   from to
//
// Here n denotes the number of trees that need trimming, which are labelled
// {1, ..., n}. The integer m denotes the number "reachable pairs" of trees.
// There are exactly m lines which follow, each containing a (from, to) pair
// which indicates that tree `to` is directly reachable from tree `from`.
// `from` and `to` are integers in the range {0, ..., n}, where {1, ..., n}
// denote the trees and 0 denotes the top of the mountain.
//
// For example the following input represents a ski slope with 3 trees and
// 4 reachable pairs of trees.
//
// input:            map:          0
//   3 4                          / \
//   0 1                         /  1
//   0 2                        / /
//   1 2                        2
//   2 3                          \
//                                 3
//
// In this example your program should return `true` as there is a way to trim
// all trees in a single run. This run is (0, 1, 2, 3).
//
// Your function should must:
//  - Read in this data from stdin
//  - Store this data in an appropriate data structure
//  - Run the algorithm you have designed to solve this problem
//  - Do any clean up required (e.g., free allocated memory)
//  - Return `true` or `false` (included in the stdbool.h library)
//
// For full marks your algorithm must run in O(n + m) time.

#define STARTPOINT 0

/* adjacency list is made of node_single */
typedef struct node_single node_t;

struct node_single {
	int vertice;
	node_t *next;
};

/* Data of nodes and trees will be saved to graph*/
typedef struct graph graph_t;

struct graph {
    int v_num;
    int *visited;
    node_t **adjlsts;
    int *topo_order;
    int topo_index;
};

/* Determine whether a single run is possible for park ranger */
bool is_single_run_possible();

/* Create graph and save edges to adjacency list for dfs search */
graph_t *new_graph(int vertice);

/* Add an edge from starting point to ending point in graph's adjacency lists */
void add_edge(graph_t *graph, int start, int end);

/* Do depth-first search and record popped off order of nodes reversely */
void dfs(graph_t *graph, int vertice);

/* Free the allocated memory of graph */
void free_graph(graph_t *graph);

#endif
