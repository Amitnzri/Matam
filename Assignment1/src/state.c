#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h> //TODO:remove
#include "../include/state.h"
#include "../include/map.h"
#include "../include/defines.h"

struct State_t{
    int id;
    char* name;
    char* song;
    int* top_ten;
    Map votes_map;
    ContestValues contest_values;
    int score_by_judges;
    int score_by_states;
};

/*****************************StaticFunctions********************************/
//Copies a name.
static char* copyName(const char* name){
    int len = strlen(name)+1;
    char* copy = malloc(sizeof(*copy)*len);
    if(!copy) return NULL;
    strcpy(copy,name);
    return copy;
}
/*
//Copies an array of ints.
static int* copyArray(const int* array, int len){
    if(!array)return NULL;
    int* copy = malloc(sizeof(*copy)*len);
    if(!copy) return NULL;
    for(int i=0;i<len;i++){
        copy[i] = array[i];
    }
    return copy;
}
*/
//Resets top ten array's values to NONE.
static void resetArray(int* array){
    assert(array);
    for(int i=0;i<TOP_TEN_LEN;i++){
        array[i] = NONE;
    }
}
//Copies an int.
static MapKeyElement copyInt (MapKeyElement key){
    /***********
    TODO: Check
    ***********/
    assert(key);
    if(!key) return NULL;
    int* copy = malloc(sizeof(*copy));
    assert(copy);
    if(!copy) return NULL;
    *copy = *(int*)key;
    return (MapKeyElement) copy;
}
//Free int allocation.
static void freeInt(MapKeyElement n){
    /**********
    TODO: Check
    **********/
    free(n);
}
//Comapares two int keys.
static int compareIntKeys(MapKeyElement key_a,MapKeyElement key_b){

    assert(key_a&&key_b);
    return *(int*)key_a - *(int*)key_b;
}

/*****************************Functions**************************************/
State stateCreate(int id,const char* name,const char* song,ContestValues contest_values){
    assert(name&&song);
    State new_state = malloc(sizeof(*new_state));
    if(!new_state)return NULL;
    new_state->id = id;
    new_state->name = copyName(name);
    new_state->song = copyName(song);
    if(!new_state->name||!new_state->song){
        free(new_state->name);
        free(new_state->song);
        free(new_state);
        return NULL;
    }
    new_state->top_ten = NULL;
    new_state->votes_map = NULL;
    new_state->contest_values = contest_values;
    new_state->score_by_judges = 0;
    new_state->score_by_states = 0;
    return new_state;
}

int stateGetId(State state){
    return state->id;
}

char* stateGetName(State state){
    if(!state)return NULL;
    return state->name;
}

char* stateGetSong(State state){
    return state->song;
}

int* stateGetVotes(State state){
    if(!state)return NULL;
    return state->top_ten;
}

ContestValues stateGetContestValues(State state){
    assert(state);
    return state->contest_values;
}

int stateGetScoreByJudges(State state){
    assert(state);
    return state->score_by_judges;
}

int stateGetScoreByStates(State state){
    assert(state);
    return state->score_by_states;
}

void stateAddVote(State giver_state,int taker_state_id){
    assert(giver_state);
    if(!giver_state)return;
    if(!giver_state->votes_map){
        giver_state->votes_map = mapCreate(copyInt,copyInt,freeInt,freeInt,
                                                           compareIntKeys);
        if(!giver_state->votes_map)return;
    }
    int num_of_votes = 0;
    if(mapContains(giver_state->votes_map,&taker_state_id)){
        num_of_votes = *(int*) mapGet(giver_state->votes_map,&taker_state_id);
    }
    num_of_votes += SINGLE_VOTE;
    mapPut(giver_state->votes_map,&taker_state_id,&num_of_votes);
}

