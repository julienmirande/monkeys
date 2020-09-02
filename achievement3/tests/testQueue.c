#include "testQueue.h"


void test_lenght_cell()
{
  struct queue successors_queue = {NULL, NULL};
  struct cell cell1 = {"toto",1, "", NULL, &successors_queue};
  struct cell cell2 = {"titi",1, "", &cell1, &successors_queue};
  assert( length(&cell1) == 1 );
  assert( length(&cell2) == 2 );
  printf("Length cell: OK\n");
}

void test_empty_queue()
{
  struct queue successors_queue = {NULL, NULL};
  struct cell cell1 = {"toto",1, "", NULL, &successors_queue};
  struct queue queue1 = { NULL, NULL };
  assert(empty_queue(&queue1) == 1);
  struct queue queue2 = { &cell1, &cell1 };
  assert(empty_queue(&queue2) == 0);
  printf("Empty queue: OK\n");
}

void test_push_cell()
{
  struct queue successors_queue = {NULL, NULL};
  struct cell cell1 = {"toto",1, "", NULL, &successors_queue};
  struct cell cell2 = {"titi",1, "", &cell1, &successors_queue};
  struct queue queue = { NULL, NULL };
  struct pool pool = { {}, pool.m };
  struct pool_queue pool_queue = { {}, pool_queue.m };
  assert(empty_queue(&queue) == 1);
  push(&queue,&pool,&pool_queue,cell1);
  assert(empty_queue(&queue) == 0);
  push(&queue,&pool,&pool_queue,cell2);
  assert(empty_queue(&queue) == 0);
  assert( length(&cell2) == 2 );
  printf("Push cell: OK\n");
}

void test_find_min()
{
  struct queue successors_queue = {NULL, NULL};
  struct cell cell1 = {"toto",1, "", NULL, &successors_queue};
  struct cell cell2 = {"titi",3, "", &cell1,&successors_queue};
  struct cell cell3 = {"tutu",12, "", &cell2, &successors_queue};
  struct cell cell4 = {"tete",6, "", &cell3, &successors_queue};
  struct queue queue = { NULL, NULL };
  struct pool pool = { {}, pool.m };
  struct pool_queue pool_queue = { {}, pool_queue.m };
  push(&queue,&pool,&pool_queue,cell1);
  push(&queue,&pool,&pool_queue,cell2);
  push(&queue,&pool,&pool_queue,cell3);
  push(&queue,&pool,&pool_queue,cell4);
  assert(find_min(queue) == 1);
  pull(&queue);
  assert(find_min(queue) == 3);
  printf("Find min: OK\n");
}

void test_find_max()
{
  struct queue successors_queue = {NULL, NULL};
  struct cell cell1 = {"toto",1, "", NULL, &successors_queue};
  struct cell cell2 = {"titi",12, "", &cell1, &successors_queue};
  struct cell cell3 = {"tutu",3, "", &cell2, &successors_queue};
  struct cell cell4 = {"tete",6, "", &cell3, &successors_queue};
  struct queue queue = { NULL, NULL };
  struct pool pool = { {}, pool.m };
  struct pool_queue pool_queue = { {}, pool_queue.m };
  push(&queue,&pool,&pool_queue,cell1);
  push(&queue,&pool,&pool_queue,cell2);
  push(&queue,&pool,&pool_queue,cell3);
  push(&queue,&pool,&pool_queue,cell4);
  assert(find_max(queue) == 12);
  pull(&queue);
  pull(&queue);
  assert(find_max(queue) == 6);
  printf("Find max: OK\n");
}

void test_fill_queue()
{
  struct queue successors_queue = {NULL, NULL};
  struct cell cell1 = {"toto",1, "", NULL, &successors_queue};
  struct cell cell2 = {"titi",3, "", &cell1, &successors_queue};
  struct cell cell3 = {"tutu",3, "", &cell2, &successors_queue};
  struct cell cell4 = {"tete",6, "", &cell3, &successors_queue};
  struct queue queue = { NULL, NULL };
  struct queue newqueue = { NULL, NULL };
  struct pool pool = { {}, pool.m };
  struct pool_queue pool_queue = { {}, pool_queue.m };
  push(&queue,&pool,&pool_queue,cell1);
  push(&queue,&pool,&pool_queue,cell2);
  push(&queue,&pool,&pool_queue,cell3);
  push(&queue,&pool,&pool_queue,cell4);
  fill_queue(3,&newqueue,queue,&pool,&pool_queue);
  assert(empty_queue(&newqueue) == 0);
  assert(length(newqueue.first) == 2);
  printf("Fill queue Min/Max: OK\n");
}

