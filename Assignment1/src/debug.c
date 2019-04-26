#include "map.h"
#include "eurovision.h"
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#define NONE -1
#define TOP_TEN_LEN 10
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

static void freeInt(MapKeyElement n){
    /**********
    TODO: Check
    **********/
    free(n);
}

static int compareIntKeys(MapKeyElement key_a,MapKeyElement key_b){
  /*********
  TODO:Check
  *********/
  assert(key_a&&key_b);
  return *(int*)key_a - *(int*)key_b;
}

static void resetArray(int* arr){
    for(int i=0;i<TOP_TEN_LEN;i++){
        arr[i] = NONE;
    }
}
//Swaps two given values.
static void swap(int* a,int* b){
  int tmp = *a;
  *a = *b;
  *b = tmp;
}
//Updates the top_ten array.
static EurovisionResult updateTopTen(Map votes_map,int* top_ten){
    /*********
    TODO:Check
    TEST TOMORROW
    *********/
    assert(top_ten);
    if(!votes_map || !top_ten){
        return EUROVISION_NULL_ARGUMENT;
    }
    resetArray(top_ten);
    int last_id = *(int*) mapGetLast(votes_map);
    int state_id = *(int*)mapGetFirst(votes_map);
    int state_score;

    do{//Scan the map
        state_score = *(int*) mapGet(votes_map,&state_id);
        int tmp = state_id;
        for(int i=0;i<TOP_TEN_LEN;i++){
            if(top_ten[i] == NONE || top_ten[i] == state_id){
                top_ten[i] = state_id;
                break;
            }else{
                //Look for a new place for the replaced state.
                if(state_score>top_ten[i]){

                    swap(&state_id,&top_ten[i]);
                    state_score = *(int*) mapGet(votes_map,&state_id);
               }
            }
        }

      if(last_id == *(int*)mapGet(votes_map,&tmp))break; //Move back to the previous states.
      state_id = *(int*)mapGetNext(votes_map);
    }while(state_id <= last_id); //Move to the next State.
      return EUROVISION_SUCCESS;
  }




int main(){
    Map votes_simulation = mapCreate(copyInt,copyInt,freeInt,freeInt,compareIntKeys);
    //eurovisionAddState(eurovision,0,"israel","home");
    int arr[10] = {1,2,3,4,5,6,7,8,9,10};
    int* top_ten = malloc(sizeof(*top_ten)*TOP_TEN_LEN);
    for(int i=1;i<=10;i++){
          mapPut(votes_simulation,&i,&arr[i-1]);
    }
    int res = updateTopTen(votes_simulation,top_ten);
    for(int i=0;i<10;i++){
        printf("State:%d\n",top_ten[i]);
    }

    printf("Code:%d\n",res);
    free(top_ten);
    mapDestroy(votes_simulation);

  return 0;
}
