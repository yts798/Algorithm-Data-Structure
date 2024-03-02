/* * * * * * *
 * Hashing module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by <Yutong Sun>
 */


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "hash.h"

// Implements a solution to Problem 1 (a), which reads in from stdin:
//   N M
//   str_1
//   str_2
//   ...
//   str_N
// And outputs (to stdout) the hash values of the N strings 1 per line.
void problem_1_a() {
  int table_size, str_num, i;
  scanf("%d %d\n", &str_num, &table_size);
  for (i=0; i<str_num; i++) {
    hash_string(table_size);
  }
  return;
}

/* This functions use horner's rule to hash a long string then print 
hash value */
void hash_string(int table) {
  int str_size, hash_val, *map;
  char str[MAX_LENGTH+1];
  scanf("%s\n", str);
  str_size = strlen(str);
  /* map each character to corresponding integer */
  map = map_char(str, str_size);
  /* use horner's rule to compute hash value */
  hash_val = hash_long_num(map, str_size, table);
  printf("%d\n", hash_val);
  free(map);
}

/* This functions uses horner's rule to compute hash value */
int hash_long_num(int *map, int str_size, int table) {
  int hash_val = map[0], i;
  /* do not need to use horner's rule for only one characater */
  if(str_size==1) {
    return hash_val % table;
  }
  /* horner's rule application*/
  for (i=1;i<str_size;i++) {
    hash_val = (hash_val * BINARY_STRING + map[i]) % table;
  }
  return hash_val;
}

/* This functions returns a int array which each integer represents the map
value of each character of input array */
int *map_char(char *str, int size) {
  int i, *map;
  char c;
  map = (int *)malloc(size*sizeof(map));

  for (i=0;i<size;i++) {
    c = str[i];
    /* map different categories with different transformations */
    if (isalpha(c)) {
      if (c>=ASCII_A && c<=ASCII_Z) {
        map[i] = c - MAP_LOWER;
      }
      else {
        map[i] = c - MAP_UPPER;
      }
    }
    else {
      map[i] = c + MAP_DIGIT;
    }
  }
  return map;
}

// Implements a solution to Problem 1 (b), which reads in from stdin:
//   N M K
//   str_1
//   str_2
//   ...
//   str_N
// Each string is inputed (in the given order) into a hash table with size
// M. The collision resolution strategy must be linear probing with step
// size K. If an element cannot be inserted then the table size should be
// doubled and all elements should be re-hashed (in index order) before
// the element is re-inserted.
//
// This function must output the state of the hash table after all insertions
// are performed, in the following format
//   0: str_k
//   1:
//   2: str_l
//   3: str_p
//   4:
//   ...
//   (M-2): str_q
//   (M-1):
void problem_1_b() {
  int str_num, table_size, step, one_size, hash_val;
  int hash_finish=0, rehash=1, index=0;
  int *one_map, *inserted_order, *processing_order;
  char **hash_table, **str_list; 

  scanf("%d %d %d\n", &str_num, &table_size, &step);

  hash_table = (char **)malloc(table_size*sizeof(hash_table));
  inserted_order = (int *)malloc(table_size*sizeof(inserted_order));
  processing_order = (int *)malloc(str_num*sizeof(processing_order));

  /* initiate processing order with 1, 2, 3, ..., n-1 */
  init_processing(processing_order, str_num);
  /* save input strings into a dynamic 2d array */
  str_list = save_strings(str_num);
  /* reset already inserted array to -1 */
  reset_inserted(inserted_order, table_size);

  /* in the hashing process, the order of hashing string is guided by 
  processing array. If rehash is needed, hashing will start over with its 
  processing array updated */
  while (!hash_finish) {
    rehash = 1;
    /* compute hash value of string, according to processing order */
    one_size = strlen(str_list[processing_order[index]]);
    one_map = map_char(str_list[processing_order[index]], one_size);
    hash_val = hash_long_num(one_map, one_size, table_size);

    /* if no collision, insert into table */
    if (hash_table[hash_val]==NULL) {
      hash_to_table(hash_table, str_list, hash_val, one_size, processing_order,
       inserted_order, &index);
      rehash = 0;
    }

    /* try whether can insert to another space with a distance of step size */
    else {
      rehash = attempt_shift(step, table_size, hash_val, hash_table, str_list, 
      one_size, processing_order, inserted_order, &index);
    }
    
    /* rehash, update processing order with inserted order and start over */
    if (rehash) {
      /* clean up hash table and update processing order */
      modify_table_order(table_size, hash_table,
       processing_order, inserted_order);
       /* double hash table size */
      table_size *= HASH_INCREASE;
      hash_table = realloc(hash_table, table_size*sizeof(hash_table));
      /* reset inserted order to -1 */
      inserted_order = realloc(inserted_order, 
      table_size*sizeof(inserted_order));
      reset_inserted(inserted_order, table_size);
      /* start hashing first element in processing order*/
      index = 0;
    }

    /* check whether hashing completes*/
    if (index==str_num) {
      hash_finish = 1;
    }
  }

  present_table(table_size, hash_table);

  free(one_map);
  free(inserted_order);
  free(processing_order);
  free_table(hash_table, table_size);
  free_table(str_list, str_num);
  return;
}