void test_cell_exist()
{
  struct queue successors_queue = {NULL, NULL};
  struct cell cell1 = {"toto",1, "", NULL, &successors_queue};
  struct cell cell2 = {"titi",12, "", &cell1, &successors_queue};
  struct cell cell3 = {"tutu",3, "", &cell2, &successors_queue};
  struct cell cell4 = {"tete",6, "", &cell3, &successors_queue};
  struct queue queue = { NULL, NULL };
  struct pool pool = { {}, pool.m };
  struct pool_queue pool_queue = { {}, pool_queue.m };
  push(&queue,&pool,&pool_queue,cell1);
  push(&queue,&pool,&pool_queue,cell2);
  push(&queue,&pool,&pool_queue,cell3);
  push(&queue,&pool,&pool_queue,cell4);
  assert(cell_exist(&queue,"titi") == 1);
  pull(&queue);
  pull(&queue);
  assert(cell_exist(&queue,"titi") == 0);
  printf("Cell exists: OK\n");
}

void test_pull_cell()
{
  struct queue successors_queue = {NULL, NULL};
  struct cell cell1 = {"toto",1, "", NULL, &successors_queue};
  struct cell cell2 = {"titi",1, "", &cell1, &successors_queue};
  struct queue queue = { NULL, NULL };
  struct pool pool = { {}, pool.m };
  struct pool_queue pool_queue = { {}, pool_queue.m };
  push(&queue,&pool,&pool_queue,cell1);
  push(&queue,&pool,&pool_queue,cell2);
  struct cell* cellBack = pull(&queue);
  cellBack = pull(&queue);
  assert(empty_queue(&queue) == 1);
  (void) cellBack;
  printf("Pull cell: OK\n");
}

void test_increment_queue()
{
  struct queue successors_queue = {NULL, NULL};
  struct cell cell1 = {"toto",1, "", NULL, &successors_queue};
  struct cell cell2 = {"titi",12, "", &cell1, &successors_queue};
  struct queue queue = { NULL, NULL };
  struct pool pool = { {}, pool.m };
  struct pool_queue pool_queue = { {}, pool_queue.m };
  push(&queue,&pool,&pool_queue,cell1);
  push(&queue,&pool,&pool_queue,cell2);
  increment_occ_queue(&queue,"toto");
  assert((queue.first)->noccs == 2);
  increment_occ_queue(&queue,"titi");
  pull(&queue);
  assert((queue.first)->noccs == 13);
  printf("Cell increment queue: OK\n");
}

void test_queue_contains_ponctuation()
{
  struct queue successors_queue1 = {NULL, NULL};
  struct cell cell1 = {"toto",1, "", NULL, &successors_queue1};
  struct cell cell2 = {"?",1, "", &cell1, &successors_queue1};
  struct cell cell3 = {"titi",1, "", &cell2, &successors_queue1};
  struct queue writer_queue = { NULL, NULL };
  struct pool pool = { {}, pool.m };
  struct pool_queue pool_queue = { {}, pool_queue.m };
  push(&writer_queue,&pool,&pool_queue,cell1);
  push(&writer_queue,&pool,&pool_queue,cell2);
  push(&writer_queue,&pool,&pool_queue,cell3);
  assert( queue_contains_punctuation(&writer_queue) == 1);
  pull(&writer_queue);
  pull(&writer_queue);
  assert( queue_contains_punctuation(&writer_queue) == 0);
  printf("Queue contains ponctuation: OK\n");
}

void test_sum_successors()
{
  struct queue successors_queue1 = {NULL, NULL};
  struct queue successors_queue2 = {NULL, NULL};
  struct cell cell1 = {"toto",1, "", NULL, &successors_queue1};
  struct cell cell2 = {"tete",1, "", NULL, &successors_queue2};
  struct cell cell3 = {"tutu",2, "", NULL, &successors_queue2};
  struct cell cell4 = {"tyty",3, "", NULL, &successors_queue2};
  struct queue queue = { NULL, NULL };
  struct pool pool = { {}, pool.m };
  struct pool_queue pool_queue = { {}, pool_queue.m };
  push(&queue,&pool,&pool_queue,cell1);
  push(cell1.successors_queue,&pool,&pool_queue,cell2);
  push(cell1.successors_queue,&pool,&pool_queue,cell3);
  push(cell1.successors_queue,&pool,&pool_queue,cell4);
  assert(sum_successors(&cell1) == 6);
  pull(cell1.successors_queue);
  assert(sum_successors(&cell1) == 5);
  pull(cell1.successors_queue);
  assert(sum_successors(&cell1) == 3);
  printf("Sum successors: OK\n");
}

