#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "../include/defines.h"
#include "../include/eurovision.h"
#include "../include/judge.h"
#include "../include/state.h"
#include "../include/map.h"
#include "../include/set.h"


/*****************************DataSturctures*********************************/

struct eurovision_t{
    Map states_map;
    Map judges_map;
    List scores_table;
    ContestValues contest_values; //set to one by default
};

/*****************************StaticFunctions********************************/
//Copy int function for Map.
static MapKeyElement copyInt (MapKeyElement key){
    assert(key);
    if(!key) return NULL;
    int* copy = malloc(sizeof(*copy));
    assert(copy);
    if(!copy) return NULL;
    *copy = *(int*)key;
    return (MapKeyElement) copy;
}
//Copy judge function for Map.
static MapDataElement copyJudge(MapDataElement judge){
    if(!judge) return NULL;
    Judge copy = judgeCopy(judge);
    return (MapDataElement) copy;
}
//Free judge allocation.
static void freeJudge(MapDataElement judge){
    assert(judge);
    if(!judge) return;
    judgeDestroy(judge);
}
//Free int function for Map.
static void freeInt(MapKeyElement n){
    free(n);
}
//Copy name function for List.
static ListElement copyName(ListElement str){
    assert(str);
    if(!str)return NULL;
    char* copy = malloc(sizeof(*copy)*(strlen(str)+1));
    if(!copy) return NULL;
    strcpy(copy,str);
    return (ListElement) copy;
}
//free str function for list.
static void freeStr(ListElement str){
    free(str);
}
//Free state function for Map.
static void freeState(MapDataElement state){
    assert(state);
    if(state == NULL)return;
    stateDestroy(state);
}
//Comapares two int keys.
static int compareIntKeys(MapKeyElement key_a,MapKeyElement key_b){
    assert(key_a&&key_b);
    return *(int*)key_a - *(int*)key_b;
}
//Checks if ID is valid.
static bool checkId(int state_id){
    return (state_id<0) ? (false) : (true);
}
//Checks if name is valid.
static bool checkName(const char *state_name){
    int len = strlen(state_name);
    for(const char *c = state_name ;c<state_name+len ;c++){
        if((*c<'a'||*c>'z')&&(*c!=SPACE))return false;
    }
    return true;
}
//Checks if state id shows up more than once inside judge's top_ten array.
static bool lookForDuplicates(const int* array,int value){
    int counter =0;
    for(int i=0;i<TOP_TEN_LEN;i++){
        if(array[i] == value) counter++;
    }
    assert(counter>=1);
    return (counter ==1) ? (true) : (false);
}
//Ckecks if  an array of ID numbers is valid.
static bool checkArrayValues(int len, int* judge_results){

    assert(judge_results);
    if(!judge_results) return false;
    for(int i = 0;i<len;i++){
        if(!checkId(judge_results[i]))return false;
        if(!lookForDuplicates(judge_results,judge_results[i])) return false;
    }
    return true;
}
//Checks if judges votes are valid.
static bool checkJudgeResults(int* judge_results, Map states_map){
    if(!states_map) return false; //if there's no states
    for(int i=0;i<TOP_TEN_LEN;i++){
        if(!mapContains(states_map,&judge_results[i])) return false;
    }
    return true;
}
//Copy state function for Map.
static MapDataElement copyState(MapDataElement state){
    if(!state) return NULL;
    State copy = stateCopy(state);
    return (MapDataElement) copy;
}
//Creates shallow copy to a state.
static ListElement copyStateShallow(ListElement state_ptr){
    assert(state_ptr);
    return state_ptr;
}
//free state function for List.Does nothing the List holds shallow copy.
static void freeListState(ListElement state_ptr){
    return;
}
//Compare scores by the audience score.(compare function for List)
static int compareAudienceScore(ListElement element_a, ListElement element_b){
    assert(element_a&&element_b);
    State state_a = (State) element_a;
    State state_b = (State) element_b;
    if(stateGetScoreByStates(state_a) - stateGetScoreByStates(state_b) !=0){
    return (stateGetScoreByStates(state_b) - stateGetScoreByStates(state_a));
    }else{
        return (stateGetId(state_a) - stateGetId(state_b));
    }
}
//Converts the location from the voting table to points.
static int convertPlaceToPoints(int i){
    assert(0<=i && i<TOP_TEN_LEN);
    return (i<2) ? (12-2*i) : (10-i);
}
//Add or Remove all votes from TopTen Array.
static void addOrRemoveOwnVotes(Map states_map,int* top_ten,Voter who_voted,
                                                            Operation sign){

    if(!states_map||!top_ten)return;
    for(int i=0;i<TOP_TEN_LEN;i++){
        State state = mapGet(states_map,&top_ten[i]);
        if(!state) continue;
        switch (who_voted) {
            case JUDGE:

                stateUpdateVotes(state,JUDGE,convertPlaceToPoints(i)*sign);
                break;

            case STATE:
                stateUpdateVotes(state,STATE,convertPlaceToPoints(i)*sign);
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
    assert(eurovision);
    Map judges_map = eurovision->judges_map;
    if(!judges_map)return;
    MapKeyElement judge_id = mapGetFirst(judges_map);
    //Scans every judge in the map and checks his votes.
    while(judge_id){
        Judge judge = mapGet(judges_map,judge_id);
        int* top_ten = judgeGetVotes(judge);
        if(checkIfArrayContainsID(top_ten,state_id)){
            addOrRemoveOwnVotes(eurovision->states_map,top_ten,JUDGE,SUBTRACT);
            mapRemove(judges_map,judge_id);
            judge_id = mapGetFirst(judges_map);
        }else{
            judge_id = mapGetNext(judges_map);
        }
    }
}
//Cancels votes of other states to removed State.
static void cancelOtherStatesVotes(Map states_map,int removed_state){
      assert(states_map);
      MapKeyElement state_id = mapGetFirst(states_map);
      //Scans every state in the map and checks his votes.
      while(state_id){
          State state = mapGet(states_map,state_id);
          if(stateGetVotes(state)){
              stateClearVotes(state,removed_state);//Check the function
              int* top_ten =stateGetVotes(state);
              addOrRemoveOwnVotes(states_map,top_ten,STATE,SUBTRACT);
              stateUpdateTopTen(state);
              top_ten =stateGetVotes(state);
              addOrRemoveOwnVotes(states_map,top_ten,STATE,ADD);
              mapGet(states_map,state_id);
          }
          state_id = mapGetNext(states_map);
      }
}
//Calculates final score
static double calculateFinalScore(float audience_percent, State state){
    assert(state);
    ContestValues contest_values = stateGetContestValues(state);
    int num_of_states = contest_values->num_of_states;
    int num_of_judges = contest_values->num_of_judges;
    int score_by_states = stateGetScoreByStates(state);
    int score_by_judges = stateGetScoreByJudges(state);
    double state_averge = (double)(score_by_states)/(num_of_states-1);
    double judges_averge = (double)(score_by_judges)/num_of_judges;
    return((audience_percent*state_averge)+(1-audience_percent)*judges_averge);

}
//Compares Two states by their final score.(Compare function for List)
static int compareFinalScore(ListElement element_a, ListElement element_b) {
    assert(element_a&&element_b);
    State state_a = (State) element_a;
    State state_b = (State) element_b;
    ContestValues contest_values = stateGetContestValues(state_a);
    int a_id = stateGetId(state_a);
    int b_id = stateGetId(state_b);

    float audience_percent = (float)(contest_values->audience_percent)/100;
    double final_score_a = calculateFinalScore(audience_percent,state_a);
    double final_score_b = calculateFinalScore(audience_percent,state_b);
    if(final_score_a == final_score_b) return (b_id-a_id);
    return (final_score_b>final_score_a) ? (1) : (-1);
}
//Updates the scores table.
static List updateScoreTable(List scores_table,Map states_map,
                             ContestValues contest_values){

    if(!scores_table){
        scores_table = listCreate(copyStateShallow,freeListState);
        if(!scores_table) return NULL;
    }
    //Check if the list is updated.
    if(listGetSize(scores_table) != contest_values->num_of_states){
        listClear(scores_table);
        int* state_id = (int*) mapGetFirst(states_map);
        while(state_id){
            listInsertFirst(scores_table,mapGet(states_map,state_id));
            state_id = (int*) mapGetNext(states_map);
        }
    }
    //Check the size again for safety.
    if(listGetSize(scores_table)!= contest_values->num_of_states){
        listDestroy(scores_table);
        return NULL;
    }
    return scores_table;
}
//Creates a List of names.
static List createWinnersNamesList(List scores_table){
    assert(scores_table);
    List states_names_list = listCreate(copyName,freeStr);
    if(!states_names_list) return NULL;
    State state = (State) listGetFirst(scores_table);
    while(state){
        listInsertLast(states_names_list,stateGetName(state));
        state = (State) listGetNext(scores_table);
    }
    return states_names_list;
}
//Checks if state B gave 12 point to the state which gave her 12 points
static int* checkIfFriendly(Map states_map,int* state_a_id){
    assert(states_map&&state_a_id);
    State state_a = mapGet(states_map,state_a_id);
    if(!stateGetVotes(state_a)) return NULL;
    int* state_b_id = stateGetVotes(state_a);
    State state_b = mapGet(states_map,state_b_id);
    if(!state_b) return NULL;
    int * b_top_ten = stateGetVotes(state_b);
    if(!b_top_ten) return NULL;
    if(*b_top_ten == *state_a_id){
        return state_b_id;
    }
    else {
        return NULL;
    }
}
//Converts 2 names into one string
static void makingPair(const char* name_a, const char* name_b, char* pair){
    char* space = " - ";
    strcpy(pair,name_a);
    strcpy(pair+strlen(name_a),space);
    strcpy(pair+strlen(pair),name_b);
}
//Inserts a pair of states into the friendly state list
static List insertPair(Map map_state,List friendly_list,int* a_id,int* b_id){
    State state_a =  (mapGet(map_state,a_id));
    char* name_a = stateGetName(state_a);
    State state_b =  (mapGet(map_state,b_id));
    char* name_b = stateGetName(state_b);
    char* pair = (char*)malloc(sizeof(char)*(strlen(name_a)+strlen(name_b)+4));
    if(!pair)
        return NULL;

    if(strcmp(name_a,name_b)<=0){
        makingPair(name_a,name_b,pair);
    }else{
        makingPair(name_b,name_a,pair);
    }
    listInsertLast(friendly_list,pair);
    free(pair);
    return friendly_list;
}
//Compaires to elements lexicographicly.
static int compareLexicographicOrder(ListElement element_a,
                                      ListElement element_b){
    return strcmp((char*)element_a,(char*)element_b);
}

/*****************************Functions**************************************/

Eurovision eurovisionCreate(void){
    Eurovision eurovision = malloc(sizeof(*eurovision));
    assert(eurovision);
    if(!eurovision) return NULL;
    eurovision->contest_values = malloc(sizeof(*eurovision->contest_values));
    if(!eurovision->contest_values){
        free(eurovision);
        return NULL;
    }
    eurovision->contest_values->audience_percent = 100;
    eurovision->contest_values->num_of_states =0;
    eurovision->contest_values->num_of_judges =0;
    eurovision->states_map = NULL;
    eurovision->judges_map = NULL;
    eurovision->scores_table = NULL;
    return eurovision;
}

EurovisionResult eurovisionAddState(Eurovision eurovision,
                                    int stateId,
                                    const char *stateName,
                                    const char *songName){
    assert(eurovision&&stateName&&songName);
                                                        //Validation Checks
    if(!eurovision||!stateName||!songName)return EUROVISION_NULL_ARGUMENT;
    if(!checkId(stateId)) return EUROVISION_INVALID_ID;
    if(!checkName(stateName)
       ||!checkName(songName))return EUROVISION_INVALID_NAME;


    if(!eurovision->states_map){
        eurovision->states_map = mapCreate(copyState,copyInt,freeState
                                       ,freeInt,compareIntKeys);
        assert(eurovision->states_map);
        if(eurovision->states_map == NULL) return EUROVISION_OUT_OF_MEMORY;
    } //Creates states dictionary if there isn't one.
    Map states_map = eurovision->states_map;

    if(mapContains(states_map,&stateId)) return EUROVISION_STATE_ALREADY_EXIST;
    State tmp_state = stateCreate(stateId,stateName,songName,
                                  eurovision->contest_values);
    if(mapPut(states_map,&stateId,tmp_state) == MAP_OUT_OF_MEMORY){
        stateDestroy(tmp_state);
        return EUROVISION_OUT_OF_MEMORY;
    }else{
        stateDestroy(tmp_state);
        eurovision->contest_values->num_of_states++;
        return EUROVISION_SUCCESS;
    }
}

EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId){
    assert(eurovision);
    if(!eurovision)return EUROVISION_NULL_ARGUMENT;
        if(!checkId(stateId)) return EUROVISION_INVALID_ID;
    if(!eurovision->states_map||!mapContains(eurovision->states_map,&stateId)){
        return EUROVISION_STATE_NOT_EXIST;
    }
    State remove = (State) mapGet(eurovision->states_map,&stateId);
    if(stateGetVotes(remove))addOrRemoveOwnVotes(eurovision->states_map,
                                       stateGetVotes(remove),STATE,SUBTRACT);
    if(stateGetScoreByJudges(remove)>0)fireTheVotingJudges(eurovision,stateId);
    if(stateGetScoreByStates(remove)>0){
        cancelOtherStatesVotes(eurovision->states_map,stateId);
    }
    if(mapGetSize(eurovision->states_map)<=1){
        mapDestroy(eurovision->states_map);
        eurovision->states_map=NULL;
    }else{
        mapRemove(eurovision->states_map,&stateId);
    }
    eurovision->contest_values->num_of_states--;
    return EUROVISION_SUCCESS;

}

EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId,
                                    const char *judgeName,
                                    int *judgeResults) {
    assert(eurovision && judgeName);
    if (!eurovision  || !judgeName ||!judgeResults){
        return EUROVISION_NULL_ARGUMENT;
    }
    if (!checkArrayValues(TOP_TEN_LEN,judgeResults) || !checkId(judgeId)){
        return EUROVISION_INVALID_ID;
    }
    if (!checkName(judgeName)) return EUROVISION_INVALID_NAME;
    if (!eurovision->judges_map) {
        eurovision->judges_map = mapCreate(copyJudge, copyInt, freeJudge,
                                           freeInt, compareIntKeys);
        assert(eurovision->judges_map);
        if (eurovision->judges_map == NULL) return EUROVISION_OUT_OF_MEMORY;

    }//Creates states dictionary if there isn't one.
    if (!checkJudgeResults(judgeResults,eurovision->states_map)){
        return EUROVISION_STATE_NOT_EXIST;
    }
    Map judges_map = eurovision->judges_map;
        if (mapContains(judges_map, &judgeId)) {
            return EUROVISION_JUDGE_ALREADY_EXIST;
        }

        Judge tmp_judge = judgeCreate(judgeId, judgeName, judgeResults);

        if (mapPut(judges_map, &judgeId, tmp_judge) == MAP_OUT_OF_MEMORY) {
            judgeDestroy(tmp_judge);
            return EUROVISION_OUT_OF_MEMORY;
        } else {
            judgeDestroy(tmp_judge);
            addOrRemoveOwnVotes(eurovision->states_map,judgeResults,JUDGE,ADD);
            eurovision->contest_values->num_of_judges++;
            return EUROVISION_SUCCESS;
        }
}

