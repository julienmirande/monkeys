// file: queue.h
// date: mar. oct. 23 14:51:10 CEST 2018
// authors: jmirande / adanquigny

#include "queue.h"

/**********************************
* Given a pool, and the constituents of a cell,
* return a pointer to a valid cell with the same constituents,
* or NULL if all the memory has been allocated.
************************************/
struct cell* create_new_cell(struct pool *pool, struct pool_queue* pool_queue, char word[], int noccs, char label[], struct cell* next){
  if (pool->next_free - pool->m >= MAX_CELLS){
    return NULL;
  }
  struct cell* res = pool->next_free;
  strncpy(res->word, word, MAX_WORD_LENGTH+1); // Always take the null-terminator into account
  res->noccs = noccs;
  strncpy(res->label, label, MAX_LABEL_LENGTH+1);
  res->next = next;
  pool->next_free++;
  res->successors_queue = create_new_queue(pool_queue,NULL,NULL);
  return res;
}

/**********************************
* To print a queue
************************************/
void print_queue(struct queue queue){
  if ( empty_queue(&queue) )
  {
    printf("Empty\n");
  }
  else
  {
    struct cell* currentCell = queue.first;
    while (currentCell->next != NULL) //to go at the end of the queue
    {
      print_cell(currentCell);
      printf(" -> ");
      currentCell = currentCell->next;
    }
    print_cell(currentCell);
    printf("\n");
  }
}

/**********************************
*  Given a pool, and the constituents of a queue,
* return a pointer to a valid queue with the same constituents,
* or NULL if all the memory has been allocated.
************************************/
struct queue* create_new_queue(struct pool_queue *pool_queue, struct cell* first, struct cell* last){
  if(pool_queue->next_free - pool_queue->m >= MAX_QUEUE)
    return NULL;
  struct queue* res = pool_queue->next_free;
  res->first = first;
  res->last = last;
  pool_queue->next_free++;
  return res;
}

/**********************************
* Return if a queue is empty
************************************/
int empty_queue(struct queue* queue){
  return (queue->first == NULL );
}

/**********************************
*  Return if a cell with a special word
*  exists or not in the queue
************************************/
int cell_exist(struct queue* queue,char * word){
  struct cell* cell = queue->first;
  while (cell != NULL) {
    if (strcmp(cell->word, word) == 0 )
    {
      return 1;
    }
    cell = cell->next;
  }

  return 0;
}

/**********************************
* Push a cell into a queue
************************************/
void push(struct queue* queue,struct pool* pool, struct pool_queue* pool_queue, struct cell cell){
  struct cell* new_cell = create_new_cell(pool,pool_queue,cell.word,cell.noccs,cell.label,NULL);
  if ( new_cell == NULL ){
    exit(EXIT_FAILURE);
  }
  // if the queue is empty the the first cell is also the last cell
  if ( empty_queue(queue) ){
    queue->first = new_cell;
    queue->last = new_cell;
  }
  else{
    //switch the last cell pointer from NULL to new_cell
    (queue->last)->next = new_cell;
    //queue "to the last" pointer point to new_cell
    queue->last = new_cell;
  }
}

/**********************************
* Push a custom cell (with a fixed
* word and noccs) into a queue
************************************/
void push_custom_cell(char new_word[], struct queue* queue, struct pool* pool,struct pool_queue* pool_queue, int noccs, char new_label[]){
  struct cell cell;
  strcpy(cell.word,new_word);
  cell.noccs = noccs;
  strcpy(cell.label,new_label);
  cell.next = NULL;
  push(queue, pool, pool_queue, cell);
}

/**********************************
* Return the cell poped from the queue
************************************/
struct cell* pull(struct queue* queue){
  // pop a cell only the queue is not empty
  if (!empty_queue(queue)){
    struct cell* cell_copy = queue->first;
    //first cell becomes the follwing one
    queue->first = (queue->first)->next;
    return cell_copy;
  }

  return NULL;
}

/**********************************
* Increments the number of occurence
* of a cell in the queue
************************************/
void increment_occ_queue(struct queue* occ_queue, char * word){
  struct cell* cell = occ_queue->first;
  while ( strcmp(cell->word, word) != 0 )
  {
    cell = cell->next;
  }
  // if we have found the same word in the
  // queue then we increment his noccs
  (cell->noccs)++;
}

/**********************************
* Return if a the queue contains a punctuation
************************************/
int queue_contains_punctuation(struct queue* successors_queue){
  char punctuation[] = {',', ';', ':', '.', '?', '-', '!'}; //7 signes de ponctuation
  struct cell* cell = successors_queue->first;
  // for each cell and each punctation
  while ( cell != NULL )
  {
    for( int i=0; i<NB_PUNCTUATION; i++)
    {
      // if word is punctation then return true
      if (cell->word[0] == punctuation[i])
      {
        return 1;
      }
    }
    cell = cell->next;
  }
  // if not found return false
  return 0;
}

