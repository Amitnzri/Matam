#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "eurovision.h"
#include <string.h>
#include "map.h"
#include "list.h"


/*****************************Defines&Typedefs*******************************/

#define TOP_TEN_LEN 10
#define SPACE 32
#define NONE -1
#define SINGLE_VOTE 1

typedef enum {
    JUDGE,
    STATE
}Voter;

typedef enum {
  ADD = 1,
  SUBTRACT = -1
} Operation;

/*****************************DataSturctures*********************************/

struct eurovision_t{
    /***********
    TODO: Check
    ***********/
    Map states_map;
    Map judges_map;
    List scores_table;
    int audiencePercent; //set to one by default
};

typedef struct state_t{
    /***********
    TODO: Check
    ***********/
    int id;
    char* name;
    char* song;
    int* top_ten;
    Map votes;
    int* audiencePercent;
    int score_by_judges;
    int score_by_audience;
}*State;

typedef struct judge_t{
    /***********
    TODO: Check
    ***********/
    int id;
    char* name;
    int* top_ten;
}*Judge;

/*****************************StaticFunctions********************************/
//Copies an int array.
static int* copyIntArray(const int *source, int len){
    /***********
    TODO: Checks
    ***********/
    if(source == NULL)return NULL;
    int* destination = malloc(sizeof(*destination)* len);
    if(!destination) return NULL;
    for(int i=0; i<len; i++){
    destination[i] = source[i];
    }
    return destination;
}
//Copies a string.
char* copyStr(const char* str){
/*********
TODO:CHECK
*********/
assert(str);
if(!str)return NULL;
char* copy = malloc(sizeof(*copy)*(strlen(str)+1));
if(!copy) return NULL;
strcpy(copy,str);
return copy;
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
//Copies a judge.
static MapDataElement copyJudge(MapDataElement judge){
    /***********
    TODO: Check
    ***********/
    if(!judge) return NULL;
    Judge source =(Judge) judge;
    Judge copy = malloc(sizeof(*copy));
    assert(copy);
    if(!copy) return NULL;
    copy->id = source->id;
    copy->name = copyStr(source->name);
    copy->top_ten = copyIntArray(source->top_ten,TOP_TEN_LEN);
    assert(copy->top_ten&&copy->name);
    if(!copy->name||!copy->top_ten){
        free(copy->name);
        free(copy->top_ten);
        free(copy);
        return NULL;
    }

    return (MapDataElement) copy;

}
//Free judge allocation.
static void freeJudge(MapDataElement judge){ //needed void*
    /***********
    TODO: Check
    1.free name.
    ***********/
    assert(judge);
    if(!judge) return;
    Judge remove = (Judge) judge;
    free(remove->name);
    free(remove->top_ten);
    free(judge);
}
//Free int allocation.
static void freeInt(MapKeyElement n){
    /**********
    TODO: Check
    **********/
    free(n);
}
//Free state allocation.
static void freeState(MapDataElement state){
  /*********
  TODO:Check
  *********/

  assert(state);
  if(state == NULL)return;
  State remove = (State) state;
  free(remove->name);
  free(remove->song);
  if(remove->top_ten)free(remove->top_ten);
  if(remove->votes)mapDestroy(remove->votes);
  free(remove);
}
//Comapares two int keys.
static int compareIntKeys(MapKeyElement key_a,MapKeyElement key_b){

    assert(key_a&&key_b);
    return *(int*)key_a - *(int*)key_b;
}
//Checks if ID is valid.
static bool checkId(int state_id){

    return (state_id<0) ? (false) : (true); //TODO:check if can use stdargs
} //Checked
//Checks if name is valid.
static bool checkName(const char *state_name){

    int len = strlen(state_name);
    for(const char *c = state_name ;c<state_name+len ;c++){
        if((*c<'a'||*c>'z')&&(*c!=SPACE))return false;
    }
    return true;
}//Checked
//Ckecks if  an array of ID numbers is valid.
static bool checkArrayValues(int len, int* judge_results){
    /***********
    TODO: Check
    ***********/
    assert(judge_results);
    if(!judge_results) return false;
    for(int i = 0;i<len;i++){
        if(!checkId(judge_results[i]))return false;
    }
    return true;
}
//Checks if judges votes are valid.
static bool checkJudgeResults(int* judge_results, Map states_map){
    /***********
    TODO: Check
    ***********/
    if(!states_map) return false; //if there's no states
    for(int i=0;i<TOP_TEN_LEN;i++){
        if(!mapContains(states_map,&judge_results[i])) return false;
        for(int j=0;j<TOP_TEN_LEN;j++){ //Checks for duplications
            if(j==i) continue;
            if(judge_results[i]==judge_results[j]) return false;
        }
    }
    return true;
}
//Creates a new state.
static State createNewState(int state_id,int* audiencePercent,const char* state_name,
                            const char* song_name){

  /*********
  TODO:Check
  *********/
  assert(state_name&&song_name);
  State new_state = malloc(sizeof(*new_state));
  if(new_state == NULL)return NULL;
  new_state->name = copyStr(state_name);
  new_state->song = copyStr(song_name);

  //if one of the mallocs has failed, free all and return null.
  if(new_state->name  == NULL || new_state->song  == NULL){
    free(new_state->name);
    free(new_state->song);
    free(new_state);
    return NULL;
  }

  new_state->id = state_id;
  new_state->top_ten = NULL;
  new_state->votes = NULL;
  new_state->audiencePercent = audiencePercent;
  new_state->score_by_judges = 0;
  new_state->score_by_audience = 0;
  return new_state;
}
//Copies a state.
static MapDataElement copyState(MapDataElement state){
    /***********
    TODO: Check
    1.check types
    ***********/
    if(!state) return NULL;
    State source = (State) state;
    State copy = createNewState(source->id,source->audiencePercent
                                ,source->name,source->song);
    assert(copy);
    if(!copy) return NULL;
    copy->top_ten = copyIntArray(source->top_ten,TOP_TEN_LEN); //can be null.
    copy->votes = mapCopy(source->votes); //can be null.

    copy->score_by_judges = source->score_by_judges;
    copy->score_by_audience = source->score_by_audience;

    return (MapDataElement) copy;

}
//Creates shallow copy to a state.
static ListElement copyStateShallow(CopyListElement state_ptr){
    assert(state_ptr);
    return state_ptr;
}
//Do Nothing.
//the list holds shallow copies so we dont want to let it remove states.
static void freeListState(FreeListElement state_ptr){
    return;
}
//Creates a new Judge.
static Judge createNewJudge(int judge_id,const char *judge_name,
                         int *judge_results){
    /***********
   TODO: Check
   ***********/
    assert(judge_name&&judge_results);

    Judge new_judge = malloc(sizeof(*new_judge));
    if(!new_judge) return NULL;

    new_judge->name = copyStr(judge_name);
    new_judge->top_ten = copyIntArray(judge_results,TOP_TEN_LEN);

    //if one of the mallocs has failed, free all and return null.
    if(new_judge->name  == NULL || new_judge->top_ten  == NULL){
        free(new_judge->name);
        free(new_judge->top_ten);
        free(new_judge);
        return NULL;
    }

    new_judge->id = judge_id;
    return new_judge;
}
//Compare scores by the audience score.
static int compareAudienceScore(ListElement element_a, ListElement element_b){
        /***********
        TODO: Check
        ***********/
    assert(element_a&&element_b);
    State state_a = (State) element_a;
    State state_b = (State) element_b;
    return (state_a->score_by_audience) - (state_b->score_by_audience);
}
//Converts the location from the voting table to points.
static int convertPlaceToPoints(int i){
    assert(0<=i && i<TOP_TEN_LEN);
    return (i<2) ? (12-2*i) : (10-i);
}//Checked
//Add or Remove all votes from TopTen Array.
static void addOrRemoveOwnVotes(Map states_map,int* top_ten,Voter who_voted,
                                                            Operation sign){
/*********
TODO:Check
*********/
assert(states_map&&top_ten);
for(int i=0;i<TOP_TEN_LEN;i++){
    State state = mapGet(states_map,&top_ten[i]);
    if(!state) continue;
    switch (who_voted) {
        case JUDGE:
            state->score_by_judges += convertPlaceToPoints(i)*sign;
            assert(state->score_by_judges>=0);
            break;

        case STATE:
            state->score_by_audience += convertPlaceToPoints(i)*sign;
            assert(state->score_by_audience>=0);
            break;
    }
}


}
//Checks if topTen array contains a given state id.
static bool checkIfArrayContainsID(int* top_ten,int state_id){
    for (int i=0;i<TOP_TEN_LEN;i++){
        if(top_ten[i]== state_id)return false;
    }
    return true;
}
//Removes a judge and his votes if he voted to a removed state.
static void fireTheVotingJudges(Eurovision eurovision,int state_id){
/*********
TODO:Check
*********/
    assert(eurovision);
    Map judges_map = eurovision->judges_map;
    if(!judges_map)return;
    MapKeyElement judge_id = mapGetFirst(judges_map);
    //Scans every judge in the map and checks his votes.
    while(judge_id)
    {
        Judge judge = mapGet(judges_map,judge_id);
        int* top_ten = judge->top_ten;
        if(checkIfArrayContainsID(top_ten,state_id)){
            addOrRemoveOwnVotes(eurovision->states_map,top_ten,JUDGE,SUBTRACT);
            mapRemove(judges_map,judge_id);
            judge_id = mapGetFirst(judges_map);
        }else{
            judge_id = mapGetNext(judges_map);
        }
    }
}
//Sets array's value to NONE.
static void resetArray(int* arr){
    assert(arr);
    for(int i=0;i<TOP_TEN_LEN;i++){
        arr[i] = NONE;
    }
}
//Updates the top_ten array.
static EurovisionResult updateTopTen(Map votes_map,int* top_ten){
    /*********
    TODO:Check
    *********/
    assert(top_ten);
    if(!votes_map || !top_ten){
        return EUROVISION_NULL_ARGUMENT;
    }
    resetArray(top_ten);
    int* state_id = (int*) mapGetFirst(votes_map);
    while(state_id){
        for(int i=0;i<TOP_TEN_LEN;i++){
            int index_state_votes = NONE;
            if(top_ten[i]!=NONE){
                index_state_votes = *(int*) mapGet(votes_map,&top_ten[i]);
            }
            int state_votes = *(int*) mapGet(votes_map,state_id);
            if(state_votes == 0 || state_votes == index_state_votes){
                state_id = (int*) mapGetNext(votes_map);
                break;
            }else if(state_votes > index_state_votes){
                top_ten[i] = *state_id;
                state_id = (int*) mapGetFirst(votes_map);
                break;
            }
        }
    }
      return EUROVISION_SUCCESS;
}
//Cancels votes of other states to removed State.
static void cancelOtherStatesVotes(Map states_map,int removed_state){
      assert(states_map);
      MapKeyElement state_id = mapGetFirst(states_map);
      //Scans every state in the map and checks his votes.
      while(state_id)
      {
          State state = mapGet(states_map,state_id);
          if(state->votes){
              if(mapContains(state->votes,&state_id))
              {
                  mapRemove(state->votes,&removed_state);
                  addOrRemoveOwnVotes(states_map,state->top_ten,STATE,SUBTRACT);
                  updateTopTen(state->votes,state->top_ten);
                  addOrRemoveOwnVotes(states_map,state->top_ten,STATE,ADD);
              }
          }
          state_id = mapGetNext(states_map);
      }
  }
//Compares Two states by their final score.
static int compareFinalScore(ListElement score_a, ListElement score_b){
    /***********
     TODO: Check
     ***********/
    return *(int*)score_a - *(int*)score_b;
}
/*****************************Functions**************************************/

Eurovision eurovisionCreate(void){
    /***********
    TODO: Check
    ***********/
    Eurovision eurovision = malloc(sizeof(*eurovision));
    assert(eurovision);
    if(!eurovision) return NULL;

    eurovision->states_map = NULL;
    eurovision->judges_map = NULL;
    eurovision->scores_table = NULL;
    eurovision->audiencePercent = 1;
    return eurovision;
}

EurovisionResult eurovisionAddState(Eurovision eurovision,
                                    int stateId,
                                    const char *stateName,
                                    const char *songName){

    /*********
    TODO:Check
    *********/
    assert(eurovision&&stateName&&songName);
                                                        //Validation Checks
    if(!eurovision||!stateName||!songName)return EUROVISION_NULL_ARGUMENT;
    if(!checkName(stateName)
       ||!checkName(songName))return EUROVISION_INVALID_NAME;
    if(!checkId(stateId)) return EUROVISION_INVALID_ID;


    if(!eurovision->states_map){
        eurovision->states_map = mapCreate(copyState,copyInt,freeState
                                       ,freeInt,compareIntKeys);
        assert(eurovision->states_map);
        if(eurovision->states_map == NULL) return EUROVISION_OUT_OF_MEMORY;
    } //Creates states dictionary if there isn't one.
    Map states_map = eurovision->states_map;

    if(mapContains(states_map,&stateId)) return EUROVISION_STATE_ALREADY_EXIST;
    State tmp_state = createNewState(stateId,&eurovision->audiencePercent
                                    ,stateName,songName); //
    if(mapPut(states_map,&stateId,tmp_state) == MAP_OUT_OF_MEMORY){
        freeState(tmp_state);
        return EUROVISION_OUT_OF_MEMORY;
    }else{
        freeState(tmp_state);
        return EUROVISION_SUCCESS;
    }
}

EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId){
    /*********
    TODO:Check
    *********/
    assert(eurovision);
    if(!eurovision)return EUROVISION_NULL_ARGUMENT;
    else if(!eurovision->states_map)return EUROVISION_STATE_NOT_EXIST;
    if(!mapContains(eurovision->states_map,&stateId)){
        return EUROVISION_STATE_NOT_EXIST;
    }
    State remove = (State) mapGet(eurovision->states_map,&stateId);

    if(remove->votes) addOrRemoveOwnVotes(eurovision->states_map,
                                       remove->top_ten,STATE,SUBTRACT);
    if(remove->score_by_judges>0)fireTheVotingJudges(eurovision,stateId);
    if(remove->score_by_audience>0)cancelOtherStatesVotes(eurovision->states_map
                                                         ,stateId);
    if(mapGetSize(eurovision->states_map)<=1){
        mapDestroy(eurovision->states_map);
        eurovision->states_map=NULL;
    }else{
        mapRemove(eurovision->states_map,&stateId);
    }

    return EUROVISION_SUCCESS;

}

EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId,
                                    const char *judgeName,
                                    int *judgeResults) {
    /***********
   TODO: Check
   ***********/
    assert(eurovision && judgeName && judgeResults);
    if (!eurovision  || !judgeName || !judgeResults){
        return EUROVISION_NULL_ARGUMENT;
    }
    if (!checkName(judgeName)) return EUROVISION_INVALID_NAME;
    if (!checkArrayValues(TOP_TEN_LEN,judgeResults)){
        return EUROVISION_INVALID_ID;
    }
    if (!checkId(judgeId))return EUROVISION_INVALID_ID;

    if (!eurovision->judges_map) {
        eurovision->judges_map = mapCreate(copyJudge, copyInt, freeJudge,
                                           freeInt, compareIntKeys);
        assert(eurovision->judges_map);
        if (eurovision->judges_map == NULL) return EUROVISION_OUT_OF_MEMORY;

    }//Creates states dictionary if there isn't one.
    Map judges_map = eurovision->judges_map;
        if (mapContains(judges_map, &judgeId)) {
            return EUROVISION_JUDGE_ALREADY_EXIST;
        }
        if (!checkJudgeResults(judgeResults, eurovision->states_map)){
            return EUROVISION_STATE_NOT_EXIST;
        }
        Judge tmp_judge = createNewJudge(judgeId, judgeName, judgeResults);

        if (mapPut(judges_map, &judgeId, tmp_judge) == MAP_OUT_OF_MEMORY) {
            freeJudge(tmp_judge);
            return EUROVISION_OUT_OF_MEMORY;
        } else {
            freeJudge(tmp_judge);
            addOrRemoveOwnVotes(eurovision->states_map,judgeResults,JUDGE,ADD);
            return EUROVISION_SUCCESS;
        }
   // updateScore(eurovision) yet to write
}

