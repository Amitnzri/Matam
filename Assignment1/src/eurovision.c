#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "eurovision.h"
#include "map.h"


/*****************************DataSturctures*********************************/
struct eurovision_t{
  /***********
  TODO: CHECK
  ***********/
  Map states;
  Map judges;
  List Votes;
  int audiencePercent; //set to one by default
};

typedef struct state_t{
  /***********
  TODO: CHECK
  ***********/
  int id;
  char* name;
  char* song;
  int top_ten[10];
  Map votes;
  int score_by_judges;
  int score_by_audience;
}*State;




/*****************************StaticFunctions********************************/

int* copyInt (MapKeyElement key){
  /***********
  TODO: CHECK
  ***********/
  assert(key);
  if(!key) return NULL;
  int* copy malloc(sizeof(*copy));
  assert(copy);
  if(!copy) return NULL;
  *copy = *(int*) key;
  return copy;
}




/*****************************Functions**************************************/

Eurovision eurovisionCreate(){
  /***********
  TODO: CHECK
  ***********/
  Eurovision eurovision = malloc(sizeof(*eurovision));
  assert(eurovision);
  if(!eurovision) return NULL;

  eurovision->states = NULL;
  eurovision->judges = NULL;
  eurovision->votes = NULL;
  eurovision->audiencePercent = 1;
}
