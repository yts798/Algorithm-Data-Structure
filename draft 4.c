/* * * * * * *
 * Park Ranger module for Assignment 1
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by <Insert Name Here>
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "parkranger.h"
#include "util.h"

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
//   0 1                         /  2
//   0 2                        / /
//   2 1                        1
//   1 3                          \
//                                 3
//
// In this example your program should return `true` as there is a way to trim
// all trees in a single run. This run is (0, 2, 1, 3).
//
// Your function should must:
//  - Read in this data from stdin
//  - Store this data in an appropriate data structure
//  - Run the algorithm you have designed to solve this problem
//  - Do any clean up required (e.g., free allocated memory)
//  - Return `true` or `false` (included in the stdbool.h library)
//
// For full marks your algorithm must run in O(n + m) time.

bool is_single_run_possible() {
  int vertice, edge, i, start, end, vert_now, vert_next, ajc_vert;
  int can_trim=0;
  graph_t *graph;
  node_t *temp;
  scanf("%d %d",&vertice, &edge);
  vertice += 1;
  graph = new_graph(vertice);
  for(i=0;i<edge;i++) {
    scanf("%d %d",&start, &end);
    add_edge(graph, start, end);
  }
  print_graph(graph);
  dfs(graph, STARTPOINT);

  for(i=0;i<vertice;i++) {
    if(!graph->visited[i]) {
      return false;
    }
  }

  printf("the topological order is:");
  for (i=0;i<vertice;i++) {
    printf("%d->", graph->topo_order[i]);
  }
  printf("\n");
  for (i=0;i<vertice-1;i++) {
    vert_now = graph->topo_order[i];
    vert_next = graph->topo_order[i+1];
    temp = graph->adjlsts[vert_now];
    while(temp) {
      ajc_vert = temp->vertice;
      if (vert_next==ajc_vert) {
        can_trim = 1;
      }
      temp=temp->next;
    }
    if (!can_trim) {
      return false;
    }
    vert_now = vert_next;
    can_trim = 0;
  }
  return true;
}

graph_t *new_graph(int vertice) {
  graph_t *graph;
  int i;
  graph = (graph_t*)malloc(sizeof(*graph));
  graph->v_num = vertice;
  graph->visited = (int *)malloc(vertice*sizeof(int));
  graph->adjlsts =  (node_t**)malloc(vertice*sizeof(node_t*));
  graph->topo_order = (int *)malloc(vertice*sizeof(int));
  graph->topo_index = vertice;
  for (i=0;i<vertice;i++) {
    graph->visited[i] = 0;
    graph->adjlsts[i] = NULL;
    graph->topo_order[i] = -1;
  }
  return graph;
}

void add_edge(graph_t *graph, int start, int end) {
  node_t *new;
  new = (node_t*)malloc(sizeof(*new));
  new->vertice = end;
  new->next = graph->adjlsts[start];
  graph->adjlsts[start] = new;
}

void dfs(graph_t *graph, int vertex) {
  int ajc_vertex;
  node_t *temp;
  graph->visited[vertex]=1;
  printf("Visited %d \n", vertex);
  temp = graph->adjlsts[vertex];
  while(temp) {
    ajc_vertex=temp->vertice;
    if(graph->visited[ajc_vertex]==0){
      dfs(graph, ajc_vertex);
    }
    temp=temp->next;
  }
  graph->topo_order[--graph->topo_index]=vertex;
}

void print_graph(graph_t *graph) {
  int i;
  node_t *temp;
  for(i=0;i<graph->v_num;i++) {
    temp = graph->adjlsts[i];
    printf("\n Adjacency list of vertex %d\n ", i);
    while(temp) {
      printf("%d -> ", temp->vertice);
      temp=temp->next;
    }
    printf("\n");
  }
}