EurovisionResult eurovisionRemoveJudge(Eurovision eurovision, int judgeId){
    /*********
    TODO:Check
    *********/
    assert(eurovision);
    if(!eurovision)return EUROVISION_NULL_ARGUMENT;
    if(!eurovision->judges_map)return EUROVISION_JUDGE_NOT_EXIST;
    if(!mapContains(eurovision->judges_map,&judgeId)){
        if(mapGetSize(eurovision->judges_map)<1){
            mapDestroy(eurovision->judges_map);
            eurovision->judges_map=NULL;
        }
        return EUROVISION_JUDGE_NOT_EXIST;
    }
    Judge remove = (Judge) mapGet(eurovision->judges_map,&judgeId);
    addOrRemoveOwnVotes(eurovision->states_map,remove->top_ten,JUDGE,SUBTRACT);
    if(mapGetSize(eurovision->judges_map)<=1){
        mapDestroy(eurovision->judges_map);
        eurovision->judges_map=NULL;
    }else{
        mapRemove(eurovision->judges_map,&judgeId);
    }
    return EUROVISION_SUCCESS;
}

void eurovisionDestroy(Eurovision eurovision){

    mapDestroy(eurovision->states_map);
    mapDestroy(eurovision->judges_map);
    listDestroy(eurovision->scores_table);
    free (eurovision);

}