void stateRemoveVote(State giver_state,int taker_state_id){
    assert(giver_state);
    if(!giver_state || !giver_state->votes_map)return;
    if(!mapContains(giver_state->votes_map,&taker_state_id))return;
    int num_of_votes = *(int*)mapGet(giver_state->votes_map,&taker_state_id);
    num_of_votes-=SINGLE_VOTE;
    if(num_of_votes>0){
        mapPut(giver_state->votes_map,&taker_state_id,&num_of_votes);
    }else{
        mapRemove(giver_state->votes_map,&taker_state_id);
    }
}

void stateUpdateVotes(State state,Voter who_voted,int n){
    switch (who_voted) {
        case JUDGE:
            state->score_by_judges += n;
            assert(state->score_by_judges>=0);
            break;
        case STATE:
            state->score_by_states += n;
            assert(state->score_by_states>=0);
            break;
        default:
            assert(0);
            break;
        }

}

void stateDestroy(State state){
    if(!state) return;
    free(state->name);
    free(state->song);
    free(state->top_ten);
    mapDestroy(state->votes_map);
    free(state);
}

State stateCopy(State state){
    assert(state);
    bool clear = false;
    if(!state)return NULL;
    State state_copy = malloc(sizeof(*state_copy));
    if(!state_copy)return NULL;
    state_copy->id = state->id;

    state_copy->name = stateGetName(state);
    if(!state_copy->name)clear = true;
    state_copy->song = stateGetSong(state);
    if(!state_copy->song)clear = true;
    state_copy->top_ten = stateGetVotes(state);
    if(state->top_ten&&!state_copy->top_ten)clear = true;
    state_copy->votes_map = mapCopy(state->votes_map);
    if(state->votes_map&&!state_copy->votes_map)clear = true;

    if(clear){
        free(state_copy->name);
        free(state_copy->song);
        free(state_copy->top_ten);
        free(state_copy->votes_map);
        free(state_copy);
        return NULL;
    }
    state_copy->contest_values =stateGetContestValues(state);
    state_copy->score_by_states =stateGetScoreByStates(state);
    state_copy->score_by_judges =stateGetScoreByJudges(state);

    return state_copy;
}

void stateUpdateTopTen(State state){
    /*********
    TODO:Check
    *********/
    if(!state||!state->votes_map)return;
    if(!state->top_ten){
        state->top_ten = malloc(sizeof(*state->top_ten)*TOP_TEN_LEN);
    }
    resetArray(state->top_ten);
    int* state_id = (int*) mapGetFirst(state->votes_map);
    while(state_id){
        for(int i=0;i<TOP_TEN_LEN;i++){ //Scans the array.
            int index_state_votes = NONE;
            if(state->top_ten[i]!=NONE){ //if There is a state in the index location.
                index_state_votes = *(int*)mapGet(state->votes_map,
                                                  &state->top_ten[i]);
            }
            int state_votes = *(int*) mapGet(state->votes_map,state_id);
            if(state_votes>index_state_votes&&state_votes!=0){
                if(state->top_ten[i]==NONE) {
                    state->top_ten[i] = *state_id;
                    state_id = (int*)mapGetNext(state->votes_map);
                    break;
                }else{
                    state->top_ten[i]=*state_id;
                    state_id = (int*) mapGetFirst(state->votes_map);
                    break;
                }
            }else if(state_votes==index_state_votes){
                //Same Countries
                if(*state_id == state->top_ten[i]){
                state_id = (int*) mapGetNext(state->votes_map);
                break;
                }
                //Different Countries.
                else if(*state_id<state->top_ten[i]){
                    state->top_ten[i] =*state_id;
                    state_id = (int*) mapGetFirst(state->votes_map);
                    break;
                }else{
                    if(i!= TOP_TEN_LEN-1)continue;
                }
            }
            if(i == TOP_TEN_LEN-1){ //Got to the end of the array.
            state_id = (int*) mapGetNext(state->votes_map);
            }
        }
    }
}

void stateRemoveAllVotesToState(State state,int other_state_id){
    while(mapContains(state->votes_map,&other_state_id)){
        stateRemoveVote(state,other_state_id);
    }
}
