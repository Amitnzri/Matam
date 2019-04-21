#include <stdio.h>
#include <stdlib.h>
#include "eurovision.h"
#include "map.h"


/*****************************DataSturctures*********************************/
sturct eurovision_t{
  /***********
  TODO: CHECK
  ***********/
  Map states;
  Map judges;
  List Votes;
  int audiencePercent; //set to one by default

}

typedef sturct state_t{
  /***********
  TODO: CHECK
  ***********/
  int id;
  char* name;
  char* song;
  int[10] top_ten;
  Map votes;
  int score_by_judges;
  int score_by_audience;

}*State;




/*****************************StaticFunctions********************************/






/*****************************Functions**************************************/
