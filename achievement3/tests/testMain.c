// File : test.c

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cell.h>
#include <queue.h>
#include <monkeys.h>

#include "testQueue.h"
#include "testMonkeys.h"


void test_word_is_ponctuation()
{
  char word1[] = "titi";
  assert(word_is_punctuation(word1) == 0);
  char word2[] = "!";
  assert(word_is_punctuation(word2) == 1);
  char word3[] = " ";
  assert(word_is_punctuation(word3) == 0);
  printf("Word is ponctuation: OK\n");
}

int main()
{
  printf("-------------TEST--------------\n");

  test_lenght_cell();
  test_empty_queue();
  test_push_cell();
  test_pull_cell();
  test_fill_queue();
  test_cell_exist();
  test_increment_queue();
  test_find_min();
  test_find_max();
  test_queue_contains_ponctuation();
  test_word_is_ponctuation();
  test_sum_successors();
  test_replace_by_exclamation();
  test_is_a_name();
  test_is_a_conjugated_verb();
  test_is_there_a_name();
  test_is_there_a_conjugated_verb();
  test_monkey_reader();
  test_monkey_statistican();
  test_monkey_writer();
  printf("-------Printer-------\n" );
  test_monkey_printer();


  return 0;
}
