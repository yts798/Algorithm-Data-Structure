/* * * * * * *
 * Hashing module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by <Yutong Sun>
 */

#ifndef HASH_H
#define HASH_H

#define MAX_LENGTH 256
#define INIT_SIZE 8
#define ASCII_A 97
#define ASCII_Z 122
#define MAP_LOWER 97
#define MAP_UPPER 39
#define MAP_DIGIT 4
#define BINARY_STRING 64
#define HASH_INCREASE 2

// Implements a solution to Problem 1 (a), which reads in from stdin:
//   N M
//   str_1
//   str_2
//   ...
//   str_N
// And outputs (to stdout) the hash values of the N strings 1 per line.
void problem_1_a();

/* This functions use horner's rule to hash a long string then print 
hash value */
void hash_string(int table);

/* This functions uses horner's rule to compute hash value */
int hash_long_num(int *map, int str_size, int table);

/* This functions returns a int array which each integer represents the map
value of each character of input array */
int *map_char(char *str, int size);

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
void problem_1_b();

/* This functions initiates processing order with 1, 2, 3, ..., n-1 */
void init_processing(int *processing_order, int str_num);

/* This functions resets already inserted array to -1 */
void reset_inserted(int *inserted, int table_size);

/* This functions save input strings into a dynamic 2d array */
char **save_strings(int str_num);

/* This function inserts a string to the according position of hash table and 
update inserted strings */
void hash_to_table(char **table, char **str_list, int val, int size,
 int *process, int *inserted, int *index);

/* This function checks whether a string can be inserted into table by 
shifting limited times */
int attempt_shift(int step, int table_size, int val, char **table, 
 char **str_list, int one_size, int *process, int *inserted, int *index);

/* This function resets the hash table value and update processing order which
is used for next round of hash */
void modify_table_order(int size, char **table, int *process, int *inserted);

/* This function present the hash table */
void present_table(int table_size, char **hash_table);

/* This function frees the hash table */
void free_table(char **table, int size);

#endif
