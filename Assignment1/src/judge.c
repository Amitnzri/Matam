#include <stdlib.h>
#include <string.h>
#include "../include/judge.h"
#include "../include/defines.h"
#include <assert.h>



struct Judge_t{

    int id;
    char* name;
    int* top_ten;
};

/*****************************StaticFunctions********************************/
//Copies an array.
static int* copyArray(const int* array, int len){
    if(!array) return NULL;
    int* copy = malloc(sizeof(*copy)*len);
    if(!copy) return NULL;
    for(int i=0;i<len;i++){
        copy[i] = array[i];
    }
    return copy;
}
//Copies a name.
static char* copyName(const char* name){
    int len = strlen(name)+1;
    char* copy = malloc(sizeof(*copy)*len);
    if(!copy) return NULL;
    strcpy(copy,name);
    return copy;
}

/*****************************Functions**************************************/
Judge judgeCreate(int id,const char* name, int* votes){
    Judge new_judge = malloc(sizeof(*new_judge));
    if(!new_judge) return NULL;
    new_judge->id = id;
    new_judge->name = copyName(name);
    new_judge->top_ten = copyArray(votes,TOP_TEN_LEN);
    if(!new_judge->name||!new_judge->top_ten){
        free(new_judge->name);
        free(new_judge->top_ten);
        free(new_judge);
        return NULL;
    }
    return new_judge;
}

int judgeGetId(Judge judge){
    if(!judge) return NULL_ARGUMENT;
    return judge->id;
}

char* judgeGetName(Judge judge){
    if(!judge)return NULL;
    return judge->name;
}

int* judgeGetVotes(Judge judge){
    if(!judge)return NULL;
    return judge->top_ten;
}

void judgeDestroy(Judge judge){
    if(!judge)return;
    free(judge->name);
    free(judge->top_ten);
    free(judge);
}

Judge judgeCopy(Judge judge){
    assert(judge);
    if(!judge)return NULL;
    Judge judge_copy = malloc(sizeof(*judge_copy));
    if(!judge_copy)return NULL;
    judge_copy->id = judge->id;
    judge_copy->name = copyName(judge->name);
    judge_copy->top_ten = copyArray(judge->top_ten,TOP_TEN_LEN);
    if(!judge_copy->name || (!judge_copy->top_ten&&judge->top_ten)){
        free(judge_copy->name);
        free(judge_copy->top_ten);
        free(judge_copy);
        return NULL;
    }
    return judge_copy;


}
