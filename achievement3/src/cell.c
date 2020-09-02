// file: cell.c
// date: mar. oct. 23 14:51:10 CEST 2018
// authors: jmirande / adanquigny

#include "cell.h"

/**********************************
* Print a cell
************************************/
void print_cell(struct cell* cell)
{
  printf("%s(%d)(%s)",cell->word, cell->noccs, cell->label);
}

/**********************************
* Given a cell, compute the length of the chained list starting from this cell
************************************/
int length(const struct cell* c) {
  int res = 0;
  while (c!= NULL) {
    c = c->next;
    res++;
  }
  return res;
}

/****************************
return if a cell word is a name <=> label contain NN or NNS or NNP or NNPS
****************************/
int is_a_name(struct cell *cell){
  //we run a small string of characters along the label to see if it is there
  int len = strlen(cell->label);

  for (int i = 0; i < len -1; i++){ // i < len - 2 + 1
    if ( (cell->label[i] == 'N') && (cell->label[i+1] == 'N') ){ // NN ?
      return 1;
    }
  }
  for (int i = 0; i < len -2; i++){ // i < len - 3 + 1
    if ( (cell->label[i] == 'N') && (cell->label[i+1] == 'N') && (cell->label[i+2] == 'S') ){ // NNS ?
      return 1;
    }
  }
  for (int i = 0; i < len -2; i++){ // i < len - 3 + 1
    if ( (cell->label[i] == 'N') && (cell->label[i+1] == 'N') && (cell->label[i+2] == 'P') ){ // NNP ?
      return 1;
    }
  }
  for (int i = 0; i < len -3; i++){ // i < len - 4 + 1
    if ( (cell->label[i] == 'N') && (cell->label[i+1] == 'N') && (cell->label[i+2] == 'P') && (cell->label[i+3] == 'S') ){ // NNPS ?
      return 1;
    }
  }

  return 0;
}

/****************************
return if a cell word is a conjugated verb <=> label contain VB or VBD or VBP or VBZ
****************************/
int is_a_conjugated_verb(struct cell *cell){

  int len = strlen(cell->label);

  for (int i = 0; i < len -1; i++){ // i < len - 2 + 1
    if ( (cell->label[i] == 'V') && (cell->label[i+1] == 'B') ){ // VB ?
      return 1;
    }
  }
  for (int i = 0; i < len -2; i++){ // i < len - 3 + 1
    if ( (cell->label[i] == 'V') && (cell->label[i+1] == 'B') && (cell->label[i+2] == 'D') ){ // VBD ?
      return 1;
    }
  }
  for (int i = 0; i < len -2; i++){ // i < len - 3 + 1
    if ( (cell->label[i] == 'V') && (cell->label[i+1] == 'B') && (cell->label[i+2] == 'P') ){ // VBP ?
      return 1;
    }
  }
  for (int i = 0; i < len -2; i++){ // i < len - 4 + 1
    if ( (cell->label[i] == 'V') && (cell->label[i+1] == 'B') && (cell->label[i+2] == 'Z') ){ // VBZ ?
      return 1;
    }
  }

  return 0;
}
