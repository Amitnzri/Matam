#ifndef STATE_H
#define STATE_H

#include "../include/defines.h"

typedef struct State_t *State;

State stateCreate(int id,const char* name,const char* song,ContestValues contest_values);

int stateGetId(State state);

char* stateGetName(State state);

char* stateGetSong(State state);

int* stateGetVotes(State state);

ContestValues stateGetContestValues(State state);

int stateGetScoreByJudges(State state);

int stateGetScoreByStates(State state);

void stateAddVote(State giver_state,int taker_state_id);

void stateRemoveVote(State giver_state,int taker_state_id);

void stateUpdateVotes(State state,Voter who_voted,int n);

void stateDestroy(State state);

State stateCopy(State state);

void stateUpdateTopTen(State state);

void stateRemoveAllVotesToState(State state,int other_state_id);

#endif
