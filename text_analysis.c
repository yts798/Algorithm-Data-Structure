/* * * * * * *
 * Text Analysis module for Assignment 2.
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
#include <math.h>

#include "text_analysis.h"

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
void problem_2_a() {
  int str_num;
  trie_t *root;  

  scanf("%d\n", &str_num);
  root = build_tries(str_num);
  /* pre order traversal of trie */
  pre_order(root);
  free(root);
  return;
}

/* This functions receives input words and insert them into a trie */ 
trie_t *build_tries(int num) {
  char str[MAX_LENGTH_2+1];
  int i, size;
  trie_t *root;
  root = new_trie(ROOT);
  /* insert words */
  for (i=0;i<num;i++) {
    scanf("%s\n", str);
    size = strlen(str);
    insert_trie(root, str);
  }
  return root;
}

/* This function inserts a new value to the trie */
void insert_trie(trie_t *root, char *str) {
  int i, size=strlen(str);
  trie_t *curr = root, *old;
  curr->freq += 1;

  /* insert every character of the word into trie */
  for(i=0;i<size;i++) {
    if (curr->character[str[i]-SHIFT] == NULL) {
      curr->character[str[i]-SHIFT] = new_trie(str[i]);
    }
    old = curr;
    curr = curr->character[str[i]-SHIFT];
    curr->prev = old;
    curr->freq += 1;
  }
  
  /* finalise insertion by adding a $ at the end */ 
  if (curr->character[0] == NULL) {
    curr->character[0] = new_trie(LEAF);
  }
  old = curr;
  curr = curr->character[0];
  curr->prev = old;
  curr->freq += 1;
}

/* This functions builds a new trie */
trie_t *new_trie(char data) {
  int i;
  trie_t *trie = (trie_t*)malloc(sizeof(*trie));

  /* initialise parameters */
  trie->data = data;
  trie->freq = 0;
  trie->prev = NULL;
  for (i=0;i<CHAR_NUM+1;i++) {
    trie->character[i] = NULL;
  }
  return trie;
}

/* This functions prints the pre-order traversal of the trie */
void pre_order(trie_t *curr) { 
  int i;
  printf("%c\n", curr->data);
  /* reach the end of word */
  if (curr->data==LEAF) {
    return;
  }
  else {
    for (i=0;i<CHAR_NUM+1;i++) {
      if (curr->character[i]!=NULL) {
        pre_order(curr->character[i]);
      }
    }
  }
}

/* This function frees the trie */
void free_trie(trie_t *curr) {
  int i;
  if (curr==NULL) {
    return;
  }
  for (i=0;i<CHAR_NUM+1;i++) {
    free_trie(curr->character[i]);
  }
  free(curr);
}

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
void problem_2_b() {
  int str_num, pref_num;
  char *str;
  trie_t *root;

  scanf("%d %d\n", &str_num, &pref_num);
  str = (char *)malloc((pref_num+1)*sizeof(str));
  /* start searching the trie and preserve all prefixs */
  root = build_tries(str_num);
  find_prefix_freq(root, str, ROOT_LEVEL, pref_num);

  free(str);
  free(root);
  return;
}

/* This function searches the trie to output all prefix with required length
and its frequency */
void find_prefix_freq(trie_t *curr, char *str, int level, int pref) {
  int i;
  /* save prefix value */
  if (level>0) {
    str[level-1] = curr->data;
  }

  /* output the prefix and its frequency when it is as long as required */
  if (level==pref) {
    str[level] = '\0';
    printf("%s", str);
    printf(" %d\n", curr->freq);
  }
  /* keep searching in the trie */
  else {
    for (i=1;i<CHAR_NUM+1;i++) {
      if (curr->character[i]!=NULL) {
        find_prefix_freq(curr->character[i], str, level+1, pref);
      }
    }
  }
}

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
void problem_2_c() {
  int str_num, stub_size;
  char *stub, str[MAX_LENGTH_2+1];
  trie_t *root;

  scanf("%d\n", &str_num);
  scanf("%s\n", str);

  /* preserve stub */
  stub_size = strlen(str);
  stub = (char *)malloc(stub_size*sizeof(str));
  strcpy(stub, str);

  /* build trie and start searching */
  root = build_tries(str_num);
  search_stub(root, stub);
  free(stub);
  free(root);
  return;
}

/* This function locates the stub in the trie and start searching the highest 
frequency word starting with the stub */
void search_stub(trie_t *root, char *stub) {
  trie_t *curr = root;
  int i, size=strlen(stub), max_freq, find=0;
  int stub_freq;

  /* search the stub in trie and find its total frequency */
  for (i=0;i<size;i++) {
    curr = curr->character[stub[i]-SHIFT];
  }
  stub_freq = curr->freq;

  /* find 5 most frequent words starting with stub */
  while (find<5) {
    max_freq = 0;
    /* find and record the most frequent word starting with stub in the trie */
    find_max_freq(curr, &max_freq);
    /* if there is not such word we stop */
    if (max_freq==0) {
      break;
    }
    /* locate the word starting with sub that have highest frequency */
    find_largest(curr, max_freq, &find, stub_freq);
  }
}

/* This function starts at the last character of stub in the trie, find the
subsequent highest word frequency */
void find_max_freq(trie_t *curr, int *max_freq) {
  int i;
  /* when reach the end of the word, check whether we get a higher frequency */
  if (curr->data==LEAF) {
    if (curr->freq > *max_freq) {
      *max_freq = curr->freq;
    } 
    return;
  }
  /* keep traversing to reach the end of words */
  else {
    for(i=0;i<CHAR_NUM+1;i++) {
      if (curr->character[i]!=NULL) {
        find_max_freq(curr->character[i], max_freq);
      }
    }
  }
}

/* This function finds all words which starts with stub and has the 
matched frequency frequency */
void find_largest(trie_t *curr, int max_freq, int *find, int total_freq) {
  int i;
  /* check whether this word has the required frequency */ 
  if (curr->data==LEAF) {
    if (curr->freq == max_freq) {
      *find += 1;
      int one_freq = curr->freq;
      /* remove its frequency */
      curr->freq = 0;
      /* start backtracking the word */
        if (*find<=5) {
        double percent = one_freq;
        printf("%.2lf ", percent/total_freq);
        back_track(curr);
      }
    }
  }
  /* keep traversing to reach the end of words */
  else {
    for (i=0;i<CHAR_NUM+1;i++) {
      if (curr->character[i]!=NULL) {
        find_largest(curr->character[i], max_freq, find, total_freq);
      }
    }
  }
}

/* This function backtracks and outputs a word from its end point in the trie */
void back_track(trie_t *curr) {
  char str[MAX_LENGTH_2+1];
  int str_index=0, i;

  /* adding element of word reversely until reaching start point */
  curr = curr->prev;
  while (curr->data!=ROOT) {
    str[str_index++] = curr->data;
    curr = curr->prev;
  }
  /* outputs the string reversely */
  for (i=str_index-1;i>=0;i--) {
    printf("%c", str[i]);
  }
  printf("\n");
}

/* Algorithms are Fun! */
