#include "testMonkeys.h"

void test_monkey_reader()
{
  char * filename = "shakespeare.txt";
  FILE * file = NULL;
  file = fopen(filename,"r");
  if (file != NULL)
  {
    struct queue queue = { NULL, NULL };
    struct pool pool = { {}, pool.m };
    struct pool_queue pool_queue = { {}, pool_queue.m };
    int nb_words_read = 0;
    monkey_reader(file,&queue,&pool,&pool_queue,&nb_words_read);
    assert(empty_queue(&queue) == 0);
    assert(nb_words_read == 1);
    printf("Monkey reader: Ok\n");
    fclose(file);
  }
  else
  {
    printf("Monkey reader: Wrong filename\n");
  }
}

void test_monkey_statistican()
{
  char * filename = "shakespeare.txt";
  FILE * file = NULL;
  file = fopen(filename,"r");
  if (file != NULL)
  {
    struct queue successors_queue = {NULL, NULL};
    struct cell cell1 = {"toto",-1, "", NULL, &successors_queue};
    struct cell cell2 = {"titi",-1, "", &cell1, &successors_queue};
    struct cell cell3 = {"tutu",-1, "", &cell2, &successors_queue};
    struct queue queue = { NULL, NULL };
    struct queue occ_queue = { NULL, NULL };
    struct pool pool = { {}, pool.m };
    struct pool_queue pool_queue = { {}, pool_queue.m };
    push(&queue,&pool,&pool_queue,cell1);
    push(&queue,&pool,&pool_queue,cell2);
    assert(length(occ_queue.first) == 0);
    monkey_statistician(&queue,&occ_queue,&pool,&pool_queue,file);
    assert(length(occ_queue.first) == 1);
    assert(length(occ_queue.first->successors_queue->first) == 1);
    push(&queue,&pool,&pool_queue,cell3);
    monkey_statistician(&queue,&occ_queue,&pool,&pool_queue,file);
    assert(length(occ_queue.first) == 2);
    assert(length(occ_queue.first->successors_queue->first) == 1);
    printf("Monkey statistican: OK\n");
    fclose(file);
  }
  else{
    printf("Monkey statistican: Wrong filename\n");
  }
}

void test_monkey_writer()
{
  struct queue successors_queue1 = {NULL, NULL};
  struct queue successors_queue2 = {NULL, NULL};
  struct cell cell1 = {"toto",1, "NN VB", NULL, &successors_queue1};
  struct cell cell2 = {"tete",1, "NN VB", NULL, &successors_queue2};
  struct cell cell3 = {"tutu",2, "NN VB", NULL, &successors_queue2};
  struct cell cell4 = {"tyty",3, "NN VB", NULL, &successors_queue2};
  struct cell cell5 = {"WORD",1, "NN VB", NULL, &successors_queue2};
  struct cell cell6 = {"WORDBIS",1, "NN VB", NULL, &successors_queue2};
  struct queue occ_queue = { NULL, NULL };
  struct queue writer_queue = { NULL, NULL };
  struct pool pool = { {}, pool.m };
  struct pool_queue pool_queue = { {}, pool_queue.m };
  push(&occ_queue,&pool,&pool_queue,cell1);
  push(&occ_queue,&pool,&pool_queue,cell2);
  push(cell1.successors_queue,&pool,&pool_queue,cell3);
  push(cell1.successors_queue,&pool,&pool_queue,cell4);
  push(&writer_queue,&pool,&pool_queue,cell5);
  push(&writer_queue,&pool,&pool_queue,cell6);
  monkey_writer(&occ_queue,&writer_queue,&pool,&pool_queue);
  assert(length(writer_queue.first) == 3);
  printf("Monkey writer: OK\n");
}

void test_monkey_printer()
{
  struct queue successors_queue1 = {NULL, NULL};
  struct cell cell1 = {"toto",0, "", NULL, &successors_queue1};
  struct cell cell2 = {"titi",0, "", &cell1, &successors_queue1};
  struct cell cell3 = {"?",0, "", &cell2, &successors_queue1};
  struct queue writer_queue = { NULL, NULL };
  struct pool pool = { {}, pool.m };
  struct pool_queue pool_queue = { {}, pool_queue.m };
  int nb_words_printed = 0;
  push(&writer_queue,&pool,&pool_queue,cell1);
  push(&writer_queue,&pool,&pool_queue,cell2);
  push(&writer_queue,&pool,&pool_queue,cell3);
  assert(length(writer_queue.first) == 3);
  monkey_printer(&writer_queue,&nb_words_printed);
  assert(length(writer_queue.first) == 0);
  assert(nb_words_printed == 3 );
  printf("Monkey printer: OK\n");
}
