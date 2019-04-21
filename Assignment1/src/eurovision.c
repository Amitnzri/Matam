#include <stdio.h>
#include <stdlib.h>
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






/*****************************Functions**************************************/
