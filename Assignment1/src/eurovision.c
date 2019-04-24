#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "eurovision.h"
#include <string.h>
#include "map.h"


/*********************************Defines************************************/

#define TOP_TEN_LEN 10
#define SPACE 32

/*****************************DataSturctures*********************************/

struct eurovision_t{
    /***********
    TODO: Check
    ***********/
    Map states;
    Map judges;
    List votes;
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

char* copyStr(const char* str){
/*********
TODO:CHECK
*********/
assert(str);
if(!str)return NULL;
char* copy = malloc(sizeof(*copy)*strlen(str));
if(!copy) return NULL;
strcpy(copy,str);
return copy;
}

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
    assert(copy->top_ten);
    if(!copy->top_ten)return NULL;
    return (MapDataElement) copy;

}

static void freeJudge(MapDataElement judge){ //needed void*
    /***********
    TODO: Check
    1.free name.
    ***********/
    assert(judge);
    Judge remove = (Judge) judge;
    if(!remove->name) {
        free(remove);
    }
}

static void freeInt(MapKeyElement n){
    /**********
    TODO: Check
    **********/
    free(n);
}

static void freeState(MapDataElement state){
  /*********
  TODO:Check
  *********/

  assert(state);
  if(state == NULL)return;
  State remove = (State) remove;
  free(remove->name);
  free(remove->song);
  free(remove);
}

static int compareIntKeys(MapKeyElement key_a,MapKeyElement key_b){
  /*********
  TODO:Check
  *********/
  assert(key_a&&key_b);
  return *(int*)key_a - *(int*)key_b;
}

static bool checkId(int state_id){
    return (state_id<0) ? (false) : (true);
}//Checked

static bool checkName(const char *state_name){

    int len = strlen(state_name);
    for(const char *c = state_name ;c<state_name+len ;c++){
        if((*c<'a'||*c>'z')&&(*c!=SPACE))return false;
    }
    return true;
}//Checked


static bool checkArrayValues (int len, int* judge_results){
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

static bool checkJudgeResults(int* judge_results, Map states){
    /***********
    TODO: Check
    ***********/
    if(!states) return false; //if there's no states
    for(int i=0;i<TOP_TEN_LEN;i++){
        if(!mapContains(states,&judge_results[i])) return false;
    }
    return true;
}


static State createNewState (int state_id ,const char* state_name,const char* song_name){

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
  new_state->score_by_judges = 0;
  new_state->score_by_audience = 0;
  return new_state;
}


static MapDataElement copyState(MapDataElement state){
    /***********
    TODO: Check
    1.check types
    ***********/
    if(!state) return NULL;
    State source = (State) state;
    State copy = createNewState(source->id,source->name,source->song);
    assert(copy);
    if(!copy) return NULL;
    copy->top_ten = copyIntArray(source->top_ten,TOP_TEN_LEN); //can be null.
    copy->votes = mapCopy(source->votes); //can be null.

    copy->score_by_judges = source->score_by_judges;
    copy->score_by_audience = source->score_by_audience;

    return (MapDataElement) copy;

}

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



/*****************************Functions**************************************/

Eurovision eurovisionCreate(){
    /***********
    TODO: Check
    ***********/
    Eurovision eurovision = malloc(sizeof(*eurovision));
    assert(eurovision);
    if(!eurovision) return NULL;

    eurovision->states = NULL;
    eurovision->judges = NULL;
    eurovision->votes = NULL;
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


    if(!eurovision->states){
        eurovision->states = mapCreate(copyState,copyInt,freeState
                                       ,freeInt,compareIntKeys);
        assert(eurovision->states);
        if(eurovision->states == NULL) return EUROVISION_OUT_OF_MEMORY;
    } //Creates states dictionary if there isn't one.
    Map states = eurovision->states;

    if(mapContains(states,&stateId)) return EUROVISION_STATE_ALREADY_EXIST;
    State new_state = createNewState(stateId,stateName,songName);
    if(mapPut(states,&stateId,new_state) == MAP_OUT_OF_MEMORY){
        return EUROVISION_OUT_OF_MEMORY;
    }else{
        return EUROVISION_SUCCESS;
    }
}


EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId,
                                    const char *judgeName,
                                    int *judgeResults) {
    /***********
   TODO: Check
   ***********/
    assert(eurovision && judgeName && judgeResults);
    if (!eurovision  || !judgeName || !judgeResults) return EUROVISION_NULL_ARGUMENT;

    if (!checkName(judgeName)) return EUROVISION_INVALID_NAME;
    if (!checkArrayValues(TOP_TEN_LEN,judgeResults)) return EUROVISION_INVALID_ID;
    if (!checkId(judgeId))return EUROVISION_INVALID_ID;

    if (!eurovision->judges) {
        eurovision->judges = mapCreate(copyJudge, copyInt, freeJudge, freeInt, compareIntKeys);
        assert(eurovision->judges);
        if (eurovision->judges == NULL) return EUROVISION_OUT_OF_MEMORY;

    } //Creates states dictionary if there isn't one.
    Map judges = eurovision->judges;


    if (mapContains(judges, &judgeId)) return EUROVISION_STATE_ALREADY_EXIST;
    if (!checkJudgeResults(judgeResults, eurovision->states)){
        return EUROVISION_STATE_NOT_EXIST;
    }
    Judge new_judge = createNewJudge(judgeId, judgeName, judgeResults);
    if (mapPut(judges, &judgeId, new_judge) == MAP_OUT_OF_MEMORY) {
        return EUROVISION_OUT_OF_MEMORY;
    } else {
        return EUROVISION_SUCCESS;
    }
   // updateScore(eurovision) yet to write
}
