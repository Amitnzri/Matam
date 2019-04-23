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
    int* destination = malloc(sizeof(*destination)* len);
    if(!destination) return NULL;
    assert(source&&destination);
    for(int i=0; i<len; i++){
    destination[i] = source[i];
    }
    return destination;
}

char* copyStr(const char* str){
/*********
TODO:CHECK
*********/
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

static MapDataElement copyState(MapDataElement state){
    /***********
    TODO: Check
    1.check types
    ***********/
    if(!state) return NULL;
    State copy = malloc(sizeof(*copy));
    assert(copy);
    if(!copy) return NULL;
    State source = (State) source;

    copy->name = copyStr(source->name);
    copy->song = copyStr(source->song);
    copy->top_ten = copyIntArray(source->top_ten,TOP_TEN_LEN);
    copy->votes = mapCopy(source->votes);
    //if one of the allocation went wrong, free all and return null.
    if(!copy->name||!copy->song||!copy->top_ten||!copy->votes){
      free(copy->name);
      free(copy->song);
      free(copy->top_ten);
      free(copy->votes);
      free(copy);
      assert(0);
      return NULL;
    }

    copy->id = source->id;
    copy->score_by_judges = source->score_by_judges;
    copy->score_by_audience = source->score_by_audience;

    return (MapDataElement) copy;

}

static Judge copyJudge(MapDataElement judge){
    /***********
    TODO: Check
    ***********/
    if(!judge) return NULL;
    Judge source =(Judge) judge;
    Judge copy = malloc(sizeof(*judge));
    assert(copy);
    if(!copy) return NULL;
    copy->id = source->id;
    strcpy(copy->name,source->name);
    assert(copy->top_ten);
    copy->top_ten = copyIntArray(source->top_ten,TOP_TEN_LEN);
    if(!copy->top_ten)return NULL;
    return copy;

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

static void freeint(MapKeyElement n){
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

static bool checkStateId(int state_id){
    return (state_id<0) ? (false) : (true);
}//Checked

static bool checkName(const char *state_name){

    printf("[+]Got:%s\n",state_name);
    int len = strlen(state_name);
    for(const char *c = state_name ;c<state_name+len ;c++){
        printf("Char:%c,%d\n",*c,*c);
        if((*c<'a'||*c>'z')&&(*c!=SPACE))return false;
    }
    return true;
}//Checked

static State createNewState (int state_id ,const char* state_name,const char* song_name){

  /*********
  TODO:Check
  *********/
  assert(state_id&&state_name&&song_name);

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
  return new_state;
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
    assert(eurovision&&stateId&&stateName&&songName);
                                                        //Validation Checks
    if(!eurovision||!stateId||!stateName
                            ||!songName)return EUROVISION_NULL_ARGUMENT;
    if(!checkName(stateName)||!checkName(songName))return EUROVISION_INVALID_NAME;
    if(!checkStateId(stateId)) return EUROVISION_INVALID_ID;


    if(!eurovision->states){
        eurovision->states = mapCreate(copyState,copyInt,freeState
                                       ,freeint,compareIntKeys);
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