EurovisionResult eurovisionAddVote (Eurovision eurovision,int stateGiver,
                                                           int stateTaker){
    /*********
    TODO:Check
    *********/
    assert(eurovision);
    if (!eurovision) return EUROVISION_NULL_ARGUMENT;
    if ((!checkId(stateGiver)) || (!checkId(stateTaker))){
         return EUROVISION_INVALID_ID;
    }
    if (!mapContains(eurovision->states_map,&stateGiver)||
        !mapContains(eurovision->states_map,&stateTaker)){
        return EUROVISION_STATE_NOT_EXIST;
    }
    if (stateGiver == stateTaker) return EUROVISION_SAME_STATE;
    State voter_state = mapGet(eurovision->states_map, &stateGiver);
    if (!voter_state->votes) {
        voter_state->votes =mapCreate(copyInt, copyInt, freeInt, freeInt,
                                                        compareIntKeys);
        if (!voter_state->votes) return EUROVISION_OUT_OF_MEMORY;
        voter_state->top_ten=malloc(sizeof(*voter_state->top_ten)*TOP_TEN_LEN);
        if(!voter_state->top_ten){
            mapDestroy(voter_state->votes);
            return EUROVISION_OUT_OF_MEMORY;
        }
        resetArray(voter_state->top_ten);
    }
    Map votes_map = voter_state->votes;
    if (!mapContains(votes_map, &stateTaker)){
        int single_vote = SINGLE_VOTE;
        mapPut(votes_map, &stateTaker,&single_vote);
    }else{
        int vote_update;
        vote_update = *(int*) mapGet(votes_map, &stateTaker);
        vote_update += SINGLE_VOTE;
        mapPut(votes_map,&stateTaker,&vote_update);
    }

    addOrRemoveOwnVotes(eurovision->states_map,voter_state->top_ten,STATE,
                                                                    SUBTRACT);
    updateTopTen(votes_map,voter_state->top_ten);
    addOrRemoveOwnVotes(eurovision->states_map,voter_state->top_ten,STATE,ADD);

    return EUROVISION_SUCCESS;
}

EurovisionResult eurovisionRemoveVote (Eurovision eurovision, int stateGiver,
                                                              int stateTaker){
/*********
TODO:Check
*********/
assert(eurovision);
if(!eurovision) EUROVISION_NULL_ARGUMENT;
Map states_map = eurovision->states_map;
if(stateGiver == stateTaker) return EUROVISION_SAME_STATE;
if(!checkId(stateGiver),!checkId(stateTaker))EUROVISION_INVALID_ID;
if(!states_map ||!mapContains(states_map,&stateGiver)
   ||!mapContains(states_map,&stateTaker))return EUROVISION_STATE_NOT_EXIST;
State giver_state = (State) mapGet(states_map,&stateGiver);
if(giver_state->votes){
    int* num_of_votes = (int*) mapGet(giver_state->votes,&stateTaker);
    if(num_of_votes && *num_of_votes>0) *num_of_votes--;
}
return EUROVISION_SUCCESS;
}
