
// file: main.c
// date: mar. oct. 23 14:51:10 CEST 2018
// authors: jmirande / adanquigny


#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   // pour rand
#include "queue.h"
#include "cell.h"
#include "monkeys.h"

#define NB_MONKEYS 6
#define MAX_NB_ROUNDS 1000

// Global seed for the random number generator
static int seed = 0;

// Differents kind of monkeys
enum MONKEYS { READER_1, READER_2, STATISTICIAN, WRITER_1, WRITER_2, PRINTER};

/**********************************
* Function for parsing the options of the program
* Currently available options are :
* -s <seed> : sets the seed
************************************/
void parse_opts(int argc, char* argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "s:")) != -1) {
    switch (opt) {
      case 's':
      seed = atoi(optarg);
      break;
      default:
      fprintf(stderr, "Usage: %s filename [-s seed] \n",argv[0]);
      exit(EXIT_FAILURE);
    }
  }
}

/**********************************
* Function who generates a rundom number
* between 0 and 2 and ask the selected
* monkey to do his job if he is active
************************************/
void work(struct files* files, struct pool* pool, struct pool_queue* pool_queue,
  struct queues* queues, struct queue* occ_queue, struct queues* writer_queues,
  int* nb_words_read, int* nb_words_printed, int nb_rounds,
  struct monkeys * monkeys)
  {
  // random bewteen 0 and 2
  int randomMonkey = rand()%NB_MONKEYS; // select a monkey between 0 & 3

  switch (randomMonkey){

    // READERS
    case READER_1:
    if ( is_monkey_reader_active(files->file_1) ){
      monkeys->monkey_reader_1(files->file_1, queues->queue_1, pool, pool_queue, nb_words_read);
    }
    break;
    ////
    case READER_2:
    if ( is_monkey_reader_active(files->file_2) ){
      monkeys->monkey_reader_2(files->file_2, queues->queue_2, pool, pool_queue, nb_words_read);
    }
    break;
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // STATISTICIAN
    case STATISTICIAN:

    if (rand()%2 == 0){

      if (is_monkey_statistician_active(queues->queue_1)){
        monkeys->monkey_statistician(queues->queue_1, occ_queue, pool, pool_queue, files->file_1);
      }
    }else{

      if (is_monkey_statistician_active(queues->queue_2)){
        monkeys->monkey_statistician(queues->queue_2, occ_queue, pool, pool_queue, files->file_2);
      }
    }
    break;

    // WRITERS
    case WRITER_1:
    if ( is_monkey_writer_active(nb_rounds) ){
        //printf("WRITER" );
      monkeys->monkey_writer_1(occ_queue, writer_queues->queue_1, pool, pool_queue);
    }
    break;
    ////
    case WRITER_2:
    if ( is_monkey_writer_active(nb_rounds) ){
        //printf("WRITER" );
      monkeys->monkey_writer_2(occ_queue, writer_queues->queue_2, pool, pool_queue);
    }
    break;
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // PRINTER
    case PRINTER:

    if (rand()%2 == 0){
      if (is_monkey_printer_active(writer_queues->queue_1)){
        monkeys->monkey_printer(writer_queues->queue_1, nb_words_printed);
      }
    }else{

      if (is_monkey_printer_active(writer_queues->queue_2)){
        monkeys->monkey_printer(writer_queues->queue_2, nb_words_printed);
      }
    }

    break;

  }
}

/**********************************
* Function who prints a queue using
* a special format
************************************/
void print_min_max_format(struct queue queue){
  printf("%d [", queue.first->noccs);
  struct cell* currentCell = queue.first;
  // to go at the end of the queue
  while (currentCell->next != NULL)
  {
    printf("%s,",currentCell->word );
    currentCell = currentCell->next;
  }
  printf("%s",currentCell->word );
  printf("]\n");
}

/**********************************
* Function who displays results
************************************/
void display_results(int nb_words_read,int nb_words_printed,struct queue occ_queue,struct queue min_queue,struct queue max_queue){
  printf("\n");
  printf("-Nombre de mots lus: %d\n", nb_words_read);
  printf("-Nombre de mots imprimés: %d\n", nb_words_printed);
  printf("-Nombre de mots différents: %d\n", length(occ_queue.first));
  printf("\n");
  printf("-La multiplicité la plus petite: ");
  print_min_max_format(min_queue);
  printf("\n");
  printf("-La multiplicité la plus grande: ");
  print_min_max_format(max_queue);
  printf("\n");
}

