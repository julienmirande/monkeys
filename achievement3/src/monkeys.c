//
// file: monkeys.c
// date: mar. oct. 23 14:51:10 CEST 2018
// authors: jmirande / adanquigny
//
//

#include "monkeys.h"


int is_monkey_reader_active(FILE * file){
  // if the file is not end then the monkey stays active
  if (feof(file)){
    return 0;
  }
  return 1;
}

void monkey_reader(FILE * file, struct queue* queue, struct pool* pool,struct pool_queue* pool_queue, int* nb_words_read){
  int read_character = 0;
  //to know where to implement the "read_character" in the "new_word" and "labels"

  //////////////////////////////////////////////////

  //buffer for the word, char by char
  char new_word[MAX_WORD_LENGTH+1] = "";
  //contain the current char
  int i = 0;

  read_character = getc(file);
  // while char is different from space,
  while (read_character != ' ' && read_character != EOF){
    // fill the buffer
    new_word[i] = read_character;
    i++;
    read_character = getc(file);
  }

  // if the buffer is full we throw an error
  if ( i > MAX_WORD_LENGTH )
  {
    printf("Word too big ! Stop \n");
    exit(EXIT_FAILURE);
  }
  ///////////////////////////////////////////////////

  char new_label[MAX_LABEL_LENGTH+1] = "";
  //contain labels
  int j = 0;    // blablz NN : i index of z, j index of first letter first label, two index next to i

  read_character = getc(file);
  // while char is different from "newline",
  while (read_character != 10 && read_character != EOF){
    // fill the buffer
    new_label[j] = read_character;
    j++;
    read_character = getc(file);
  }

  // if the buffer is full we throw an error
  if ( j > MAX_LABEL_LENGTH )
  {
    printf("Label too big ! Stop \n");
    exit(EXIT_FAILURE);
  }

  ///////////////////////////////////////////////////
  new_label[MAX_LABEL_LENGTH] = 0;

  if (i>0 && j>0){
    push_custom_cell(new_word,queue,pool,pool_queue,-1, new_label);
    (void) (*nb_words_read)++;
  }
}

int is_monkey_statistician_active(struct queue* queue){
  // Empty queue or the first one of the queue is already read
  // if already read by the statistician, noccs is 0
  if (empty_queue(queue) || (queue->first)->noccs == 0 || ( queue->first->noccs == -2 && queue->first->next == NULL))
  {
    return 0;
  }
  // Else the monkey stays active
  return 1;
}

void monkey_statistician(struct queue* queue, struct queue* occ_queue,struct pool* pool, struct pool_queue* pool_queue, FILE * file){
  if ( !empty_queue(queue) ){
    struct cell* cell = queue->first;
    // cell->noccs == -1 means that the word haven't been read yet
    if ( cell->noccs == -1){
      // if the cell already exist in the occurence queue then we
      // just increment the noccs of the cell with the same word
      if (cell_exist(occ_queue,cell->word)){
        increment_occ_queue(occ_queue,cell->word);
      }
      // else we create a new cell with 1 as noccs because it's
      // a new word
      else{
        push_custom_cell(cell->word,occ_queue,pool,pool_queue,1,cell->label);
      }
      // we put 0 to know if the printer can print it or not and avoid statistician
      // to compute two times the same word
      cell->noccs = 0;

      struct cell* cell_occ = occ_queue->first;
      while (cell_occ != NULL && strcmp(cell_occ->word, cell->word) != 0 ) {
        cell_occ = cell_occ->next;
      }

      if ( cell->next != NULL && cell_occ != NULL){
        // On traite le cas du mot suivant
        // Si le mot suivant de la file du lecteur est inclut dans la file succeseur
        // du mot actuel de la file du statisticien alors on l'incrémente
        if (cell_exist(cell_occ->successors_queue,cell->next->word)){
          increment_occ_queue(cell_occ->successors_queue,cell->next->word);
        }
        // sinon on ajoute un nouveau succeseur dans la file successeur du mot actuel
        else{
          push_custom_cell(cell->next->word,cell_occ->successors_queue,pool,pool_queue,1,cell->next->label);
        }
        pull(queue);
      }
      else{
        if ((queue->first != NULL && (queue->first)->noccs == 0) || (queue->first != NULL && queue->first->noccs == -2 && feof(file)))
        {
          pull(queue);
        }
        cell->noccs = -2;
      }
    }

    // si on trouve -2 dans la file du lecteur alors il est deja dans la file du
    // statisticien mais on doit toujours affecter son suivant a la file des successeurs
    else if ( cell->noccs == -2 ){
      struct cell* cell_occ = occ_queue->first;
      //////////////////////////////////////////////////////////////////////////////////////////////////////////
      while (cell_occ != NULL && strcmp(cell_occ->word, cell->word) != 0 ) {
        cell_occ = cell_occ->next;
      }
      //////////////////////////////////////////////////////////////////////////////////////////////////////////

      if (cell_exist(cell_occ->successors_queue,(cell->next)->word)){
        increment_occ_queue(cell_occ->successors_queue,(cell->next)->word);
      }
      else{
        push_custom_cell((cell->next)->word,cell_occ->successors_queue,pool,pool_queue,1,(cell->next)->label);
      }
      cell->noccs = 0;
      pull(queue);
    }

  }
}

