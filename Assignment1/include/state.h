#ifndef STATE_H
#define STATE_H

#include "../include/defines.h"

typedef struct State_t *State;

/******************************************
stateCreate:
Creates a new state.
Returns:
NULL if failed to allocate or null argument
the new state if succeeded.
******************************************/
State stateCreate(int id,const char* name,const char* song,
                                          ContestValues contest_values);
/***********************
stateGetId:
Returns:
the ID of a given state.
NUll if NULL argument.
***********************/
int stateGetId(State state);
/******************************
stateGetName:
(shallow copy)
Returns:
the name of a given state.
NULL_ARGUMENT if NULL argument.
******************************/
char* stateGetName(State state);
/************************************
stateGetSong:
(shallow copy)
Returns:
the the name of a given state's song.
NUll if NULL argument.
************************************/
char* stateGetSong(State state);
/**********************************
stateGetVotes:
(shallow copy)
Returns:
the top ten array of a given state.
NUll if NULL argument.
**********************************/
int* stateGetVotes(State state);
/*************************
stateGetContestValues:
(shallow copy)
Returns:
the contest values
NUll if NULL argument.
*************************/
ContestValues stateGetContestValues(State state);
/**************************************************
stateGetScoreByJudges:
Returns:
the number of votes the state has earned by judges.
NULL_ARGUMENT if NULL argument.
**************************************************/
int stateGetScoreByJudges(State state);
/********************************************************
stateGetScoreByStates
Returns:
the number of votes the state has earned by other states.
NULL_ARGUMENT if NULL argument.
********************************************************/
int stateGetScoreByStates(State state);
/****************************
stateAddVote:
Give a vote to a given state.
****************************/
void stateAddVote(State giver_state,int taker_state_id);
/********************************
stateRemoveVote:
Remove a vote from a given state.
********************************/
void stateRemoveVote(State giver_state,int taker_state_id);
/****************************************
stateUpdateVotes:
Update score_by_judges or score_by_state
****************************************/
void stateUpdateVotes(State state,Voter who_voted,int n);
/*****************
stateDestroy:
Destroy the state.
*****************/
void stateDestroy(State state);
/*********************************************
stateCopy:
Creates a new copy of the state.
Returns:
the copy of the state if allocation succeeded.
NULL if failed.
*********************************************/
State stateCopy(State state);
/************************
stateUpdateVotes:
Update the top_ten array.
************************/
void stateUpdateTopTen(State state);
/********************************************************
stateClearVotes:
Clears all the votes the state has given to other states.
********************************************************/
void stateClearVotes(State state,int other_state_id);

#endif
