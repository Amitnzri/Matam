#ifndef STATE_H
#define STATE_H

#include "../include/defines.h"

typedef struct State_t *State;

//Creates new state.
State stateCreate(int id,const char* name,const char* song,
                                          ContestValues contest_values);
//return the ID of a given state.
int stateGetId(State state);
//Returns the name of a given state (shallow copy).
char* stateGetName(State state);
//Returns the the name of a given state's song. (shallow copy).
char* stateGetSong(State state);
//Returns the top ten array of a given state. (shallow copy).
int* stateGetVotes(State state);
//Returns the contest values (shallow copy).
ContestValues stateGetContestValues(State state);
//Returns the number of votes the state has earned by judges.
int stateGetScoreByJudges(State state);
//Returns the number of votes the state has earned by other states.
int stateGetScoreByStates(State state);
//Gives a vote to a given state.
void stateAddVote(State giver_state,int taker_state_id);
//Removes a vote to a given state.
void stateRemoveVote(State giver_state,int taker_state_id);
//Updates score_by_judges or score_by_states.
void stateUpdateVotes(State state,Voter who_voted,int n);
//Destroys the state
void stateDestroy(State state);
//Creates a new copy of the state.
State stateCopy(State state);
//Update the top ten array.
void stateUpdateTopTen(State state);
//Clears all the votes the state has given to other states.
void stateClearVotes(State state,int other_state_id);

#endif