EurovisionResult eurovisionRemoveJudge(Eurovision eurovision, int judgeId){
    assert(eurovision);
    if(!eurovision)return EUROVISION_NULL_ARGUMENT;
    if(!checkId(judgeId)) return EUROVISION_INVALID_ID;
    if(!eurovision->judges_map)return EUROVISION_JUDGE_NOT_EXIST;
    if(!mapContains(eurovision->judges_map,&judgeId)){
        if(mapGetSize(eurovision->judges_map)<1){
            mapDestroy(eurovision->judges_map);
            eurovision->judges_map=NULL;
        }
        return EUROVISION_JUDGE_NOT_EXIST;
    }
    Judge remove = (Judge) mapGet(eurovision->judges_map,&judgeId);
    int* top_ten = judgeGetVotes(remove);
    addOrRemoveOwnVotes(eurovision->states_map,top_ten,JUDGE,SUBTRACT);
    if(mapGetSize(eurovision->judges_map)<=1){
        mapDestroy(eurovision->judges_map);
        eurovision->judges_map=NULL;
    }else{
        mapRemove(eurovision->judges_map,&judgeId);
    }
    eurovision->contest_values->num_of_judges--;
    return EUROVISION_SUCCESS;
}

void eurovisionDestroy(Eurovision eurovision){
    if(!eurovision)return;
    mapDestroy(eurovision->states_map);
    mapDestroy(eurovision->judges_map);
    free(eurovision->contest_values);
    listDestroy(eurovision->scores_table);
    free (eurovision);

}

