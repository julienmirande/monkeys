// file: cell.h
// date: mar. oct. 23 14:51:10 CEST 2018
// authors: jmirande / adanquigny

#ifndef H_CELL
#define H_CELL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 50
#define MAX_CELLS 20000
#define MAX_LABEL_LENGTH 50

struct cell {
  char word[MAX_WORD_LENGTH+1];    // a char array for words of length MAX_WORD_LENGTH
  int  noccs;                      // an integer for counting occurrences
  char label[MAX_LABEL_LENGTH+1];  // a char array to contain the word label (NLTK)
  struct cell* next;               // a (possibly NULL) pointer to the following cell
  struct queue* successors_queue;  // point to his successors queue
};

struct pool {
  struct cell m[MAX_CELLS];      // an array of memory cells
  struct cell* next_free;        // a pointer to the first free cell
};

// To print a cell
void print_cell(struct cell* cell);

// Return the lenght of cell
int length(const struct cell* c);

//return if cell word is a name
int is_a_name(struct cell *cell);

//return if cell word is a conjugated verb
int is_a_conjugated_verb(struct cell *cell);

#endif
