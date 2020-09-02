//
// file: monkeys.h
// date: mar. oct. 23 14:51:10 CEST 2018
// authors: jmirande / adanquigny
//
//

#ifndef H_MONKEYS
#define H_MONKEYS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include "cell.h"
#include "queue.h"

#define NB_ROUNDS_INACTIVE 50


struct files{
  FILE * file_1;
  FILE * file_2;
};

struct monkeys{
  void (*monkey_reader_1)(FILE *, struct queue*, struct pool*, struct pool_queue*, int*);
  void (*monkey_reader_2)(FILE *, struct queue*, struct pool*, struct pool_queue*, int*);
  void (*monkey_statistician)(struct queue*, struct queue*, struct pool*, struct pool_queue*, FILE *);
  void (*monkey_writer_1)(struct queue*, struct queue*, struct pool*, struct pool_queue*);
  void (*monkey_writer_2)(struct queue*, struct queue*, struct pool*, struct pool_queue*);
  void (*monkey_printer)(struct queue*, int*);
};

// Return if a monkey reader is active or not
int is_monkey_reader_active(FILE * file);

// Do the job of a monkey reader : read one word and push it in the queue
void monkey_reader(FILE * file, struct queue* queue, struct pool* pool, struct pool_queue* pool_queue, int* nb_words_read);

// Return is monkey reader is active or not
int is_monkey_statistician_active(struct queue* queue);

// Do the job of a monkey statistician : compute results for one word
void monkey_statistician(struct queue* queue, struct queue* occ_queue, struct pool* pool, struct pool_queue* pool_queue,FILE *file);

// Return if a monkey righter is active or not
int is_monkey_writer_active(int nb_rounds);

// Do the job of a monkey writer: generates random sentences using statistician queue
void monkey_writer(struct queue* occ_queue, struct queue* writer_queue, struct pool* pool, struct pool_queue* pool_queue);

// Return if a monkey reader is active or not
int is_monkey_printer_active(struct queue* writer_queue);

// Do the job of a monkey printer : pop a word from the queue and print it
void monkey_printer(struct queue* writer_queue, int* nb_words_printed);

// Return if at least a monkey is active or not
int is_at_least_one_monkey_active(struct files* files, struct queues* queues, struct queues * writer_queues, int nb_rounds);

#endif
