#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "eurovision.h"
#include <string.h>
#include "map.h"


/*********************************Defines************************************/

#define TOP_TEN_LEN 10

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
    for(int i=0; i<len; i++){
    destination[i] = source[i];
    }
}

static int* copyInt (int* key){
    /***********
    TODO: Check
    ***********/
    assert(key);
    if(!key) return NULL;
    int* copy = malloc(sizeof(*copy));
    assert(copy);
    if(!copy) return NULL;
    *copy = *key;
    return copy;
}

static State copyState(State state){
    /***********
    TODO: Check
    ***********/
    if(!state) return NULL;
    State copy = malloc(sizeof(*copy));
    assert(copy);
    if(!copy) return NULL;

    copy->id = state->id;
    strcpy(copy->name,state->name);
    strcpy(copy->song,state->song);
    assert(copy->top_ten);
    copyIntArray(copy->top_ten,state->top_ten,TOP_TEN_LEN);
    assert(!copy->votes);
    copy->votes = mapCopy(state->votes);
    assert(copy->votes);
    if(!copy->votes)return NULL;
    copy->score_by_judges = state->score_by_judges;
    copy->score_by_audience =state->score_by_audience;
    return copy;

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

static void freeint(int* n){
    /**********
    TODO: Check
    **********/
    free(n);
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
