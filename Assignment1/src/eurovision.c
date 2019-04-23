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
//a-z:97-122, " " =32

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
    int top_ten[TOP_TEN_LEN];
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
    int top_ten[TOP_TEN_LEN];
}*Judge;

/*****************************StaticFunctions********************************/

static void copyIntArray(int *destination,int *source, int len){
    /***********
    TODO: Checks
    ***********/
    assert(source&&destination);
    for(int i=0; i<len; i++){
    destination[i] = source[i];
    }
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
    copy->id =  source->id;
    strcpy(copy->name,source->name);
    strcpy(copy->song,source->song);
    assert(copy->top_ten);
    copyIntArray(copy->top_ten,source->top_ten,TOP_TEN_LEN);
    assert(!copy->votes);
    copy->votes = mapCopy(source->votes);
    assert(copy->votes);
    if(!copy->votes)return NULL;
    copy->score_by_judges = source->score_by_judges;
    copy->score_by_audience = source->score_by_audience;
    return (MapDataElement) copy;

}

static Judge copyJudge(Judge judge){
    /***********
    TODO: Check
    ***********/
    if(!judge) return NULL;
    Judge copy = malloc(sizeof(*judge));
    assert(copy);
    if(!copy) return NULL;
    copy->id = judge->id;
    strcpy(copy->name,judge->name);
    assert(copy->top_ten);
    copyIntArray(copy->top_ten,judge->top_ten,TOP_TEN_LEN);

    return copy;

}

static void freeJudge(Judge judge){
    /***********
    TODO: Check
    1.free name.
    ***********/
    assert(judge);
    if(!judge->name) {
        free(judge);
    }
}

static void freeint(MapKeyElement n){
    /**********
    TODO: Check
    **********/
    free(n);
}

static void freeState(MapDataElement state){return;} //TODO

static int compareIntKeys(MapKeyElement key1,MapKeyElement key2){return 0;} //TODO,Check convention.

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

static State createNewState (int state_id ,const char* state_name,const char* song_name);

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
