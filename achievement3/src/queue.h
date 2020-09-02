// file: queue.h
// date: mar. oct. 23 14:51:10 CEST 2018
// authors: jmirande / adanquigny

#ifndef H_QUEUE
#define H_QUEUE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cell.h"

#define MAX_QUEUE 20000
#define NB_PUNCTUATION 7

struct queue {
  struct cell* first;
  struct cell* last;
};

struct pool_queue {
  struct queue m[MAX_QUEUE];      // an array of memory queue
  struct queue* next_free;        // a pointer to the first free queue
};

struct queues{
  struct queue* queue_1;
  struct queue* queue_2;
};

// To print a cell
void print_cell(struct cell* cell);

// Create a new cell using a pool as allocation
struct cell* create_new_cell(struct pool *pool,struct pool_queue* pool_queue,
                             char word[], int noccs, char label[],
                             struct cell* next);

// Return the lenght of cell
int length(const struct cell* c);

// To print a queue
void print_queue(struct queue queue);

// To create a queue without using array as allocation
struct queue* create_new_queue(struct pool_queue *pool_queue, struct cell* first,
                             struct cell* last);

// return if a queue is empty or not
int empty_queue(struct queue* queue);

// check if a cell containing a special word exists in a queue
int cell_exist(struct queue* queue,char * word);

// push a cell into a queue
void push(struct queue* queue,struct pool* pool,struct pool_queue* pool_queue, struct cell cell);

// push a cell with paramters into a queue
void push_custom_cell(char new_word[], struct queue* queue, struct pool* pool, struct pool_queue* pool_queue, int noccs, char new_label[]);

// pop a cell for the queue
struct cell* pull(struct queue* queue);

// increments the number of occurence of a cell in the queue
void increment_occ_queue(struct queue* occ_queue, char * word);

// return if a the queue contains a punctuation
int queue_contains_punctuation(struct queue* successors_queue);

// return if a word is a punctation
int word_is_punctuation(char word[]);

// Sum the noccs of the different cells noccs of the queue
int sum_successors(struct cell* cell_occ);

// replace punctuation by exclamation when there is only one word to print
void replace_by_exclamation(struct queue* writer_queue);

// return if there is a name in the queue
int is_there_a_name(struct queue *writer_queue);

// return if there is a conjugated verb in the queue
int is_there_a_conjugated_verb(struct queue *writer_queue);

// find the min into a queue
int find_min(struct queue queue);

// find the max into a queue
int find_max(struct queue queue);

// push all the cells containing a special value from the source queue into a destination queue
void fill_queue(int value, struct queue* queueDst, struct queue queueSrc, struct pool* pool, struct pool_queue* pool_queue);

// find the min and then create a queue with all the cells containing the value
void min(struct queue* queueDst, struct queue queueSrc, struct pool* pool, struct pool_queue* pool_queue);

// find the max and then create a queue with all the cells containing the value
void max(struct queue* queueDst, struct queue queueSrc, struct pool* pool, struct pool_queue* pool_queue);


#endif