EurovisionResult eurovisionAddVote (Eurovision eurovision,int stateGiver,
                                                           int stateTaker){
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
    stateAddVote(voter_state,stateTaker);
    int* top_ten = stateGetVotes(voter_state);
    addOrRemoveOwnVotes(eurovision->states_map,top_ten,STATE,SUBTRACT);
    stateUpdateTopTen(voter_state);
    top_ten = stateGetVotes(voter_state);
    addOrRemoveOwnVotes(eurovision->states_map,top_ten,STATE,ADD);

    return EUROVISION_SUCCESS;
}

EurovisionResult eurovisionRemoveVote (Eurovision eurovision, int stateGiver,
                                                              int stateTaker){
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
stateRemoveVote(voter_state,stateTaker); //check this function.
int* top_ten = stateGetVotes(voter_state);
if(top_ten)addOrRemoveOwnVotes(eurovision->states_map,top_ten,STATE,SUBTRACT);
stateUpdateTopTen(voter_state);
top_ten = stateGetVotes(voter_state);
addOrRemoveOwnVotes(eurovision->states_map,top_ten,STATE,ADD);

return EUROVISION_SUCCESS;

}

List eurovisionRunAudienceFavorite(Eurovision eurovision){
    assert(eurovision);
    if(!eurovision) return NULL;
    eurovision->scores_table = updateScoreTable(eurovision->scores_table,
                            eurovision->states_map,eurovision->contest_values);
    if(!eurovision->scores_table) return NULL;
    listSort(eurovision->scores_table,compareAudienceScore);
    return createWinnersNamesList(eurovision->scores_table);
}