/**********************************
* Initialize and lauch the game
************************************/
int main(int argc,  char* argv[]) {
  // If we forgot the filenames in arguments
  if ( argc < 2 || argc > 5)
  {
    printf("Usage %s filename_1 [filename_2] [-s] [seed]\n",argv[0]);
  }
  else
  {
    // parse the differents arguments
    parse_opts(argc, argv);
    // Initialize all the datas
    struct pool pool = { {}, pool.m }; // Trick to initialize the pool statically
    struct pool_queue pool_queue = { {}, pool_queue.m };
    struct queue queue_1 = { NULL, NULL }; // Reader_1 queue
    struct queue queue_2 = { NULL, NULL }; // Reader_2 queue
    struct queue occ_queue = { NULL, NULL }; // Stat queue
    struct queue queue_1_writer = { NULL, NULL }; // Writer_1 queue
    struct queue queue_2_writer = { NULL, NULL }; // Writer_2 queue
    struct queue min_queue = { NULL, NULL };
    struct queue max_queue = { NULL, NULL };
    int nb_words_read = 0;
    int nb_words_printed = 0;
    int nb_rounds = 0;

    struct files files;

    if (argc == 3 || argc == 5){
      char * filename_1 = argv[optind];
      char * filename_2 = argv[optind + 1];

      FILE * file_1 = NULL;
      FILE * file_2 = NULL;

      file_1 = fopen(filename_1,"r");
      file_2 = fopen(filename_2,"r");

      files.file_1 = file_1;
      files.file_2 = file_2;
    }else{
      char * filename = argv[optind];

      FILE * file = NULL;

      file = fopen(filename,"r");

      files.file_1 = file;
      files.file_2 = file;
    }

    // if the file opens correctly
    if ( files.file_1 != NULL && files.file_2 != NULL )
    {
      // if not seed givin in parameter we use random
      if( seed == 0 )
      srand(time(NULL));
      // we use the seed in parameter
      else
      srand(seed);

      struct queues queues;
      queues.queue_1 = &queue_1;
      queues.queue_2 = &queue_2;

      struct queues writer_queues;
      writer_queues.queue_1 = &queue_1_writer;
      writer_queues.queue_2 = &queue_2_writer;

      struct monkeys monkeys;
      monkeys.monkey_reader_1 = monkey_reader;
      monkeys.monkey_reader_2 = monkey_reader;
      monkeys.monkey_statistician = monkey_statistician;
      monkeys.monkey_writer_1 = monkey_writer;
      monkeys.monkey_writer_2 = monkey_writer;
      monkeys.monkey_printer = monkey_printer;

      // main loop to run the game while a monkey is active and the number of round is not reached
      while ( is_at_least_one_monkey_active(&files, &queues, &writer_queues, nb_rounds) && nb_rounds < MAX_NB_ROUNDS )
      {
        work(&files, &pool, &pool_queue, &queues, &occ_queue, &writer_queues, &nb_words_read, &nb_words_printed, nb_rounds, &monkeys);
        nb_rounds++;
      }
    }

    // throw error if the opening of the file crashed
    else
    {
      printf("Can't open files\n");
      exit(EXIT_FAILURE);
    }

    // close the files at the end of the game
    if (argc == 3){
      fclose(files.file_1);
      fclose(files.file_2);
    }else{
      fclose(files.file_1);
    }

    // if the statistician queue ( occ_queue ) isn't empty, it means that the game has been played
    // so we compute the results and display them
    printf("\n");
    if ( !empty_queue(&occ_queue) )
    {
      min(&min_queue,occ_queue,&pool,&pool_queue);
      max(&max_queue,occ_queue,&pool,&pool_queue);
      display_results(nb_words_read,nb_words_printed,occ_queue,min_queue,max_queue);
      //print_queue(occ_queue);

    }
    // if queue is empty, it means than the reader got no words to read than it's usless
    // to display results
    else
    {
      printf("Empty file! \n");
    }
  }

  // the program ended well
  return 0;
}