/* This functions initiates processing order with 1, 2, 3, ..., n-1 */
void init_processing(int *processing_order, int str_num) {
  int i;
  for (i=0;i<str_num;i++) {
      processing_order[i]=i;
  }
}

/* This functions resets already inserted array to -1 */
void reset_inserted(int *inserted, int table_size) {
  int i;
  for (i=0;i<table_size;i++) {
      inserted[i]=-1;
  }
}

/* This functions save input strings into a dynamic 2d array */
char **save_strings(int str_num) {
  char **str_list, one_str[MAX_LENGTH+1];
  int i, one_size;
  str_list = (char **)malloc(str_num*sizeof(str_list));
  /* save input strings */
  for (i=0;i<str_num;i++) {
    scanf("%s\n", one_str);
    one_size = strlen(one_str);
    str_list[i] = (char *)malloc((one_size+1)*sizeof(char));
    strcpy(str_list[i], one_str);
  }
  return str_list;
}

/* This function inserts a string to the according position of hash table and 
update inserted strings */
void hash_to_table(char **table, char **str_list, int val, int size,
 int *process, int *inserted, int *index) {
  table[val] = (char *)malloc((size+1)*sizeof(char));
  strcpy(table[val], str_list[process[*index]]);
  inserted[val] = process[*index];
  *index += 1;
}

/* This function checks whether a string can be inserted into table by 
shifting limited times */
int attempt_shift(int step, int table_size, int val, char **table, 
 char **str_list, int one_size, int *process, int *inserted, int *index) {
  int traverse = 0;
  /* if has traversed the whole graph, stop and go to rehash */
  while (traverse < table_size) {
    val += step;
    traverse += step;
    val = val % table_size; /* make sure val is less than table size */
    /* find a insert position */
    if (table[val]==NULL) {
      hash_to_table(table, str_list, val, one_size, process,
      inserted, index);
      return 0;
    }
  }
  return 1;
}

/* This function resets the hash table value and update processing order which
is used for next round of hash */
void modify_table_order(int size, char **table, int *process, int *inserted) {
  int i, index=0;
  for (i=0;i<size;i++) {
    table[i] = NULL;
  }
  /* modify the order of processing strings in the next round of hash */
  for (i=0;i<size;i++) {
    if (inserted[i]!=-1) {
      process[index++] = inserted[i];
    }
  }
}

/* This function present the hash table */
void present_table(int table_size, char **hash_table) {
  int i;
  for (i=0;i<table_size;i++) {
    if (hash_table[i]!=NULL) {
      printf("%d: %s\n", i, hash_table[i]);
    }
    else {
      printf("%d:\n", i);
    }
  }
}

/* This function frees the hash table */
void free_table(char **table, int size) {
  int i;
  for (i=0;i<size;i++) {
    free(table[i]);
  }
  free(table);
}

/* Algorithms are Fun! */