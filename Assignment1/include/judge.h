#ifndef JUDGE_H
#define JUDGE_H

typedef struct Judge_t *Judge;


/********************************************
judgeCreate:
Creates new instance of Judge.
Returns:
New copy of judge if succeeded.
NULL if failed or null argument.
********************************************/
Judge judgeCreate(int id,const char* name,int* votes);
/**********************
judgeDestroy:
Free Judge from memory.
***********************/
void judgeDestroy(Judge judge);
/*******************************
judgeGetVotes:
(shallow copy)
Returns:
Judge's top ten array.
NULL if null argument has given.
*******************************/
int* judgeGetVotes(Judge judge);
/*******************************
judgeGetName:
Returns:
The name of the judge.
NULL if null argument has given.
*******************************/
char* judgeGetName(Judge judge);
/*******************************
judgeCopy:
Creates new copy of given judge.
Returns:
The copy if succeeded.
NULL argument if failed.
*******************************/
Judge judgeCopy(Judge judge);

#endif