int is_monkey_writer_active(int nb_rounds){
  // Can't be active before one hundred rounds
  if ( nb_rounds < NB_ROUNDS_INACTIVE )
  {
    return 0;
  }
  // Else the monkey stays active
  return 1;
}

void monkey_writer(struct queue* occ_queue, struct queue* writer_queue, struct pool* pool, struct pool_queue* pool_queue){
  // if the queue is not empty
  if (!empty_queue(occ_queue))
  {
    struct cell* cell_occ = occ_queue->first;
    //we randomly get a value in occ_queue
    int random = rand()%length(cell_occ);
    //we go forward until we are at the good position
    for (int i=0 ; i < random; i++)
    {
      cell_occ = cell_occ->next;
    }

    // we compute the number of successors
    int nb_successors = sum_successors(cell_occ);

    // if he dosn't have successors or 1/10 we have to push a punctuation
    if ( nb_successors == 0 || rand()%10 == 0)
    {
      // we test if the previous word is not a puncation to avoid multiple punctation in a row
      if (writer_queue->first != NULL && !word_is_punctuation(writer_queue->last->word) )
      {
        char punctuation[] = {',', ';', ':', '.', '?', '-', '!'}; //7 signes de punctuation
        // random value for punctuation
        int nb_punctuation = rand()%NB_PUNCTUATION;

        // create a word with the punctuation
        /*
        char str[2] = "\0";
        str[0] = punctuation[nb_punctuation];
        */
        char str[] = {punctuation[nb_punctuation],0};
        // push the new cell into the writer queue
        push_custom_cell(str, writer_queue, pool, pool_queue, 0, "");

        // if there are not at least 2 distinct word with 1 name and 1 conjugated verb when he end his sentence, he pull it all and start again later
        if ( ! ( (is_there_a_name(writer_queue)) && (is_there_a_conjugated_verb(writer_queue)) && length(writer_queue->first) > 2) ){
          while ( ! empty_queue(writer_queue) ){
            pull(writer_queue);
          }
        }

      }
    }
    // else we have to push a word
    else
    {
      // random inside the succesors
      int word_suc = rand()%nb_successors;
      struct cell* cell_suc = (cell_occ->successors_queue)->first;
      // tricks to go until the good noccs position
      while (word_suc >= 0)
      {
        word_suc -= cell_suc->noccs;
        if ( word_suc <= 0)
        {
          // when we found the good position, then we push the new word inside the writer queue
          push_custom_cell(cell_suc->word, writer_queue, pool, pool_queue, 0, cell_suc->label);
        }
        cell_suc = cell_suc->next;
      }
    }
  }
  // if we have only one word in the writer queue then put a '!'
  replace_by_exclamation(writer_queue);
}

int is_monkey_printer_active(struct queue* writer_queue){
  // Not active while the writer queue is empty or while there is no punctuation in the sentance
  if (empty_queue(writer_queue) || !queue_contains_punctuation(writer_queue))
  {
    return 0;
  }
  // Else the monkey stays active
  return 1;
}

void monkey_printer(struct queue* writer_queue, int* nb_words_printed){
  struct cell* cell = writer_queue->first;

  // print and pull all the words before the first punctuation of the writer queue
  while (cell != NULL && !word_is_punctuation(cell->word))
  {
    printf("%s ", cell->word);
    pull(writer_queue);
    (void) (*nb_words_printed)++;
    cell = cell->next;
  }
  // print and pull the first punctuation of the writer queue
  printf("%s ", cell->word);
  pull(writer_queue);
  (void) (*nb_words_printed)++;

  // Just go to the new line if we find '.!?'
  // if (cell->word[0] == '.' || cell->word[0] == '?' || cell->word[0] == '!' )
  // {
  //   printf("\n" );
  // }
}

int is_at_least_one_monkey_active(struct files* files, struct queues* queues, struct queues * writer_queues, int nb_rounds){
  return is_monkey_reader_active(files->file_1)
  || is_monkey_reader_active(files->file_2)
  || is_monkey_statistician_active(queues->queue_1)
  || is_monkey_statistician_active(queues->queue_2)
  || is_monkey_printer_active(writer_queues->queue_1)
  || is_monkey_printer_active(writer_queues->queue_2)
  || is_monkey_writer_active(nb_rounds);
}
