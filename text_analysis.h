/* * * * * * *
 * Text Analysis module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by <Yutong Sun>
 */

#ifndef TEXT_ANALYSIS_H
#define TEXT_ANALYSIS_H

#define MAX_LENGTH_2 99
#define CHAR_NUM 26
#define ROOT '^'
#define LEAF '$'
#define SHIFT 96
#define ROOT_LEVEL 0

typedef struct trie trie_t;

struct trie {
  int freq;
  char data;
  trie_t *prev;
  trie_t *character[CHAR_NUM+1];
};

// Build a character level trie for a given set of words.
//
// The input to your program is an integer N followed by N lines containing
// words of length < 100 characters, containing only lowercase letters.
//
// Your program should built a character level trie where each node indicates
// a single character. Branches should be ordered in alphabetic order.
//
// Your program must output the post-order traversal of the characters in
// the trie, on a single line.
void problem_2_a();

/* This functions receives input words and insert them into a trie */ 
trie_t *build_tries(int num);

/* This functions inserts a new value to the trie */
void insert_trie(trie_t *root, char *str);

/* This functions builds a new trie */
trie_t *new_trie(char data);

/* This functions prints the pre-order traversal of the trie */
void pre_order(trie_t *root);

/* This function frees the trie */
void free_trie(trie_t *curr);

// Using the trie constructed in Part (a) this program should output all
// prefixes of length K, in alphabetic order along with their frequencies
// with their frequencies. The input will be:
//   n k
//   str_0
//   ...
//   str_(n-1)
// The output format should be as follows:
//   an 3
//   az 1
//   ba 12
//   ...
//   ye 1
void problem_2_b();

/* This function searches the trie to output all prefix with required length
and its frequency */
void find_prefix_freq(trie_t *curr, char *str, int level, int pref);

// Again using the trie data structure you implemented for Part (a) you will
// provide a list (up to 5) of the most probable word completions for a given
// word stub.
//
// For example if the word stub is "al" your program may output:
//   0.50 algorithm
//   0.25 algebra
//   0.13 alright
//   0.06 albert
//   0.03 albania
//
// The probabilities should be formatted to exactly 2 decimal places and
// should be computed according to the following formula, for a word W with the
// prefix S:
//   Pr(word = W | stub = S) = Freq(word = W) / Freq(stub = S)
//
// The input to your program will be the following:
//   n
//   stub
//   str_0
//   ...
//   str_(n-1)
// That is, there are n + 1 strings in total, with the first being the word
// stub.
//
// If there are two strings with the same probability ties should be broken
// alphabetically (with "a" coming before "aa").
void problem_2_c();

/* This function locates the stub in the trie and start searching the highest 
frequency word starting with the stub */
void search_stub(trie_t *root, char *stub);

/* This function starts at the last character of stub in the trie, find the
subsequent highest word frequency */
void find_max_freq(trie_t *curr, int *max_freq);

/* This function finds all words which starts with stub and has the 
matched frequency frequency */
void find_largest(trie_t *curr, int max_freq, int *find, int total_freq);

/* This function backtracks and outputs a word from its end point in the trie */
void back_track(trie_t *curr);

#endif
