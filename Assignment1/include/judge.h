#ifndef JUDGE_H
#define JUDGE_H

typedef struct Judge_t *Judge;


/*****************************
judgeCreate:
Creates new instance of Judge.
*****************************/
Judge judgeCreate(int id,const char* name,int* votes);
/**********************
judgeDestroy:
Free Judge from memory.
***********************/
void judgeDestroy(Judge judge);
/************************************
judgeGetVotes:
Returns a copy of judge's votes.
Caller's responsibility to free after use.
************************************/
int* judgeGetVotes(Judge judge);
/************************************
judgeGetName:
Returns a copy of judge's name.
Caller's responsibility to free after use.
************************************/
char* judgeGetName(Judge judge);
/************************************
judgeCopy:
Creates new copy of given judge.
************************************/
Judge judgeCopy(Judge judge);

#endif