List eurovisionRunContest (Eurovision eurovision, int audiencePercent){
    assert(eurovision);
    if(!eurovision) return NULL;
    if(audiencePercent>100 || audiencePercent<0)return NULL;
    eurovision->contest_values->audience_percent =audiencePercent;
    eurovision->scores_table = updateScoreTable(eurovision->scores_table,
                            eurovision->states_map,eurovision->contest_values);
    if(!eurovision->scores_table) return NULL;
    listSort(eurovision->scores_table,compareFinalScore);
    return createWinnersNamesList(eurovision->scores_table);
}

List eurovisionRunGetFriendlyStates(Eurovision eurovision){
    assert(eurovision);
    //creates set and inserts the states in it.
    Set states_set = setCreate(copyInt,freeInt,compareIntKeys);
    if(!states_set)return NULL;
    Map states_map = eurovision->states_map;
    int* state_id = (int*)mapGetFirst(states_map);
    while(state_id){
        setAdd(states_set,state_id);
        state_id = mapGetNext(states_map);
    }
    List friendly_list = listCreate(copyName,freeStr);
    if(!friendly_list){
        setDestroy(states_set);
        return NULL;
    }
    state_id = (int*)mapGetFirst(states_map);
    while(state_id) {

        if(!setIsIn(states_set,state_id)) state_id=(int*)mapGetNext(states_map);

        int *friendly_state = checkIfFriendly(states_map, state_id);
        if (friendly_state) {
            if(!insertPair(states_map,friendly_list,state_id,
                                                    friendly_state)){
                setDestroy(states_set);
                return NULL;
            }
            setRemove(states_set, friendly_state);
        }
        setRemove(states_set,state_id);
        mapGet(states_map, state_id);
        state_id = (int*) mapGetNext(states_map);
    }
    listSort(friendly_list,compareLexicographicOrder);
    setDestroy(states_set);
    return friendly_list;
}
