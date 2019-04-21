#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "eurovision.h"
#include <string.h>
#include "map.h"



/*********************************Defines************************************/
#define TOP_TEN_LEN 10

/*****************************DataSturctures*********************************/
struct eurovision_t{
  /***********
  TODO: Check
  ***********/
  Map states;
  Map judges;
  List votes;
  int audiencePercent; //set to one by default
};

typedef struct state_t{
  /***********
  TODO: Check
  ***********/
  int id;
  char* name;
  char* song;
  int top_ten[TOP_TEN_LEN];
  Map votes;
  int score_by_judges;
  int score_by_audience;
}*State;


/*****************************StaticFunctions********************************/

void copyIntArray(int *destination,int *source, int len){
  for(int i=0; i<len; i++){
    destination[i] = source[i];
  }
}

int* copyInt (int* key){
  /***********
  TODO: Check
  ***********/
  assert(key);
  if(!key) return NULL;
  int* copy = malloc(sizeof(*copy));
  assert(copy);
  if(!copy) return NULL;
  *copy = *key;
  return copy;
}

State copyState(State state){
  /***********
  TODO: Check
  ***********/
  if(!state) return NULL;
  State copy = malloc(sizeof(*copy));
  assert(copy);
  if(!copy) return NULL;

  copy->id = state->id;
  strcpy(copy->name,state->name);
  strcpy(copy->song,state->song);
  assert(copy->top_ten);
  copyIntArray(copy->top_ten,state->top_ten,TOP_TEN_LEN);
  assert(!copy->votes);
  copy->votes = mapCopy(state->votes);
  assert(copy->votes);
  if(!copy->votes)return NULL;
  copy->score_by_judges = state->score_by_judges;
  copy->score_by_audience =state->score_by_audience;

}



/*****************************Functions**************************************/

Eurovision eurovisionCreate(){
  /***********
  TODO: Check
  ***********/
  Eurovision eurovision = malloc(sizeof(*eurovision));
  assert(eurovision);
  if(!eurovision) return NULL;

  eurovision->states = NULL;
  eurovision->judges = NULL;
  eurovision->votes = NULL;
  eurovision->audiencePercent = 1;
}