void test_replace_by_exclamation()
{
  struct queue successors_queue1 = {NULL, NULL};
  struct cell cell1 = {"toto",1, "", NULL, &successors_queue1};
  struct cell cell2 = {"?",1, "", NULL, &successors_queue1};
  struct queue writer_queue = { NULL, NULL };
  struct pool pool = { {}, pool.m };
  struct pool_queue pool_queue = { {}, pool_queue.m };
  push(&writer_queue,&pool,&pool_queue,cell1);
  push(&writer_queue,&pool,&pool_queue,cell2);
  assert(strcmp((writer_queue.first)->next->word,"?") == 0);
  replace_by_exclamation(&writer_queue);
  assert(strcmp((writer_queue.first)->next->word,"!") == 0);
  printf("Replace by exclamation: OK\n");
}

void test_is_a_name()
{
  struct queue successors_queue = {NULL, NULL};
  struct cell cell_1 = {"toto",1, "ABE bibou nn marsupilami",NULL, &successors_queue};
  struct cell cell_2 = {"toto",1, "BOUBI NN BOBO",NULL, &successors_queue};
  struct cell cell_3 = {"toto",1, "DT IN WDT",NULL, &successors_queue};
  struct cell cell_4 = {"toto",1, "VB JJ NN IN",NULL, &successors_queue};
  assert(is_a_name(&cell_1) == 0);
  assert(is_a_name(&cell_2) != 0);
  assert(is_a_name(&cell_3) == 0);
  assert(is_a_name(&cell_4) != 0);
  printf("Is a name: OK\n");
}

void test_is_a_conjugated_verb()
{
  struct queue successors_queue = {NULL, NULL};
  struct cell cell_1 = {"toto",1, "ABE bibou vb marsupilami",NULL, &successors_queue};
  struct cell cell_2 = {"toto",1, "BOUBI VB BOBO",NULL, &successors_queue};
  struct cell cell_3 = {"toto",1, "DT IN WDT",NULL, &successors_queue};
  struct cell cell_4 = {"toto",1, "VBD JJ NN IN",NULL, &successors_queue};
  assert(is_a_conjugated_verb(&cell_1) == 0);
  assert(is_a_conjugated_verb(&cell_2) != 0);
  assert(is_a_conjugated_verb(&cell_3) == 0);
  assert(is_a_conjugated_verb(&cell_4) != 0);
  printf("Is a conjugated verb: OK\n");
}

void test_is_there_a_name()
{
  struct queue successors_queue = {NULL, NULL};

  struct cell cell_1 = {"toto",1, "ABE bibou nn marsupilami",NULL, &successors_queue};
  struct cell cell_2 = {"toto",1, "BOUBI VB BOBO", &cell_1, &successors_queue};
  struct cell cell_3 = {"toto",1, "DT IN WDT", &cell_2, &successors_queue};
  struct cell cell_4 = {"toto",1, "VBD JJ NO IN", &cell_3, &successors_queue};
  struct queue queue_ = {&cell_4, &cell_1};

  struct cell cell1 = {"toto",1, "ABE bibou vb marsupilami",NULL, &successors_queue};
  struct cell cell2 = {"toto",1, "BOUBI VB BOBO", &cell1, &successors_queue};
  struct cell cell3 = {"toto",1, "DT IN WDT", &cell2, &successors_queue};
  struct cell cell4 = {"toto",1, "VBD JJ NN IN", &cell3, &successors_queue};
  struct queue queue = {&cell4, &cell1};

  assert(is_there_a_name(&queue_) == 0);
  assert(is_there_a_name(&queue) != 0);

  printf("Is there a name: OK\n");
}

void test_is_there_a_conjugated_verb()
{
  struct queue successors_queue = {NULL, NULL};

  struct cell cell_1 = {"toto",1, "ABE bibou vb marsupilami",NULL, &successors_queue};
  struct cell cell_2 = {"toto",1, "BOUBI NN BOBO", &cell_1, &successors_queue};
  struct cell cell_3 = {"toto",1, "DT IN WDT", &cell_2, &successors_queue};
  struct cell cell_4 = {"toto",1, "NNP JJ NN IN", &cell_3, &successors_queue};
  struct queue queue_ = {&cell_4, &cell_1};

  struct cell cell1 = {"toto",1, "ABE bibou vb marsupilami",NULL, &successors_queue};
  struct cell cell2 = {"toto",1, "BOUBI VB BOBO", &cell1, &successors_queue};
  struct cell cell3 = {"toto",1, "DT IN WDT", &cell2, &successors_queue};
  struct cell cell4 = {"toto",1, "VBD JJ NN IN", &cell3, &successors_queue};
  struct queue queue = {&cell4, &cell1};

  assert(is_there_a_conjugated_verb(&queue_) == 0);
  assert(is_there_a_conjugated_verb(&queue) != 0);

  printf("Is there a conjugated verb: OK\n");
}