/**********************************
* Return if a word is a punctation
************************************/
int word_is_punctuation(char word[]){
  char ponctuation[] = {',', ';', ':', '.', '?', '-', '!'}; //7 signes de ponctuation
  // for each punctuation
  for( int i=0; i<NB_PUNCTUATION; i++)
  {
    if (word[0] == ponctuation[i])
    {
      return 1;
    }
  }
  return 0;
}

/**********************************
* Sum the noccs of the different cells noccs of the queue
************************************/
int sum_successors(struct cell* cell_occ){
  int sum = 0;
  struct cell* cell_suc = (cell_occ->successors_queue)->first;
  while (cell_suc != NULL){
    sum += cell_suc->noccs;
    cell_suc = cell_suc->next;
  }
  return sum;
}

/**********************************
* replace punctuation by exclamation when there is only one word to print
************************************/
void replace_by_exclamation(struct queue* writer_queue){
  // if the queue contains only 2 words we look if the second word is punctuation
  if (length(writer_queue->first)>= 2 && !word_is_punctuation(writer_queue->first->word)&& word_is_punctuation(writer_queue->first->next->word))
  {
    /*
    char str[2] = "\0";
    str[0] = '!';
    */
    char str[]={'!',0};

    // if we found, then we swap punctuation by '!'
    strcpy(writer_queue->first->next->word,str);
  }
  /*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // if the queue contains more then 3 words
  if (length(writer_queue->first) >=3){
    struct cell* cell = writer_queue->first;
    while (cell->next->next != NULL)
    {
      //we look the 3 words in a row
      if ( word_is_punctuation(cell->word) && !word_is_punctuation(cell->next->word)&& word_is_punctuation(cell->next->next->word))
      {
        char str[2] = "\0";
        str[0] = '!';
        // if we found, then we swap punctuation by '!'
        strcpy(cell->next->next->word,str);
      }
      cell = cell->next;
    }
  }
  *////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/***************************
return if there is a name in the queue
***************************/
int is_there_a_name(struct queue *writer_queue){
  struct cell* cell = writer_queue->first;
  while (cell != NULL){
    if (is_a_name(cell)){
      return 1;
    }
    cell = cell->next;
  }
  return 0;
}

/***************************
return if there is a conjugated verb in the queue
***************************/
int is_there_a_conjugated_verb(struct queue *writer_queue){
  struct cell* cell = writer_queue->first;
  while (cell != NULL){
    if (is_a_conjugated_verb(cell)){
      return 1;
    }
    cell = cell->next;
  }
  return 0;
}

/**********************************
* Find the min into a queue
************************************/
int find_min(struct queue queue){
  struct cell* cell = queue.first;
  int min = cell->noccs;
  while (cell != NULL) {
    if (cell->noccs < min)
    {
      min = cell->noccs;
    }
    cell = cell->next;
  }
  return min;
}

/**********************************
* Find the max into a queue
************************************/
int find_max(struct queue queue){
  struct cell* cell = queue.first;
  int max = cell->noccs;
  while (cell != NULL) {
    if (cell->noccs > max)
    {
      max = cell->noccs;
    }
    cell = cell->next;
  }
  return max;
}

/**********************************
* Push all the cells containing a special
* value from the source queue into a destination queue
************************************/
void fill_queue(int value, struct queue* queueDst, struct queue queueSrc, struct pool* pool, struct pool_queue* pool_queue){
  struct cell* cell = queueSrc.first;
  while (cell != NULL) {
    // if the cell noccs is the same than the value then
    // we copy it and push it in the new queue
    if (cell->noccs == value)
    {
      push_custom_cell(cell->word,queueDst,pool, pool_queue,cell->noccs,cell->label);
    }
    cell = cell->next;
  }
}

/**********************************
* Find the min and then create a queue
* with all the cells containing the value
************************************/
void min(struct queue* queueDst, struct queue queueSrc, struct pool* pool, struct pool_queue* pool_queue){
  int min = find_min(queueSrc);
  fill_queue(min, queueDst, queueSrc, pool, pool_queue);
}

/**********************************
* Find the max and then create a queue
* with all the cells containing the value
************************************/
void max(struct queue* queueDst, struct queue queueSrc, struct pool* pool, struct pool_queue* pool_queue){
  int max = find_max(queueSrc);
  fill_queue(max, queueDst, queueSrc, pool, pool_queue);
}
